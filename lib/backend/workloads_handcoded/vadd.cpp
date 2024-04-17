// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <cmath>
#include <numeric>
#include <sstream>

#include "backend/System.h"
/////////////////////////////////////////////////////////////
// Conv2d microbenchmark
/////////////////////////////////////////////////////////////


int32_t vadd(System* sys, std::string param_file)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT32;


    //Parameters:
    int H = 56;
    int W = 56;
    int C = 64;
    int N = 256;
    int L = H*W*C*N;


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
        if(name == "L"){
            L = value;
        }
        else if(name == "H"){
            H = value;
        }
        else if(name == "W"){
            W = value;
        }
        else if(name == "C"){
            C = value;
        }
        else if(name == "N"){
            N = value;
        }
        else{
            std::cout<<"unknown parameter: "<<name<<std::endl;
        }
    }
    L = H*W*C*N;
    sys->app_param_file<<"\noriginal vadd parameters:"<<std::endl;
    sys->app_param_file<<"H: "<<H<<std::endl;
    sys->app_param_file<<"W: "<<W<<std::endl;
    sys->app_param_file<<"C: "<<C<<std::endl;
    sys->app_param_file<<"N: "<<N<<std::endl;
    sys->app_param_file<<"L: "<<L<<std::endl;

    
    //hdw parameters
    int numColPerArray = cfg->_ncols;
    int numArrayPerTile = cfg->_nblocks;
    int numTile = cfg->_ntiles_used;
    int capacityPerTile = cfg->_nrows * cfg->_ncols * cfg->_nblocks;
    int meshWidth = cfg->_meshWidth;
    int meshHeight = cfg->_meshHeight;

    
    //padding 0s if there is not enough parallelism
    int least_length = numColPerArray * numArrayPerTile;
    L = ceil(L/(float)least_length) * least_length;
   

    sys->app_param_file<<"\npadded conv parameters"<<std::endl;
    sys->app_param_file<<"L: "<<L<<std::endl;

    //partition parameters
    // int L_p = ceil(L/(float)meshHeight);
    // int W_Yp = ceil(F/(float)meshWidth);
    // sys->app_param_file<<"\npartition parameters"<<std::endl;
    // sys->app_param_file<<"H_Yp: "<<H_Yp<<std::endl;
    // sys->app_param_file<<"W_Yp: "<<W_Yp<<std::endl;
    // int M_p = M;
    // sys->app_param_file<<"M_p: "<<M_p<<std::endl;
    // int N_p = N;
    // sys->app_param_file<<"N_p: "<<N_p<<std::endl;

    PrecisionT::Precision precision_temp = PrecisionT::INT8;

    //print loop info
    // sys->app_param_file<<"\nloop info:"<<std::endl;
    
    // sys->app_param_file<<"n_:"<<ceil(N/(float)N_p)<<std::endl;
    // sys->app_param_file<<"n__"<<N_p<<std::endl;
    // sys->app_param_file<<"m_:"<<ceil(M/(float)numColPerArray)<<std::endl;
    // sys->app_param_file<<"e_:"<<ceil(E/(float)H_Yp)<<std::endl;
    // sys->app_param_file<<"f_:"<<ceil(F/(float)W_Yp)<<std::endl;
    // sys->app_param_file<<"e__:"<<H_Yp<<std::endl;
    // sys->app_param_file<<"f__:"<<W_Yp<<std::endl;
    // sys->app_param_file<<"c_: "<<ceil(C/(float)numArrayPerTile)<<std::endl;
    // sys->app_param_file<<"r: "<<R<<std::endl;
    // sys->app_param_file<<"s: "<<S<<std::endl;
    // sys->app_param_file<<"num tiles involved:"<< ceil(E/(float)H_Yp)*ceil(F/(float)W_Yp)<<std::endl;
    // sys->app_param_file<<"per tile serial pass:"<<N * M/numColPerArray * H_Yp * W_Yp * C/numArrayPerTile * R * S<<std::endl;
   
    
    //Load weights
    for(int l_=0; l_<L/least_length; l_++){//mapped to different tiles
        int tile = l_%numTile;
        request = new Request(Request::Type::RowLoad);
        request->addOperand(sys->getAddress(tile,0,0), numColPerArray*numArrayPerTile, precision_input); //cram addr
        request->addOperand(sys->DRAM_ADDR, numColPerArray*numArrayPerTile, precision_input); //dram addr
        request->setShuffle(0,0, 0, 0);
        requests.push_back(*request); 
        request = new Request(Request::Type::RowLoad);
        request->addOperand(sys->getAddress(tile,0,0), numColPerArray*numArrayPerTile, precision_input); //cram addr
        request->addOperand(sys->DRAM_ADDR, numColPerArray*numArrayPerTile, precision_input); //dram addr
        request->setShuffle(0,0, 0, 0);
        requests.push_back(*request); 

        request = new Request(Request::Type::RowAdd);
        request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
        request->addOperand(sys->getAddress(tile,0,0), 0, precision_input);//src_
        request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //dst
        requests.push_back(*request);

        request = new Request(Request::Type::RowStore);
        request->addOperand(sys->getAddress(tile,0,0), numColPerArray*numArrayPerTile, precision_input); //cram addr
        request->addOperand(sys->DRAM_ADDR, numColPerArray*numArrayPerTile, precision_input); //dram addr
        requests.push_back(*request);
    }


       

    sys->print_data_hit_rate();
    sys->print_req_hit_rate();
    


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}

/////////////////////////////////////////////////////////////
// Simple program to perform an FIR filter
/////////////////////////////////////////////////////////////


static __attribute__((unused)) Registry::Entry &__vadd__ = pimsim::registerFunc("vadd", vadd);

