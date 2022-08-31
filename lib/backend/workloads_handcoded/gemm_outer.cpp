// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t gemm_outer(System* sys){
    std::vector<Request> requests;
    Request *request;

    int matrixARowNum = 128*256;
    int matrixAColNum = 256*8;
    int matrixBRowNum = matrixAColNum;
    int matrixBColNum = sys->_num_regs_per_rf;

    int use_tiles = 128;
    int dram_tile = 64;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT32;

    int basicMatrixARowNum = sys->_nblocks*sys->_ncols;
    int basicMatrixAColNum = matrixAColNum;
    int basicMatrixBRowNum = matrixAColNum;
    int basicMatrixBColNum = min(sys->_num_regs_per_rf, (sys->_nrows-precision_input.bits()-precision_accumulate.bits())/precision_accumulate.bits());//6
    
    int partial_result_matrix_start_row = precision_input.bits();
    int accumulate_row = precision_input.bits()+basicMatrixBColNum*precision_accumulate.bits();
    for(int i=0; i<(int)ceil(matrixARowNum/(float)basicMatrixARowNum); i++){//1
        for(int j=0; j<(int)ceil(matrixBColNum/(float)basicMatrixBColNum); j++){//6
            for(int iter_tile=dram_tile; iter_tile<dram_tile+use_tiles; iter_tile++){//1
                int tile = iter_tile % sys->_ntiles;
                for(int basicA_col_idx=0; basicA_col_idx<matrixAColNum; basicA_col_idx+=use_tiles){//striped basicA columns X basicB rows //256*8
                    request = new Request(Request::Type::RowLoad);
                    request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //cram addr
                    request->addOperand(sys->DRAM_ADDR, 0, precision_input); //dram addr
                    requests.push_back(*request);

                    request = new Request(Request::Type::RowLoad_RF);
                    request->addOperand(sys->_num_regs_per_rf * tile, basicMatrixBRowNum, precision_input); //RF addr
                    request->addOperand(sys->DRAM_ADDR, 0, precision_input); //dram addr
                    requests.push_back(*request);
                    //multiply add to get partial sum matrix in 1 core
                    for(int regIndex=0; regIndex<basicMatrixBColNum; regIndex++){
                        
                        request = new Request(Request::Type::RowMul_CRAM_RF);
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        request->addOperand(sys->_num_regs_per_rf * tile + regIndex, 4, precision_input);//rf
                        request->addOperand(sys->getAddress(tile,0,accumulate_row), 0, precision_multiply); //dst
                        requests.push_back(*request);    

                        request = new Request(Request::Type::RowAdd);
                        request->addOperand(sys->getAddress(tile,0,accumulate_row),0,precision_multiply);//src
                        request->addOperand(sys->getAddress(tile,0,precision_input.bits()+regIndex*precision_accumulate.bits()), 0, precision_accumulate); //src
                        request->addOperand(sys->getAddress(tile,0,precision_input.bits()+regIndex*precision_accumulate.bits()), 0, precision_accumulate); //dst
                        requests.push_back(*request);

                    }  
                }
            }
            for(int gap =1; gap<use_tiles; gap = gap*2){
                for(int tile=dram_tile; tile+gap<dram_tile+use_tiles; tile=tile+2*gap){
                    //Send partial results to tile that is gap away
                    for(int resultColIndex=0; resultColIndex<basicMatrixBColNum; resultColIndex++){
                        request = new Request(Request::Type::TileSend);
                        request->addOperand(sys->getAddress((tile+gap)%sys->_ntiles,0,precision_input.bits()+resultColIndex*precision_accumulate.bits()), 0, precision_multiply); //src
                        request->addOperand(sys->getAddress(tile%sys->_ntiles,0, precision_input.bits()+basicMatrixBColNum*precision_accumulate.bits()), 0, precision_multiply); //dst
                        requests.push_back(*request);

                        request = new Request(Request::Type::TileReceive);
                        request->addOperand(sys->getAddress((tile+gap)%sys->_ntiles,0,precision_input.bits()+resultColIndex*precision_accumulate.bits()), 0, precision_multiply); //src
                        request->addOperand(sys->getAddress(tile%sys->_ntiles,0,precision_input.bits()+basicMatrixBColNum*precision_accumulate.bits()), 0, precision_multiply); //dst
                        requests.push_back(*request);

                        //add
                        request = new Request(Request::Type::RowAdd);
                        request->addOperand(sys->getAddress(tile%sys->_ntiles,0,precision_input.bits()+resultColIndex*precision_accumulate.bits()), 0, precision_multiply); //src
                        request->addOperand(sys->getAddress(tile%sys->_ntiles,0,precision_input.bits()+basicMatrixBColNum*precision_accumulate.bits()), 0, precision_multiply); //src2
                        request->addOperand(sys->getAddress(tile%sys->_ntiles,0,precision_input.bits()+resultColIndex*precision_accumulate.bits()), 0, precision_accumulate); //dst
                        requests.push_back(*request);
                    }
                }
            }  
            for(int resultColIndex=0; resultColIndex<basicMatrixBColNum; resultColIndex++){//6
                request = new Request(Request::Type::RowStore);
                request->addOperand(sys->getAddress(dram_tile,0,precision_input.bits()+resultColIndex*precision_accumulate.bits()), 0, precision_accumulate); //cram addr
                request->addOperand(sys->DRAM_ADDR, 0, precision_accumulate); //dram addr
                requests.push_back(*request);
            }
        }
        
    }
    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}



static __attribute__((unused)) Registry::Entry &__gemm_outer__ = pimsim::registerFunc("gemm_outer", gemm_outer);