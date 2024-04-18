// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <cmath>
#include <numeric>
#include <sstream>

#include "backend/System.h"
#include "conv2d_low_latency.h"
/////////////////////////////////////////////////////////////
// Conv2d microbenchmark
/////////////////////////////////////////////////////////////


int32_t conv2d_lowLatency_onlyLoadWeight(System* sys, std::string param_file)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT32;

    Conv_layer_params conv_layer_params;
    //Parameters:
    conv_layer_params.N = 2;
    conv_layer_params.H = 32;
    conv_layer_params.W = 32;
    conv_layer_params.C = 256;
    conv_layer_params.stride = 1; //not used below; so if you want a different stride, change the code below first.
    conv_layer_params.R = 3; // take 3*3 rows to hold filter
    conv_layer_params.S = 3; 
    conv_layer_params.M = 256; //across cols
    conv_layer_params.E = 30;
    conv_layer_params.F = 30;

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
            conv_layer_params.N = value;
        }
        else if(name == "H"){
            conv_layer_params.H = value;
        }
        else if(name == "W"){
            conv_layer_params.W = value;
        }
        else if(name == "C"){
            conv_layer_params.C = value;
        }
        else if(name == "Stride"){
            conv_layer_params.stride = value;
        }
        else if(name == "R"){
            conv_layer_params.R = value;
        }
        else if(name == "S"){
            conv_layer_params.S = value;
        }
        else if(name == "M"){
            conv_layer_params.M = value;
        }
        else if(name == "E"){
            conv_layer_params.E = value;
        }
        else if(name == "F"){
            conv_layer_params.F = value;
        }
        else{
            std::cout<<"unknown parameter: "<<name<<std::endl;
        }
    }
    

    // int H_I = (H-1)*stride+R;
    // int W_I = (W-1)*stride+S;

    int N = conv_layer_params.N;
    int H = conv_layer_params.H;
    int W = conv_layer_params.W;
    int C = conv_layer_params.C;
    int Stride = conv_layer_params.stride;
    int R = conv_layer_params.R;
    int S = conv_layer_params.S;
    int M = conv_layer_params.M;
    int E = conv_layer_params.E;
    int F = conv_layer_params.F;


    sys->app_param_file<<"\noriginal conv parameters:"<<std::endl;
    sys->app_param_file<<"N: "<<N<<std::endl;
    sys->app_param_file<<"H: "<<H<<std::endl;
    sys->app_param_file<<"W: "<<W<<std::endl;
    sys->app_param_file<<"C: "<<C<<std::endl;
    sys->app_param_file<<"Stride: "<<Stride<<std::endl;
    sys->app_param_file<<"R: "<<R<<std::endl;
    sys->app_param_file<<"S: "<<S<<std::endl;
    sys->app_param_file<<"M: "<<M<<std::endl;
    sys->app_param_file<<"E: "<<E<<std::endl;
    sys->app_param_file<<"F: "<<F<<std::endl;   
    // sys->app_param_file<<"H_I: "<<H_I<<std::endl;
    // sys->app_param_file<<"W_I: "<<W_I<<std::endl;

    
    //hdw parameters
    int numColPerArray = cfg->_ncols;
    int numArrayPerTile = cfg->_nblocks;
    int numTile = cfg->_ntiles_used;
    int capacityPerTile = cfg->_nrows * cfg->_ncols * cfg->_nblocks;
    int meshWidth = cfg->_meshWidth;
    int meshHeight = cfg->_meshHeight;

    //partition parameters
    

    
    //Load weights
    std::cout<<"Load weights"<<std::endl;
    conv2d_low_latency_load_weight(conv_layer_params, precision_input, precision_multiply, precision_accumulate, requests, sys);
    

    // sys->print_data_hit_rate();
    // sys->print_req_hit_rate();
    


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}

/////////////////////////////////////////////////////////////
// Simple program to perform an FIR filter
/////////////////////////////////////////////////////////////


static __attribute__((unused)) Registry::Entry &__conv2d_lowLatency_onlyLoadWeight__ = pimsim::registerFunc("conv2d_lowLatency_onlyLoadWeight", conv2d_lowLatency_onlyLoadWeight);

