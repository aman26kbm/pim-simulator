// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <numeric>

#include "backend/System.h"

int32_t gemm_systolic(System* sys){
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    //int matrixARowNum = 128*256;//128*256
    //int matrixAColNum = 256*8;//256*8
    //int matrixBRowNum = matrixAColNum;//256*8
    //int matrixBColNum = cfg->_num_regs_per_rf;//32

    int matrixARowNum = 120*512;
    int matrixAColNum = 256*8;
    int matrixBRowNum = matrixAColNum;
    int matrixBColNum = 32;

    int use_tiles = cfg->_ntiles_used;
    PrecisionT::Precision precision_input = PrecisionT::INT4;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;

    //broadcast matrixB to each core
    // for(int i=0; i<cfg->_meshHeight*cfg->_meshWidth; i+=cfg->_meshWidth){
    //     for(int j=0; j<8; j++){
    //         request = new Request(Request::Type::RowLoad);
    //         request->addOperand(sys->getAddress(i,0,32+j*precision_input.bits()),256, precision_input); //cram addr
    //         request->addOperand(sys->DRAM_ADDR, 0, precision_input); //dram addr
    //         requests.push_back(*request);

    //         std::vector<int> v(sys->_config->_meshWidth);
    //         std::iota (std::begin(v), std::end(v), i); // Fill with 0, 1, ...
    //         // for(int i: v){
    //         //     std::cout<<i<<" ";
    //         // }
    //         // std::cout<<std::endl;
    //         sys->broadcast_p2p(sys->getAddress(i,0,32+j*precision_input.bits()), precision_input, v, 256);
    //     }
    // }
    
    for(int meshCol=0; meshCol<cfg->_meshWidth; meshCol++){
        for(int meshRow=0; meshRow<cfg->_meshHeight; meshRow++){
            for(int iter=0; iter<10*512/cfg->_ncols; iter++){
                //get data
                if(meshRow==0){
                    for(int i=0; i<8; i++){
                        request = new Request(Request::Type::RowLoad);
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,i*precision_input.bits()),cfg->_nrows*cfg->_ncols, precision_input); //cram addr
                        request->addOperand(sys->DRAM_ADDR,cfg->_nrows*cfg->_ncols, precision_input); //cram addr
                        requests.push_back(*request);
                    }
                }
                else{
                    for(int i=0; i<8; i++){
                        request = new Request(Request::Type::TileReceive);
                        request->addOperand(sys->getAddress((meshRow-1)*cfg->_meshWidth + meshCol,0,i*precision_input.bits()),cfg->_nrows*cfg->_ncols, precision_input); //cram addr
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,i*precision_input.bits()),cfg->_nrows*cfg->_ncols, precision_input); //cram addr
                        requests.push_back(*request);
                    }
                }
                //send to neighbor
                if(meshRow!=cfg->_meshHeight-1){
                    for(int i=0; i<8; i++){
                        request = new Request(Request::Type::TileSend);
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,i*precision_input.bits()),cfg->_nrows*cfg->_ncols, precision_input); //cram addr
                        request->addOperand(sys->getAddress((meshRow+1)*cfg->_meshWidth + meshCol,0,i*precision_input.bits()),cfg->_nrows*cfg->_ncols, precision_input); //cram addr
                        requests.push_back(*request);
                    }
                }

                //compute
                for(int j=0; j<4; j++){
                    for(int i=0; i<8; i++){
                        request = new Request(Request::Type::RowMul);
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,i*precision_input.bits()), 0, precision_input); //src
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,8*precision_input.bits()+i*precision_input.bits()), 0, precision_input); //src
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,8*precision_input.bits()+8*precision_input.bits()), 0, precision_multiply); //dst
                        requests.push_back(*request);  

                        request = new Request(Request::Type::RowAdd);
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,8*precision_input.bits()+i*precision_input.bits()), 0, precision_multiply); //src
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,8*precision_input.bits()+8*precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //src
                        request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,8*precision_input.bits()+8*precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //dst
                        requests.push_back(*request);  
                    }
                }
                request = new Request(Request::Type::RowReduce_WithinTile);
                request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,8*precision_input.bits()+8*precision_input.bits()+precision_multiply.bits()), (int)log2(cfg->_nblocks), precision_accumulate); //src
                request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,8*precision_input.bits()+8*precision_input.bits()+precision_multiply.bits()), (int)log2(cfg->_nblocks), precision_accumulate); //dst

                request = new Request(Request::Type::RowStore);
                request->addOperand(sys->getAddress((meshRow)*cfg->_meshWidth + meshCol,0,8*precision_input.bits()+8*precision_input.bits()+precision_multiply.bits()),cfg->_ncols, precision_accumulate); //cram addr
                request->addOperand(sys->DRAM_ADDR, 0, precision_accumulate); //dram addr
                requests.push_back(*request);
            }
        }
     
    }
    // for(int iter=0; iter<matrixARowNum/use_tiles+1; iter++){
    //     for(int core=0; core<use_tiles; core++){
    //         int matArow = iter*use_tiles+core;
    //         if(matArow<matrixARowNum){
    //             request = new Request(Request::Type::RowLoad);
    //             request->addOperand(sys->getAddress(core,0,precision_input.bits()),matrixAColNum, precision_input); //cram addr
    //             request->addOperand(sys->DRAM_ADDR, 0, precision_input); //dram addr
    //             requests.push_back(*request);

    //             request = new Request(Request::Type::RowMul);
    //                 request->addOperand(sys->getAddress(core,0,0), 0, precision_input); //src
    //                 request->addOperand(sys->getAddress(core,0,precision_input.bits()), 0, precision_input);//rf
    //                 request->addOperand(sys->getAddress(core,0,2*precision_input.bits()), 0, precision_multiply); //dst
    //                 requests.push_back(*request);    
                
    //             request = new Request(Request::Type::RowReduce_WithinTile);
    //                         request->addOperand(sys->getAddress(core,0,2*precision_input.bits()), (int)log2(matrixAColNum/cfg->_ncols), precision_multiply); //src
    //                         request->addOperand(sys->getAddress(core,0,2*precision_input.bits()), (int)log2(matrixAColNum/cfg->_ncols), precision_accumulate); //dst
    //                         requests.push_back(*request);
    //             request = new Request(Request::Type::RowReduce);
    //                         request->addOperand(sys->getAddress(core,0,2*precision_input.bits()), (int)log2(cfg->_ncols)-cfg->_popcount_pipeline_stages, precision_accumulate); //src
    //                         request->addOperand(sys->getAddress(core,0,2*precision_input.bits()), (int)log2(cfg->_ncols)-cfg->_popcount_pipeline_stages, precision_accumulate); //dst
    //                         requests.push_back(*request);
    //             request = new Request(Request::Type::PopCountReduce_RF);
    //                         request->addOperand(sys->getAddress(core,0,2*precision_input.bits()), 0, precision_accumulate); //src
    //                         request->addOperand(sys->getAddress(core,0,2*precision_input.bits()), 0, precision_accumulate); //dst
    //                         requests.push_back(*request);
                
    //                 request = new Request(Request::Type::RowStore);
    //                             request->addOperand(sys->getAddress(core,0,2*precision_input.bits()),32, precision_accumulate); //cram addr
    //                             request->addOperand(sys->DRAM_ADDR, 0, precision_accumulate); //dram addr
    //                             requests.push_back(*request);
                
    //         }
    //     }
    // }
    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}



static __attribute__((unused)) Registry::Entry &__gemm_systolic__ = pimsim::registerFunc("gemm_systolic", gemm_systolic);