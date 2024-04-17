// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <cmath>
#include <numeric>
#include <sstream>
#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

#include "backend/System.h"
#include "conv2d_stream.h"
/////////////////////////////////////////////////////////////
// Conv2d microbenchmark
/////////////////////////////////////////////////////////////


int32_t conv2d_stream(System* sys, std::string param_dir)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT32;


    //Parameters:
    std::vector<Conv_layer_params> conv_layers;
    
    if(param_dir == ""){
        std::cout<<"need to specify directory of params"<<std::endl;
        return -1;
    }
    else{
        std::cout<<"using parameters from dir: "<<param_dir<<std::endl;
    }
    std::vector<std::string> param_files;
    for (const auto & entry : fs::directory_iterator(param_dir)){
        param_files.push_back(entry.path());
    }
    std::sort(param_files.begin(), param_files.end());
    for (int param_file_idx=0; param_file_idx<param_files.size(); param_file_idx++){
        Conv_layer_params conv_params;
        std::ifstream input( param_files[param_file_idx]);
        for( std::string line; getline( input, line ); )
        {
            std::istringstream iss(line);
            std::string name;
            int value;
            if (!(iss >> name >> value)) { break; } // error
            if(name == "N"){
                conv_params.N = value;
            }
            else if(name == "H"){
                conv_params.H = value;
            }
            else if(name == "W"){
                conv_params.W = value;
            }
            else if(name == "C"){
                conv_params.C = value;
            }
            else if(name == "Stride"){
                conv_params.stride = value;
            }
            else if(name == "R"){
                conv_params.R = value;
            }
            else if(name == "S"){
                conv_params.S = value;
            }
            else if(name == "M"){
                conv_params.M = value;
            }
            else if(name == "E"){
                conv_params.E = value;
            }
            else if(name == "F"){
                conv_params.F = value;
            }
            else{
                std::cout<<"unknown parameter: "<<name<<std::endl;
            }
        }
        conv_layers.push_back(conv_params);
    }
    
    //hdw parameters
    int numColPerArray = cfg->_ncols;
    int numArrayPerTile = cfg->_nblocks;
    int numTile = cfg->_ntiles_used;
    int capacityPerTile = cfg->_nrows * cfg->_ncols * cfg->_nblocks;
    int meshWidth = cfg->_meshWidth;
    int meshHeight = cfg->_meshHeight;

    // int H_I = (H-1)*stride+R;
    // int W_I = (W-1)*stride+S;
    int start_tile = 0;
    
    for(int i=0; i<conv_layers.size(); i++){
        Conv_layer_params conv_params = conv_layers[i];
        print_conv_layer_params(conv_params);
        std::vector<int> tiles;
        for(int tile=start_tile; tile<start_tile+meshWidth; tile++){
            tiles.push_back(tile%numTile);
        }
        std::vector<int> tiles_receive_from;
        if(i>0){
            for(int tile=start_tile-meshWidth; tile<start_tile; tile++){
                tiles_receive_from.push_back(tile%numTile);
            }
        }
        std::vector<int> tiles_send_to;
        if(i<conv_layers.size()-1){
            for(int tile=start_tile+meshWidth; tile<start_tile+2*meshWidth; tile++){
                tiles_send_to.push_back(tile%numTile);
            }
        }
        std::cout<<"mapping to tiles: "<<std::endl;
        for(int tile : tiles){
            std::cout<<tile<<" ";
        }
        std::cout<<std::endl;
        conv2d_stream_load_weight(conv_params, precision_input, precision_multiply, precision_accumulate, tiles, requests, sys);
        conv2d_stream_compute(conv_params, precision_input, precision_multiply, precision_accumulate, tiles_receive_from, tiles, tiles_send_to, requests, sys);
        start_tile+=meshWidth;
    }
    
    


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}

/////////////////////////////////////////////////////////////
// Simple program to perform an FIR filter
/////////////////////////////////////////////////////////////


static __attribute__((unused)) Registry::Entry &__conv2d_stream__ = pimsim::registerFunc("conv2d_stream", conv2d_stream);

