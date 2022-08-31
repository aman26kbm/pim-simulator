// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t gemm_inner(System* sys){
    std::vector<Request> requests;
    Request *request;

    // int matrixARowNum = 128*16*16;
    // int matrixAColNum = 256*8;
    // int matrixBRowNum = matrixAColNum;
    // int matrixBColNum = sys->_num_regs_per_rf;

    // int matrixARowNum = sys->_ntiles;
    // int matrixAColNum = sys->_nblocks*sys->_ncols;
    // int matrixBRowNum = matrixAColNum;
    // int matrixBColNum = sys->_num_regs_per_rf;

    int matrixARowNum = 128*16;
    int matrixAColNum = 128*16;
    int matrixBRowNum = matrixAColNum;
    int matrixBColNum = sys->_num_regs_per_rf;


    int basicMatrixARowNum = sys->_ntiles;
    int basicMatrixAColNum = sys->_nblocks*sys->_ncols;
    int basicMatrixBRowNum = matrixAColNum;
    int basicMatrixBColNum = sys->_num_regs_per_rf;

    int RowReduce_WithinTile_count = log2(matrixAColNum/sys->_ncols);
    int rowReduce_count = log2(sys->_ncols/32);
    for(int i=0; i<matrixARowNum*matrixAColNum/(sys->_ntiles*sys->_nblocks*sys->_ncols); i++){
        for(int tile=0; tile<sys->_ntiles; tile++){
             //a[0:]
                request = new Request(Request::Type::RowLoad);
                request->addOperand(sys->getAddress(tile,0,0), 0, PrecisionT::INT4); //cram addr
                request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //dram addr
                requests.push_back(*request);
        }
        for(int b_col_index=0; b_col_index<basicMatrixBColNum; b_col_index++){
            //b[:0]
            request = new Request(Request::Type::RowLoad);
            request->addOperand(sys->getAddress(0,0,4), 0, PrecisionT::INT4); //cram addr
            request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //dram addr
            requests.push_back(*request);
            //broadcast for tiles
            for(int tile=0; tile+1<sys->_ntiles; tile++){
                request = new Request(Request::Type::TileSend);
                request->addOperand(sys->getAddress(tile,0,4),0,PrecisionT::INT4);//src
                request->addOperand(sys->getAddress(tile+1,0,4),0,PrecisionT::INT4);//dest
                requests.push_back(*request);

                request = new Request(Request::Type::TileReceive);
                request->addOperand(sys->getAddress(tile,0,4),0,PrecisionT::INT4);//src
                request->addOperand(sys->getAddress(tile+1,0,4),0,PrecisionT::INT4);//dest
                requests.push_back(*request);

            }
            
            for(int tile=0; tile<sys->_ntiles; tile++){
               //broadcast for blocks
                for(int block=0; block+(matrixAColNum/sys->_ncols)<sys->_nblocks; block+=(matrixAColNum/sys->_ncols)){
                    request = new Request(Request::Type::BlockSend_Receive);
                    request->addOperand(sys->getAddress(tile,block,4),0,PrecisionT::INT4);//src
                    request->addOperand(sys->getAddress(tile,block+(matrixAColNum/sys->_ncols),4),0,PrecisionT::INT4);//dest
                    requests.push_back(*request);
                }                
                //multiply
                request = new Request(Request::Type::RowMul);
                request->addOperand(sys->getAddress(tile,0,0), 0, PrecisionT::INT4); //src1
                request->addOperand(sys->getAddress(tile,0,4), 0, PrecisionT::INT4);//src2
                request->addOperand(sys->getAddress(tile,0,8), 0, PrecisionT::INT4); //dst
                requests.push_back(*request);
                //reduce to get c0_0
                request = new Request(Request::Type::RowReduce_WithinTile);
                request->addOperand(sys->getAddress(tile,0,8), RowReduce_WithinTile_count, PrecisionT::INT4); //src
                request->addOperand(sys->getAddress(tile,0,12), RowReduce_WithinTile_count, PrecisionT::INT4); //dst
                requests.push_back(*request);
                request = new Request(Request::Type::RowReduce);
                request->addOperand(sys->getAddress(tile,0,12), rowReduce_count, PrecisionT::INT4); //src
                request->addOperand(sys->getAddress(tile,0,12), rowReduce_count, PrecisionT::INT4); //dst
                requests.push_back(*request);
                request = new Request(Request::Type::PopCountReduce_RF);
                request->addOperand(sys->getAddress(tile,0,12), 0, PrecisionT::INT4); //src
                request->addOperand(sys->_num_regs_per_rf * tile, 0, PrecisionT::INT4); //dst
                requests.push_back(*request);
                // int start_block = 0;
                // int round = 0;
                // for(int gap =1; gap<sys->_nblocks; gap = gap*2){
                //     for(int block=start_block; block+gap<sys->_nblocks; block=block+2*gap){
                //         //Send partial results to block that is gap away
                        
                //             request = new Request(Request::Type::BlockSend_Receive);
                //             request->addOperand(sys->getAddress(tile,block,8), 0, PrecisionT::INT4); //src
                //             request->addOperand(sys->getAddress(tile,block+gap,12), 0, PrecisionT::INT16); //dst
                //             requests.push_back(*request);

                //             //add
                //             request = new Request(Request::Type::RowAdd);
                //             request->addOperand(sys->getAddress(tile,block+gap,8), 0, PrecisionT::INT16); //src
                //             request->addOperand(sys->getAddress(tile,block+gap,12), 0, PrecisionT::INT16); //src2
                //             request->addOperand(sys->getAddress(tile,block+gap,8), 0, PrecisionT::INT16); //dst
                //             requests.push_back(*request);
                        

                //     }
                //     start_block += gap;
                //     round++;
                // }  

            }
        }
    }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}



static __attribute__((unused)) Registry::Entry &__gemm_inner__ = pimsim::registerFunc("gemm_inner", gemm_inner);