#include "conv2d_low_latency.h"

void conv2d_low_latency_store_input_dup(Conv_layer_params conv_layer_params,
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

    int M_p = ceil(M/(float)numTile);
    Request *request;

    for (int tile = 0; tile < numTile; tile++){
        
        int output_volume = E*F * M_p;
        request = new Request(Request::Type::RowStore);
        request->addOperand(sys->getAddress(tile,0,0), output_volume, precision_input); //cram addr
        request->addOperand(sys->DRAM_ADDR, output_volume, precision_input); //dram addr
        request->setShuffle(0,0, 0, 0);
        requests.push_back(*request);  
    }
                       
}