// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t gemm(System* sys){
    std::vector<Request> requests;
    Request *request;

    int matrixARowNum = sys->_nblocks*sys->_ncols;
    int matrixAColNum = sys->_ntiles;
    int matrixBRowNum = matrixAColNum;
    int matrixBColNum = sys->_num_regs_per_rf;

    for(int tile=0; tile<sys->_ntiles; tile++){
        request = new Request(Request::Type::RowLoad);
        request->addAddr(sys->getAddress(tile,0,0), 0, PrecisionT::INT4); //cram addr
        request->addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT4); //dram addr
        requests.push_back(*request);

        request = new Request(Request::Type::RowLoad_RF);
        request->addAddr(sys->_num_regs_per_rf * tile, sys->_num_regs_per_rf, PrecisionT::INT4); //RF addr
        request->addAddr(sys->DRAM_ADDR, 4, PrecisionT::INT4); //dram addr
        requests.push_back(*request);
        //multiply to get partial sum matrix in 1 core
        for(int regIndex=0; regIndex<sys->_num_regs_per_rf; regIndex++){
            
            request = new Request(Request::Type::RowMul_CRAM_RF);
            request->addAddr(sys->getAddress(tile,0,0), 0, PrecisionT::INT4); //src
            request->addAddr(sys->_num_regs_per_rf * tile + regIndex, 4, PrecisionT::INT4);//rf
            request->addAddr(sys->getAddress(tile,0,4*(regIndex+1)), 0, PrecisionT::INT8); //dst
            requests.push_back(*request);    
                
        }  
    }
    int start_tile = 0;
    int round = 0;
    for(int gap =1; gap<sys->_ntiles; gap = gap*2){
        for(int tile=start_tile; tile<sys->_ntiles; tile=tile+2*gap){
            //Send partial results to tile that is gap away
            for(int resultColIndex=0; resultColIndex<sys->_num_regs_per_rf; resultColIndex++){
                request = new Request(Request::Type::TileSend);
                request->addAddr(sys->getAddress(tile,0,4 + resultColIndex*4), 0, PrecisionT::INT8); //src
                request->addAddr(sys->getAddress(tile+gap,0, 4 + sys->_num_regs_per_rf*4), 0, PrecisionT::INT16); //dst
                requests.push_back(*request);

                request = new Request(Request::Type::TileReceive);
                request->addAddr(sys->getAddress(tile,0,4 + resultColIndex*4), 0, PrecisionT::INT8); //src
                request->addAddr(sys->getAddress(tile+gap,0,4 + sys->_num_regs_per_rf*4), 0, PrecisionT::INT16); //dst
                requests.push_back(*request);

                //add
                request = new Request(Request::Type::RowAdd);
                request->addAddr(sys->getAddress(tile+gap,0,4 + resultColIndex*4), 0, PrecisionT::INT16); //src
                request->addAddr(sys->getAddress(tile+gap,0,4 + sys->_num_regs_per_rf*4), 0, PrecisionT::INT16); //src2
                request->addAddr(sys->getAddress(tile+gap,0,4 + resultColIndex*4), 0, PrecisionT::INT16); //dst
                requests.push_back(*request);
            }

        }
        start_tile += gap;
        round++;
    }  
    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}



static __attribute__((unused)) Registry::Entry &__gemm__ = pimsim::registerFunc("gemm", gemm);