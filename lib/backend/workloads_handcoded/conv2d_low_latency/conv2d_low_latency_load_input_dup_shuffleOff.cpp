#include "conv2d_low_latency.h"

void conv2d_low_latency_load_input_dup_shuffleOff(Conv_layer_params conv_layer_params,
                                PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate,
                                std::vector<Request> &requests, System *sys)
{
    Config *cfg = sys->_config;
    int numColPerArray = cfg->_ncols;
    int numArrayPerTile = cfg->_nblocks;
    int numTile = cfg->_ntiles_used;
    int capacityPerTile = cfg->_nrows * cfg->_ncols * cfg->_nblocks;
    int meshWidth = cfg->_meshWidth;
    int meshHeight = cfg->_meshHeight;

    int N = conv_layer_params.N;
    int M = conv_layer_params.M;
    int E = conv_layer_params.E;
    int F = conv_layer_params.F;
    int C = conv_layer_params.C;
    int R = conv_layer_params.R;
    int S = conv_layer_params.S;
    int Stride = conv_layer_params.stride;

    int M_p = ceil(M / (float)numTile);
    int numTileInvolved = std::min(numTile, (int)ceil(M/(float)M_p));

    
    Request *request;
    
    int H = (E-1)*Stride+R;
    int W = (F-1)*Stride+S;

    int weightDupInArr = 1;
    if (numColPerArray / (float)M_p > 1)
        weightDupInArr = floor(numColPerArray / (float)M_p);

    int weightDupAcrossArr = 1;
    if (numArrayPerTile / (float)C > 1)
        weightDupAcrossArr = floor(numArrayPerTile / (float)C);

    int input_dup_factor = numColPerArray/weightDupInArr;
    std::cout<<"input_dup_factor: "<<input_dup_factor<<std::endl;
    int input_volume = H * W * C * input_dup_factor;
    // int input_volume = H * W * C;
    request = new Request(Request::Type::RowLoad);
    request->addOperand(sys->getAddress(0,0,0), input_volume, precision_input); //cram addr
    request->addOperand(sys->DRAM_ADDR, input_volume, precision_input); //dram addr
    request->setShuffle(0,0, 0, 0);
    requests.push_back(*request);  

    std::vector<int> v;
    for(int tile_=0; tile_<numTileInvolved; tile_++){//parallel on tiles
        v.push_back(tile_);
    }
    // int data_volume = ceil(M_p/(float)numColPerArray) * ceil(C/(float)numArrayPerTile) * R * S * numArrayPerTile * numColPerArray;

    

    sys->app_param_file<<"data_volume: "<<input_volume<<std::endl;
    sys->broadcast_p2p(sys->getAddress(0,0,0),precision_input, v, input_volume, requests);
            
                       
}