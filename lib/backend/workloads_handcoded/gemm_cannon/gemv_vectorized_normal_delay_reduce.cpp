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


int32_t gemv_vectorized_normal_delay_reduce(System* sys, std::string param_file)
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
    int M_p = ceil(M/(float)numTile);

    std::cout<<"M_p: "<<M_p<<std::endl;

    //print loop info
    std::cout<<"loop info:"<<std::endl;
    
    std::cout<<"i_:"<<ceil(M/(float)M_p)<<std::endl;
    std::cout<<"i__:"<<ceil(M_p/(float)numColPerArray)<<std::endl;
    std::cout<<"j_: "<<ceil(K/(float)numArrayPerTile)<<std::endl;
    std::cout<<"num tiles involved:"<< ceil(M/(float)M_p)<<std::endl;
    std::cout<<"max per tile serial pass:"<<ceil(M_p/(float)numColPerArray) * ceil(K/(float)numArrayPerTile)<<std::endl;

    // \State $M_p = M/\#tile$
    // \For {$i'$ in $[0,M/M_p)$}
    //     \For {$i''$ in $[0,M_p/\#col)$}
    //         \State $\vec{i'''}=[0,\#col)$
    //         \For{$j'$ in $[0,K/\#arr)$}
    //             \State $\Vec{j''}=[0,\#arr)$
    //             \State $\vec{i}=i'\times M_p+i''\times\#col+\vec{i'''}$
    //             \State $\vec{j}=j'\times\#arr+\vec{j''}$
    //             \State $C_t[\vec{i},\vec{j''}] += A[\vec{i},\vec{j}]*B[\vec{j}]$
    //         \EndFor
    //         \State $C[\vec{i}]+=C_t[\vec{i},\vec{j''}]$
    //     \EndFor
    // \EndFor
    
    int total_serial_pass = 0;
    for(int i_=0; i_<ceil(M/(float)M_p); i_++){
        int tile = i_;
        for(int i__=0; i__<ceil(M_p/(float)numColPerArray); i__++){
            for(int j_=0; j_<ceil(K/(float)numArrayPerTile); j_++){
                int i_start= i_*M_p+i__*numColPerArray;
                int j_start= j_*numArrayPerTile;
                if(i_start>=(i_+1)*M_p || j_start>=(j_+1)*numArrayPerTile || i_start >= M || j_start >= K)
                    continue;
                request = new Request(Request::Type::RowMul);
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                requests.push_back(*request);

                request = new Request(Request::Type::RowAdd);
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //src
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate);//src2
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                requests.push_back(*request);  
                
                total_serial_pass++;
            }
            int RowReduce_WithinTile_count = log2(numArrayPerTile);
            request = new Request(Request::Type::RowReduce_WithinTile);
            request->addOperand(sys->getAddress(tile,0,0), RowReduce_WithinTile_count, precision_accumulate); //src
            request->addOperand(sys->getAddress(tile,0,0), RowReduce_WithinTile_count, precision_accumulate); //dst
            requests.push_back(*request); 
        }
    }

    std::cout<<"total serial pass:"<<total_serial_pass<<std::endl;


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}


static __attribute__((unused)) Registry::Entry &__gemv_vectorized_normal_delay_reduce__ = pimsim::registerFunc("gemv_vectorized_normal_delay_reduce", gemv_vectorized_normal_delay_reduce);


