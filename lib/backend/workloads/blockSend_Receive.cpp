// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t blockSend_Receive(System* sys){
    std::vector<Request> requests;
    Request *request;
int matrixARowNum = sys->_ntiles;
    int matrixAColNum = sys->_nblocks*sys->_ncols;
    int matrixBRowNum = matrixAColNum;
    int matrixBColNum = sys->_num_regs_per_rf;
    //for(int b_col_index=0; b_col_index<matrixBColNum; b_col_index++){
       
        
        for(int tile=0; tile<sys->_ntiles; tile++){
            
            int start_block = 0;
            int round = 0;
            for(int gap =1; gap<sys->_nblocks; gap = gap*2){
                for(int block=start_block; block+gap<sys->_nblocks; block=block+2*gap){
                    //Send partial results to block that is gap away
                    
                        request = new Request(Request::Type::BlockSend_Receive);
                        request->addAddr(sys->getAddress(tile,block,8), 0, PrecisionT::INT4); //src
                        request->addAddr(sys->getAddress(tile,block+gap,12), 0, PrecisionT::INT16); //dst
                        requests.push_back(*request);

                    

                }
                start_block += gap;
                round++;
            }  

        }
    //}


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}



static __attribute__((unused)) Registry::Entry &__blockSend_Receive__ = pimsim::registerFunc("blockSend_Receive", blockSend_Receive);