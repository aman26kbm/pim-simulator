#include "conv2d_low_latency.h"

void conv2d_low_latency_store(Conv_layer_params conv_layer_params,
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

     int EF = pow(ceil(sqrt(E * F / (float)(meshWidth * meshHeight))),2);

    Request *request;

    for (int ef_ = 0; ef_ < ceil(E * F / (float)EF); ef_++){
        
        int output_volume = EF * M;
        request = new Request(Request::Type::RowStore);
        request->addOperand(sys->getAddress(ef_,0,0), output_volume, precision_input); //cram addr
        request->addOperand(sys->DRAM_ADDR, output_volume, precision_input); //dram addr
        request->setShuffle(0,0, 0, 0);
        requests.push_back(*request);  
    }
                       
}