// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <cmath>
#include <numeric>
#include <sstream>

#include "backend/System.h"
/////////////////////////////////////////////////////////////
// GEMM microbenchmark
/////////////////////////////////////////////////////////////


int32_t fir_vectorized_const_op(System* sys, std::string param_file)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT32;


    //Parameters:
    int N = 120*256*256*10;
    int K = 32;

    if(param_file == ""){
        std::cout<<"using default parameters"<<std::endl;
    }
    else{
        std::cout<<"using parameters from file: "<<param_file<<std::endl;
    }
    std::ifstream input( param_file);
    for( std::string line; getline( input, line ); )
    {
        std::istringstream iss(line);
        std::string name;
        int value;
        if (!(iss >> name >> value)) { break; } // error
        if(name == "N"){
            N = value;
        }
        else if(name == "K"){
            K = value;
        }
    }

    std::cout<<"N: "<<N<<std::endl;
    std::cout<<"K: "<<K<<std::endl;

    //hdw parameters
    int numColPerArray = cfg->_ncols;
    int numArrayPerTile = cfg->_nblocks;
    int numTile = cfg->_ntiles_used;


    //partition parameters
    int N_p = ceil(N/(float)numTile);

    std::cout<<"N_p: "<<N_p<<std::endl;

    //print loop info
    std::cout<<"loop info:"<<std::endl;
    
    std::cout<<"i_: "<<ceil(N/(float)N_p)<<std::endl;
    std::cout<<"i__:"<<ceil(N_p/(float)(numColPerArray*numArrayPerTile-K))<<std::endl;
    std::cout<<"j:"<<K<<std::endl;
    std::cout<<"num tiles involved:"<< ceil(N/(float)N_p)<<std::endl;
    std::cout<<"per tile serial pass:"<<ceil(N_p/(float)(numColPerArray*numArrayPerTile-K)) * K<<std::endl;
    // \For {$i'$ in $N/N_p$}
    //     \For {$i''$ in $[0,N_p/(\#col\times\#arr - K)))$}
    //         \For{$j$ in $[0,K)$}
                
    //             \State $\Vec{i'''} = [0,\#col\times\#arr - K)$
    //             \State $\vec{i}=i'\times N_p + i''\times (\#col\times\#arr - K) + \vec{i'''} $
    //             \State $B[i]+=A[i+j]*F[j]$

    //         \EndFor
    //     \EndFor
    // \EndFor
    
    for(int i_=0; i_<ceil(N/(float)N_p); i_++){
        int tile = i_;
        for(int i__=0; i__<ceil(N_p/(float)(numColPerArray*numArrayPerTile-K)); i__++){
            for(int j=0; j<K; j++){
                
                request = new Request(Request::Type::RowMul_CRAM_RF);
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                request->addOperand(cfg->_num_regs_per_rf * tile, 0,precision_input);//rf
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                requests.push_back(*request);

                request = new Request(Request::Type::RowAdd);
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //src
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate);//src2
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                requests.push_back(*request);  

                request = new Request(Request::Type::RowShift);
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //dst
                requests.push_back(*request);
                
            }
        }
    }

    


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}


static __attribute__((unused)) Registry::Entry &__fir_vectorized_const_op__ = pimsim::registerFunc("fir_vectorized_const_op", fir_vectorized_const_op);


