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


int32_t gemv_vectorized_const_op(System* sys, std::string param_file)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT32;


    //Parameters:
    int M = 65536;
    int K = 2048;

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
        if(name == "M"){
            M = value;
        }
        else if(name == "K"){
            K = value;
        }
    }

    std::cout<<"M: "<<M<<std::endl;
    std::cout<<"K: "<<K<<std::endl;

    //hdw parameters
    int numColPerArray = cfg->_ncols;
    int numArrayPerTile = cfg->_nblocks;
    int numTile = cfg->_ntiles_used;


    //partition parameters
    int K_p = ceil(K/(float)numTile);

    std::cout<<"K_p: "<<K_p<<std::endl;

    //print loop info
    std::cout<<"loop info:"<<std::endl;
    
    std::cout<<"j_:"<<ceil(K/(float)K_p)<<std::endl;
    std::cout<<"j__:"<<K_p<<std::endl;
    std::cout<<"i_: "<<ceil(M/(float)(numColPerArray * numArrayPerTile))<<std::endl;
    std::cout<<"num tiles involved:"<< ceil(K/(float)K_p)<<std::endl;
    std::cout<<"per tile serial pass:"<<ceil(M/(float)(numColPerArray * numArrayPerTile)) * K_p<<std::endl;
    // \State $K_p = K/\#tile$
    // \For {$j'$ in $[0,K/K_p)$}
    //     \For {$j''$ in $[0,K_p)$}
    //         \State $j = j'*K_p+j''$
    //         \For{$i'$ in $[0,M/(\#col*\#arr))$}
    //             \State $\Vec{i''}=[0,\#col*\#arr)$
    //             \State $\Vec{i}=i'+\Vec{i''}$
    //             \State $\vec{C}[\vec{i}] += \vec{A}[\vec{i},j]*B[j]$
    //         \EndFor
    //     \EndFor
    // \EndFor
    
    for(int j_=0; j_<ceil(K/(float)K_p); j_++){
        for(int j__=0; j__<K_p; j__++){
            int tile = j_;
            int j = j_*K_p+j__;
            for(int i_=0; i_<ceil(M/(float)(numColPerArray * numArrayPerTile)); i_++){
                
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
                
            }
        }
    }

    


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}


static __attribute__((unused)) Registry::Entry &__gemv_vectorized_const_op__ = pimsim::registerFunc("gemv_vectorized_const_op", gemv_vectorized_const_op);


