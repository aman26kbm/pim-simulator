#include "conv2d_low_latency.h"

void conv2d_low_latency_load_input(Conv_layer_params conv_layer_params,
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
        int E_p = (int)sqrt(EF);
        int F_p = (int)sqrt(EF);
        int H_p = (E_p-1)*Stride+R;
        int W_p = (F_p-1)*Stride+S;

        int weightDupInArr = 1;
        if (numColPerArray / (float)M > 1)
            weightDupInArr = floor(numColPerArray / (float)M);

        int weightDupAcrossArr = 1;
        if (numArrayPerTile / (float)C > 1)
            weightDupAcrossArr = floor(numArrayPerTile / (float)C);

        int input_dup_factor = numColPerArray/weightDupInArr;

        // int input_volume = H_p * W_p * C * input_dup_factor;
        int input_volume = H_p * W_p * C;
        request = new Request(Request::Type::RowLoad);
        request->addOperand(sys->getAddress(ef_,0,0), input_volume, precision_input); //cram addr
        request->addOperand(sys->DRAM_ADDR, input_volume, precision_input); //dram addr
        request->setShuffle(0,0, 0, 0);
        requests.push_back(*request);  
    }
                       
}