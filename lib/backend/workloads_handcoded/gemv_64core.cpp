// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

/////////////////////////////////////////////////////////////
// Ignore this benchmark
/////////////////////////////////////////////////////////////

int32_t gemv_64core(System* sys)
{
    //hardware config
    int tile_num = 64;
    int array_dim = 256;
    int regs_per_rf = 32;
    //matrix size
    int matrix_row = tile_num*array_dim*4;//
    int matrix_col = tile_num*regs_per_rf*8;//


    int sub_matrix_row = tile_num*array_dim;
    int sub_matrix_col = tile_num*regs_per_rf;
    int total_col = (matrix_col>sub_matrix_col)?sub_matrix_col:matrix_col;
    int total_row = (matrix_row>sub_matrix_row)?sub_matrix_row:matrix_row;
    
    int cols_per_tile = total_col/tile_num;
    int col_iter_num = (int)ceil(matrix_col/(double)total_col);
    int row_iter_num = (int)ceil(matrix_row/(double)total_row);
    //compute for 1 sub-matrix
    //We have 128 tiles
    //Each has 128 CRAMs.
    //So total cram columns = 64 * 256 = 16384
    //Let's consider sub-matrix size to be 32768 rows * 4096 columns.
    //Let's consider sub-vector size to be 512 rows * 1 column.

    //We will load 32 columns of matrix into tile0
    //and 32 columns of matrix into tile1.
    //....
    //That is, 32 elements in one column of each CRAM. So sub-matrix has at most 32*64=2048 cols

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
            for(int tile=0; tile<tile_num; tile++){
                //Load weight matrix values into CRAM
                //This is typically done beforehand/offline.
                //Each core is loading separate data, so no broadcasting here.
                //No need for synchronization either because different
                //cores load data that only they will need/use.
                //4 elements in each column need to be loaded.
                for (int i=0; i<cols_per_tile; i++) {
                    request = new Request(Request::Type::RowLoad);
                    request->addOperand(sys->getAddress(tile,0,i*4), 0, PrecisionT::INT4); //dst
                    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //src
                    requests.push_back(*request);
                }

                //Now load vector values into RF.
                //Only 4 elements need to be loaded per core, so just 1 instruction is enough.
                request = new Request(Request::Type::RowLoad_RF);
                request->addOperand(sys->_num_regs_per_rf * tile, 4, PrecisionT::INT4); //dst
                request->addOperand(sys->DRAM_ADDR, 4, PrecisionT::INT4); //src
                requests.push_back(*request);

                //Initialize rows that'll hold the accumulator (accumulator size=16)
                request = new Request(Request::Type::RowReset);
                request->addOperand(sys->getAddress(tile,0,cols_per_tile*4), 0, PrecisionT::INT16); //src
                request->addOperand(sys->getAddress(tile,0,cols_per_tile*4), 0, PrecisionT::INT16); //dst
                requests.push_back(*request);

                //4 multiplications and additions into the accumulator
                for (int i=0; i<cols_per_tile; i++) {
                    //Read an element from the RF
                    request = new Request(Request::Type::RowRead_RF);
                    request->addOperand(sys->_num_regs_per_rf * tile, 0, PrecisionT::INT4); //src
                    requests.push_back(*request);

                    //Multiply in this core/tile.
                    request = new Request(Request::Type::RowMul_CRAM_RF);
                    request->addOperand(sys->getAddress(tile,0,i*4), 0, PrecisionT::INT4); //src
                    request->addOperand(sys->_num_regs_per_rf * tile, 4, PrecisionT::INT4);//rf
                    request->addOperand(sys->getAddress(tile,0,i*4), 0, PrecisionT::INT4); //dst
                    requests.push_back(*request);

                    //Add the result of multiplication into the accumulator
                    request = new Request(Request::Type::RowAdd);
                    request->addOperand(sys->getAddress(tile,0,i*4), 0, PrecisionT::INT16); //src
                    request->addOperand(sys->getAddress(tile,0,cols_per_tile*4), 0, PrecisionT::INT16);//src2
                    request->addOperand(sys->getAddress(tile,0,cols_per_tile*4), 0, PrecisionT::INT16); //dst
                    requests.push_back(*request);
                }
            }
            //reduce
            int start_tile = 0;
            int round = 0;
            for(int gap =1; gap<tile_num; gap = gap*2){
                for(int tile=start_tile; tile<tile_num; tile=tile+2*gap){
                    //Send partial results to tile that is gap away
                    request = new Request(Request::Type::TileSend);
                    request->addOperand(sys->getAddress(tile,0,cols_per_tile*4 + round*16), 0, PrecisionT::INT16); //src
                    request->addOperand(sys->getAddress(tile+gap,0,cols_per_tile*4 + round*16 + 16), 0, PrecisionT::INT16); //dst
                    requests.push_back(*request);

                    request = new Request(Request::Type::TileReceive);
                    request->addOperand(sys->getAddress(tile,0,cols_per_tile*4 + round*16), 0, PrecisionT::INT16); //src
                    request->addOperand(sys->getAddress(tile+gap,0,cols_per_tile*4 + round*16 + 16), 0, PrecisionT::INT16); //dst
                    requests.push_back(*request);

                    //add
                    request = new Request(Request::Type::RowAdd);
                    request->addOperand(sys->getAddress(tile+gap,0,cols_per_tile*4 + round*16), 0, PrecisionT::INT16); //src
                    request->addOperand(sys->getAddress(tile+gap,0,cols_per_tile*4 + round*16 + 16), 0, PrecisionT::INT16); //src2
                    request->addOperand(sys->getAddress(tile+gap,0,cols_per_tile*4 + round*16 + 16), 0, PrecisionT::INT16); //dst
                    requests.push_back(*request);

                }
                start_tile += gap;
                round++;
            }

            //Store results into DRAM
            request = new Request(Request::Type::RowStore);
            request->addOperand(sys->getAddress(tile_num-1, 0, cols_per_tile*4 + round*16), 0, PrecisionT::INT16); //src
            request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); //dst
            requests.push_back(*request);
        }
        if(col_iter_num>=1){
            for(int col_iteration=0; col_iteration<col_iter_num; col_iteration++){
                //load results from DRAM
                request = new Request(Request::Type::RowLoad);
                request->addOperand(sys->getAddress(tile_num-1, 0, col_iteration*16), 0, PrecisionT::INT16); //src
                request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); //dst
                requests.push_back(*request);
            }
            for(int col_iteration=0; col_iteration<col_iter_num; col_iteration++){
                //accumulate partial results
                request = new Request(Request::Type::RowAdd);
                request->addOperand(sys->getAddress(tile_num-1, 0, col_iteration*16), 0, PrecisionT::INT16); //src
                request->addOperand(sys->getAddress(tile_num-1, 0, col_iter_num*16), 0, PrecisionT::INT16); //src2
                request->addOperand(sys->getAddress(tile_num-1, 0, col_iter_num*16), 0, PrecisionT::INT16); //dst
                requests.push_back(*request);
            }
            //Store results into DRAM
            request = new Request(Request::Type::RowStore);
            request->addOperand(sys->getAddress(tile_num-1, 0, col_iter_num*16), 0, PrecisionT::INT16); //src
            request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); //dst
            requests.push_back(*request);
        }
    }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}

static __attribute__((unused)) Registry::Entry &__gemv_64core__ = pimsim::registerFunc("gemv_64core", gemv_64core);
