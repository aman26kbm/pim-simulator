// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t gemm_inner_broadcast(System* sys){
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
                request->addAddr(sys->getAddress(tile,0,0), 0, PrecisionT::INT4); //cram addr
                request->addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT4); //dram addr
                requests.push_back(*request);
        }
        for(int b_col_index=0; b_col_index<basicMatrixBColNum; b_col_index++){
            //b[:0]
            request = new Request(Request::Type::RowLoad);
            request->addAddr(sys->getAddress(0,0,4), 0, PrecisionT::INT4); //cram addr
            request->addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT4); //dram addr
            requests.push_back(*request);

            //broadcast for tiles
            request = new Request(Request::Type::TileSend_BroadCast);
            request->addAddr(sys->getAddress(0,0,4), 0, PrecisionT::INT4); //src
            requests.push_back(*request);
            request = new Request(Request::Type::Signal, sys->m1);
            request->addAddr(sys->getAddress(0,0,4), 0, PrecisionT::INT4); //src
            requests.push_back(*request);
            for(int tile=1; tile<sys->_ntiles; tile++){
                request = new Request(Request::Type::Wait, sys->m1);
                request->addAddr(sys->getAddress(tile,0,4), 0, PrecisionT::INT4); //src
                requests.push_back(*request);
                request = new Request(Request::Type::TileReceive_BroadCast);
                request->addAddr(sys->getAddress(tile,0,4), 0, PrecisionT::INT4); //src
                requests.push_back(*request);
            }
            
            for(int tile=0; tile<sys->_ntiles; tile++){
               //broadcast for blocks
                request = new Request(Request::Type::BlockBroadCast);
                request->addAddr(sys->getAddress(tile,0,4), matrixAColNum/sys->_ncols, PrecisionT::INT4); //src
                requests.push_back(*request);              
                //multiply
                request = new Request(Request::Type::RowMul);
                request->addAddr(sys->getAddress(tile,0,0), 0, PrecisionT::INT4); //src1
                request->addAddr(sys->getAddress(tile,0,4), 0, PrecisionT::INT4);//src2
                request->addAddr(sys->getAddress(tile,0,8), 0, PrecisionT::INT4); //dst
                requests.push_back(*request);
                //reduce to get c0_0
                request = new Request(Request::Type::RowReduce_WithinTile);
                request->addAddr(sys->getAddress(tile,0,8), RowReduce_WithinTile_count, PrecisionT::INT4); //src
                request->addAddr(sys->getAddress(tile,0,12), RowReduce_WithinTile_count, PrecisionT::INT4); //dst
                requests.push_back(*request);
                request = new Request(Request::Type::RowReduce);
                request->addAddr(sys->getAddress(tile,0,12), rowReduce_count, PrecisionT::INT4); //src
                request->addAddr(sys->getAddress(tile,0,12), rowReduce_count, PrecisionT::INT4); //dst
                requests.push_back(*request);
                request = new Request(Request::Type::PopCountReduce_RF);
                request->addAddr(sys->getAddress(tile,0,12), 0, PrecisionT::INT4); //src
                request->addAddr(sys->_num_regs_per_rf * tile, 0, PrecisionT::INT4); //dst
                requests.push_back(*request);

            }
        }
    }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}



static __attribute__((unused)) Registry::Entry &__gemm_inner_broadcast__ = pimsim::registerFunc("gemm_inner_broadcast", gemm_inner_broadcast);