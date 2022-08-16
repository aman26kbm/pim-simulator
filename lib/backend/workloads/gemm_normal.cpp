// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t gemm_normal(System* sys){
    std::vector<Request> requests;
    Request *request;

    int matrixARowNum = sys->_ntiles;
    int matrixAColNum = sys->_nblocks*sys->_ncols;
    int matrixBRowNum = matrixAColNum;
    int matrixBColNum = sys->_num_regs_per_rf;
    for(int b_col_index=0; b_col_index<matrixBColNum; b_col_index++){
        //b[:0]
        request = new Request(Request::Type::RowLoad);
        request->addAddr(sys->getAddress(0,0,4), 0, PrecisionT::INT4); //cram addr
        request->addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT4); //dram addr
        requests.push_back(*request);
        //broadcast
        
        for(int tile=0; tile<sys->_ntiles; tile++){
            //a[0:]
            request = new Request(Request::Type::RowLoad);
            request->addAddr(sys->getAddress(tile,0,0), 0, PrecisionT::INT4); //cram addr
            request->addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT4); //dram addr
            requests.push_back(*request);
            
            //multiply
            request = new Request(Request::Type::RowMul);
            request->addAddr(sys->getAddress(tile,0,0), 0, PrecisionT::INT4); //src1
            request->addAddr(sys->getAddress(tile,0,4), 0, PrecisionT::INT4);//src2
            request->addAddr(sys->getAddress(tile,0,8), 0, PrecisionT::INT4); //dst
            requests.push_back(*request);
            //reduce to get c0_0
            request = new Request(Request::Type::RowReduce_WithinTile);
            request->addAddr(sys->getAddress(tile,0,8), 7, PrecisionT::INT4); //src
            request->addAddr(sys->getAddress(tile,0,12), 7, PrecisionT::INT4); //dst
            requests.push_back(*request);
            request = new Request(Request::Type::RowReduce);
            request->addAddr(sys->getAddress(tile,0,12), 2, PrecisionT::INT4); //src
            request->addAddr(sys->getAddress(tile,0,12), 2, PrecisionT::INT4); //dst
            requests.push_back(*request);
            request = new Request(Request::Type::PopCountReduce_RF);
            request->addAddr(sys->getAddress(tile,0,12), 0, PrecisionT::INT4); //src
            request->addAddr(sys->_num_regs_per_rf * tile, 0, PrecisionT::INT4); //dst
            requests.push_back(*request);
            // int start_block = 0;
            // int round = 0;
            // for(int gap =1; gap<sys->_nblocks; gap = gap*2){
            //     for(int block=start_block; block+gap<sys->_nblocks; block=block+2*gap){
            //         //Send partial results to block that is gap away
                    
            //             request = new Request(Request::Type::BlockSend_Receive);
            //             request->addAddr(sys->getAddress(tile,block,8), 0, PrecisionT::INT4); //src
            //             request->addAddr(sys->getAddress(tile,block+gap,12), 0, PrecisionT::INT16); //dst
            //             requests.push_back(*request);

            //             //add
            //             request = new Request(Request::Type::RowAdd);
            //             request->addAddr(sys->getAddress(tile,block+gap,8), 0, PrecisionT::INT16); //src
            //             request->addAddr(sys->getAddress(tile,block+gap,12), 0, PrecisionT::INT16); //src2
            //             request->addAddr(sys->getAddress(tile,block+gap,8), 0, PrecisionT::INT16); //dst
            //             requests.push_back(*request);
                    

            //     }
            //     start_block += gap;
            //     round++;
            // }  

        }
    }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}



static __attribute__((unused)) Registry::Entry &__gemm_normal__ = pimsim::registerFunc("gemm_normal", gemm_normal);