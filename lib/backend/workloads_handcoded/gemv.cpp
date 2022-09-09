// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

/////////////////////////////////////////////////////////////
// Simple program to perform a matrix-vector mul 
/////////////////////////////////////////////////////////////

int32_t gemv(System* sys)
{
    int matrix_row = 32768*1;//32768 * X
    int matrix_col = 4096 *1;// 4096 * Y
    int use_tiles = 128;
    int dram_tile = 0; //This specifies the location of the DRAM controller (0 implies core 0 is connected to DRAM controller)
    int basic_matrix_row = sys->_ncols * sys->_nblocks;//256*128=32768
    int basic_matrix_col = use_tiles * sys->_num_regs_per_rf;// 128*32=4096
    int effective_basic_col = (matrix_col>basic_matrix_col)?basic_matrix_col:matrix_col;
    int effective_basic_row = (matrix_row>basic_matrix_row)?basic_matrix_row:matrix_row;
    int cols_per_tile = effective_basic_col/use_tiles;//equal to # register iteration <=32
    int col_iter_num = (int)ceil(matrix_col/(double)effective_basic_col);
    int row_iter_num = (int)ceil(matrix_row/(double)effective_basic_row);
    PrecisionT::Precision input_precision = PrecisionT::INT8;
    PrecisionT::Precision multiply_precision = PrecisionT::INT16;
    PrecisionT::Precision accumulate_precision = PrecisionT::INT32;
    //compute for 1 sub-matrix
    //We have 128 tiles
    //Each has 128 CRAMs.
    //So total cram columns = 128 * 256 = 32768
    //Let's consider sub-matrix size to be 32768 rows * 4096 columns.
    //Let's consider sub-vector size to be 512 rows * 1 column.

    //We will load 32 columns of matrix into tile0
    //and 32 columns of matrix into tile1.
    //....
    //That is, 32 elements in one column of each CRAM. So sub-matrix has at most 32*128=4096 cols

    //The vector is outside in RF.
    //32 elements in 1 core's RF, 32 in second core's RF.

    //We will perform 32 multiply-adds in each tile.
    //Then we will send data from tile0 to tile1.
    //And add them up in tile1.
    //This is the result (1024x1 vector).
    //One element of the result is in each column of tile1's CRAMs.
    //We perform reduce on 128 tiles
    //Then store results on tile 127 to DRAM.
    std::vector<Request> requests;
    Request *request;
    for(int row_iteration =0; row_iteration<row_iter_num; row_iteration++){
        for(int col_iteration=0; col_iteration<col_iter_num; col_iteration++){
            for(int iter_tile=dram_tile; iter_tile<dram_tile+use_tiles; iter_tile++){
                int tile = iter_tile % sys->_ntiles;
                //Load weight matrix values into CRAM
                //This is typically done beforehand/offline.
                //Each core is loading separate data, so no broadcasting here.
                //No need for synchronization either because different
                //cores load data that only they will need/use.
                //4 elements in each column need to be loaded.
                for (int i=0; i<cols_per_tile; i++) {
                    request = new Request(Request::Type::RowLoad);
                    request->addOperand(sys->getAddress(tile,0,i*4), 0, input_precision); //dst
                    request->addOperand(sys->DRAM_ADDR, 0, input_precision); //src
                    requests.push_back(*request);
                }

                //Now load vector values into RF.
                //Only 4 elements need to be loaded per core, so just 1 instruction is enough.
                request = new Request(Request::Type::RowLoad_RF);
                request->addOperand(sys->_num_regs_per_rf * tile, 4, input_precision); //dst
                request->addOperand(sys->DRAM_ADDR, 4, input_precision); //src
                requests.push_back(*request);

                //Initialize rows that'll hold the accumulator (accumulator size=16)
                request = new Request(Request::Type::RowReset);
                request->addOperand(sys->getAddress(tile,0,cols_per_tile*4), 0, accumulate_precision); //src
                request->addOperand(sys->getAddress(tile,0,cols_per_tile*4), 0, accumulate_precision); //dst
                requests.push_back(*request);

                //32 multiplications and additions into the accumulator
                for (int i=0; i<cols_per_tile; i++) {
                    //Read an element from the RF
                    request = new Request(Request::Type::RowRead_RF);
                    request->addOperand(sys->_num_regs_per_rf * tile, 0, input_precision); //src
                    requests.push_back(*request);

                    //Multiply in this core/tile.
                    request = new Request(Request::Type::RowMul_CRAM_RF);
                    request->addOperand(sys->getAddress(tile,0,i*4), 0, input_precision); //src
                    request->addOperand(sys->_num_regs_per_rf * tile, 4, input_precision);//rf
                    request->addOperand(sys->getAddress(tile,0,i*4), 0, multiply_precision); //dst
                    requests.push_back(*request);

                    //Add the result of multiplication into the accumulator
                    request = new Request(Request::Type::RowAdd);
                    request->addOperand(sys->getAddress(tile,0,i*4), 0, multiply_precision); //src
                    request->addOperand(sys->getAddress(tile,0,cols_per_tile*4), 0, accumulate_precision);//src2
                    request->addOperand(sys->getAddress(tile,0,cols_per_tile*4), 0, accumulate_precision); //dst
                    requests.push_back(*request);
                }
            }
            //reduce
            int round = 0;
            if(use_tiles>1){                
                for(int gap =1; gap<use_tiles; gap = gap*2){
                    for(int iter_tile=dram_tile; iter_tile+gap<dram_tile+use_tiles; iter_tile=iter_tile+2*gap){
                        int tile = iter_tile % sys->_ntiles;
                        //receive partial results from tile that is gap away
                        request = new Request(Request::Type::TileSend);
                        request->addOperand(sys->getAddress((tile+gap)%sys->_ntiles,0,cols_per_tile*4 + round*16), 0, accumulate_precision); //src
                        request->addOperand(sys->getAddress(tile,0,cols_per_tile*4 + round*16 + 16), 0, accumulate_precision); //dst
                        requests.push_back(*request);

                        request = new Request(Request::Type::TileReceive);
                        request->addOperand(sys->getAddress((tile+gap)%sys->_ntiles,0,cols_per_tile*4 + round*16), 0, accumulate_precision); //src
                        request->addOperand(sys->getAddress(tile,0,cols_per_tile*4 + round*16 + 16), 0, accumulate_precision); //dst
                        requests.push_back(*request);

                        //add
                        request = new Request(Request::Type::RowAdd);
                        request->addOperand(sys->getAddress(tile,0,cols_per_tile*4 + round*16), 0, accumulate_precision); //src
                        request->addOperand(sys->getAddress(tile,0,cols_per_tile*4 + round*16 + 16), 0, accumulate_precision); //src2
                        request->addOperand(sys->getAddress(tile,0,cols_per_tile*4 + round*16 + 16), 0, accumulate_precision); //dst
                        requests.push_back(*request);

                    }
                    round++;
                }
            }

            //Store results into DRAM
            request = new Request(Request::Type::RowStore);
            request->addOperand(sys->getAddress(dram_tile, 0, cols_per_tile*4 + round*16), 0, accumulate_precision); //src
            request->addOperand(sys->DRAM_ADDR, 0, accumulate_precision); //dst
            requests.push_back(*request);
        }
        if(col_iter_num>1){
            for(int col_iteration=0; col_iteration<col_iter_num; col_iteration++){
                //load results from DRAM
                request = new Request(Request::Type::RowLoad);
                request->addOperand(sys->getAddress(dram_tile, 0, 0), 0, accumulate_precision); //src
                request->addOperand(sys->DRAM_ADDR, 0,accumulate_precision); //dst
                requests.push_back(*request);
                //accumulate partial results
                request = new Request(Request::Type::RowAdd);
                request->addOperand(sys->getAddress(dram_tile, 0, 0), 0, accumulate_precision); //src
                request->addOperand(sys->getAddress(dram_tile, 0, 16), 0, accumulate_precision); //src2
                request->addOperand(sys->getAddress(dram_tile, 0, 16), 0, accumulate_precision); //dst
                requests.push_back(*request);
            }
            for(int col_iteration=0; col_iteration<col_iter_num; col_iteration++){
                
            }
            //Store results into DRAM
            request = new Request(Request::Type::RowStore);
            request->addOperand(sys->getAddress(dram_tile, 0, 16), 0, accumulate_precision); //src
            request->addOperand(sys->DRAM_ADDR, 0,accumulate_precision); //dst
            requests.push_back(*request);
        }
    }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);

    return 0;
}

static __attribute__((unused)) Registry::Entry &__gemv__ = pimsim::registerFunc("gemv", gemv);