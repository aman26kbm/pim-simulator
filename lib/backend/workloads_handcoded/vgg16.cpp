// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <cmath>

#include "backend/System.h"
/////////////////////////////////////////////////////////////
// FIR filter micro benchmark
/////////////////////////////////////////////////////////////



//tile 0
void conv1_1(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;
    int tile=0;
    int nextTile=1;
    int H=224;
    int W=224;
    int R=3;
    int S=3;
    int C=3;
    int M=64;
    int pad=1;
    int E=224;
    int F=224;

    request = new Request(Request::Type::RowLoad);
    request->addOperand(sys->getAddress(tile,0,0), H*W*C, precision_input); //dst
    request->addOperand(sys->DRAM_ADDR, 0, precision_input); //src
    requests.push_back(*request);
    for(int i=-pad; i<H+pad; i++){
        for(int j=-pad; j<=W+pad; j++){
            
            if(0<=i && i<H && 0<=j && j<W){
                //load 1 pixel
                // request = new Request(Request::Type::RowLoad);
                // request->addOperand(sys->getAddress(tile,0,0), C, precision_input); //dst
                // request->addOperand(sys->DRAM_ADDR, 0, precision_input); //src
                // requests.push_back(*request);

                for(int p=i-R+1; p<=i; p++){
                    for(int q=j-S+1; q<=j; q++){
                        if(p>=-pad && p<=H+pad-R && q>=-pad && q<=W+pad-S){
                            request = new Request(Request::Type::RowMul);
                            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                            request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                            request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                            requests.push_back(*request);

                            request = new Request(Request::Type::RowReduce);
                            request->addOperand(sys->getAddress(tile,0,0), (int)log2(cfg->_ncols)-cfg->_popcount_pipeline_stages, precision_accumulate); //src
                            request->addOperand(sys->getAddress(tile,0,0), (int)log2(cfg->_ncols)-cfg->_popcount_pipeline_stages, precision_accumulate); //dst
                            requests.push_back(*request);
                            request = new Request(Request::Type::PopCountReduce_RF);
                            request->addOperand(sys->getAddress(tile,0,2*precision_input.bits()), 0, precision_accumulate); //src
                            request->addOperand(sys->getAddress(tile,0,2*precision_input.bits()), 0, precision_accumulate); //dst
                            requests.push_back(*request);

                            request = new Request(Request::Type::RowAdd);
                            request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                            request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                            request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                            requests.push_back(*request);
                        }
                    }
                }
            }
            // int out_i = i-R+1+pad;
            // int out_j = j-S+pad;
            // if(out_i>=0 && out_i<E && out_j>=0 && out_j<F){
            //     request = new Request(Request::Type::TileSend);
            //     request->addOperand(sys->getAddress(tile,0,0), M, precision_input);//src
            //     request->addOperand(sys->getAddress(nextTile,0,0), M, precision_input);//DST
            //     requests.push_back(*request);
            // }

        }
    }
    for (unsigned int i = 0; i < requests.size(); i++)
            sys->sendRequest(requests[i]);
}

void conv1_2(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;
    int tile=1;
    int fromTile=0;
    int nextTile=2;
    int H=224;
    int W=224;
    int R=3;
    int S=3;
    int C=64;
    int M=64;
    int pad=1;
    int E=224;
    int F=224;
    for(int i=-pad; i<H+pad; i++){
        for(int j=-pad; j<=W+pad; j++){
            
            if(0<=i && i<H && 0<=j && j<W){
                //load 1 pixel
                request = new Request(Request::Type::TileReceive);
                request->addOperand(sys->getAddress(fromTile,0,0), M, precision_input);//src
                request->addOperand(sys->getAddress(tile,0,0), M, precision_input);//DST
                requests.push_back(*request);

                // for(int p=i-R+1; p<=i; p++){
                //     for(int q=j-S+1; q<=j; q++){
                //         if(p>=-pad && p<=H+pad-R && q>=-pad && q<=W+pad-S){
                //             request = new Request(Request::Type::RowMul);
                //             request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                //             request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                //             request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                //             requests.push_back(*request);

                //             request = new Request(Request::Type::RowReduce);
                //             request->addOperand(sys->getAddress(tile,0,0), (int)log2(cfg->_ncols)-cfg->_popcount_pipeline_stages, precision_accumulate); //src
                //             request->addOperand(sys->getAddress(tile,0,0), (int)log2(cfg->_ncols)-cfg->_popcount_pipeline_stages, precision_accumulate); //dst
                //             requests.push_back(*request);
                //             request = new Request(Request::Type::PopCountReduce_RF);
                //             request->addOperand(sys->getAddress(tile,0,2*precision_input.bits()), 0, precision_accumulate); //src
                //             request->addOperand(sys->getAddress(tile,0,2*precision_input.bits()), 0, precision_accumulate); //dst
                //             requests.push_back(*request);

                //             request = new Request(Request::Type::RowAdd);
                //             request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                //             request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //src
                //             request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                //             requests.push_back(*request);
                //         }
                //     }
                // }
            }
            // int out_i = i-R+1+pad;
            // int out_j = j-S+pad;
            // if(out_i>=0 && out_i<E && out_j>=0 && out_j<F){
            //     request = new Request(Request::Type::TileSend);
            //     request->addOperand(sys->getAddress(tile,0,0), M, precision_input);//src
            //     request->addOperand(sys->getAddress(nextTile,0,0), M, precision_input);//DST
            //     requests.push_back(*request);
            // }

        }
    }
    for (unsigned int i = 0; i < requests.size(); i++)
            sys->sendRequest(requests[i]);
}

int32_t vgg16(System* sys)
{

    conv1_1(sys);
    //conv1_2(sys);
    
    
}
static __attribute__((unused)) Registry::Entry &__vgg16__ = pimsim::registerFunc("vgg16", vgg16);


