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


int32_t gemm_vectorized_delay_reduce(System* sys, std::string param_file)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    PrecisionT::Precision precision_input = PrecisionT::INT4;
    PrecisionT::Precision precision_multiply = PrecisionT::INT8;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;


    //Parameters:
    int M = 61440;
    int K = 2048;
    int N = 32;

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
        else if(name == "N"){
            N = value;
        }
    }

    std::cout<<"M: "<<M<<std::endl;
    std::cout<<"K: "<<K<<std::endl;
    std::cout<<"N: "<<N<<std::endl;

    //hdw parameters
    int numColPerArray = cfg->_ncols;
    int numArrayPerTile = cfg->_nblocks;
    int numTile = cfg->_ntiles_used;
    int meshRow = cfg->_meshHeight;
    int meshCol = cfg->_meshWidth;


    //partition parameters
    int H_Cp = ceil(M/(float)meshCol);
    int W_Cp = ceil(N/(float)meshRow);

    std::cout<<"H_Cp: "<<H_Cp<<std::endl;
    std::cout<<"W_Cp: "<<W_Cp<<std::endl;

    //print loop info
    std::cout<<"loop info:"<<std::endl;
    std::cout<<"i_: "<<ceil(M/(float)H_Cp)<<std::endl;
    std::cout<<"j_:"<<ceil(N/(float)W_Cp)<<std::endl;
    std::cout<<"i__"<<ceil(H_Cp/(float)numColPerArray)<<std::endl;
    std::cout<<"j__:"<<W_Cp<<std::endl;
    std::cout<<"k_:"<<ceil(K/(float)numArrayPerTile)<<std::endl;
    std::cout<<"num tiles involved:"<< ceil(M/(float)H_Cp)*ceil(N/(float)W_Cp)<<std::endl;
    std::cout<<"per tile serial pass:"<<ceil(H_Cp/(float)numColPerArray) * W_Cp * ceil(K/(float)numArrayPerTile)<<std::endl;
    for(int i_=0; i_<ceil(M/(float)H_Cp); i_++){
        for(int j_=0; j_<ceil(N/(float)W_Cp); j_++){
            int tile = i_*ceil(N/(float)W_Cp) + j_;
            for(int i__=0; i__<ceil(H_Cp/(float)numColPerArray); i__++){
                for(int j__=0; j__<W_Cp; j__++){
                    for(int k_=0; k_<ceil(K/(float)numArrayPerTile); k_++){
                        request = new Request(Request::Type::RowMul);
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//src2
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //dst
                        requests.push_back(*request);

                        request = new Request(Request::Type::RowAdd);
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_multiply); //src
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate);//src2
                        request->addOperand(sys->getAddress(tile,0,0), 0, precision_accumulate); //dst
                        requests.push_back(*request);  
                    }
                    int RowReduce_WithinTile_count = log2(numArrayPerTile);
                    request = new Request(Request::Type::RowReduce_WithinTile);
                    request->addOperand(sys->getAddress(tile,0,0), RowReduce_WithinTile_count, precision_accumulate); //src
                    request->addOperand(sys->getAddress(tile,0,0), RowReduce_WithinTile_count, precision_accumulate); //dst
                    requests.push_back(*request); 
                }
            }
        }
    }

    


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}


static __attribute__((unused)) Registry::Entry &__gemm_vectorized_delay_reduce__ = pimsim::registerFunc("gemm_vectorized_delay_reduce", gemm_vectorized_delay_reduce);


