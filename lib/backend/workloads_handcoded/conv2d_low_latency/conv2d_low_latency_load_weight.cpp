#include "conv2d_low_latency.h"

void conv2d_low_latency_load_weight(Conv_layer_params conv_layer_params, \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System* sys){
    Config* cfg = sys->_config;
    int numColPerArray = cfg->_ncols;
    int numArrayPerTile = cfg->_nblocks;
    int numTile = cfg->_ntiles_used;
    int capacityPerTile = cfg->_nrows * cfg->_ncols * cfg->_nblocks;
    int meshWidth = cfg->_meshWidth;
    int meshHeight = cfg->_meshHeight;

    PrecisionT::Precision precision_temp = PrecisionT::INT8;

    int M = conv_layer_params.M;
    int C = conv_layer_params.C;
    int R = conv_layer_params.R;
    int S = conv_layer_params.S;
    int E = conv_layer_params.E;
    int F = conv_layer_params.F;
    int EF = ceil(E * F / (float)(meshWidth * meshHeight));
    int numTilesUsed = ceil(E*F/(float)EF);
    Request* request;
    for(int phase=0; phase<16; phase++){
        std::cout<<"phase: "<<phase<<std::endl;
        for(int tile=0; tile<meshWidth; tile++){
            std::cout<<"tile: "<<tile<<std::endl;
            int data_volume = M * C * R * S/ meshWidth/phase;
            request = new Request(Request::Type::RowLoad);
            request->addOperand(sys->getAddress(tile,0,0), data_volume, precision_input); //cram addr
            request->addOperand(sys->DRAM_ADDR, data_volume, precision_input); //dram addr
            request->setShuffle(0,0, 0, 0);
            requests.push_back(*request); 

            std::cout<<"broadcast"<<std::endl;

            std::vector<int> v;
            for(int tile_=0; tile_<numTilesUsed; tile_++){//parallel on tiles
                v.push_back(tile_);
            }
            // int data_volume = ceil(M_p/(float)numColPerArray) * ceil(C/(float)numArrayPerTile) * R * S * numArrayPerTile * numColPerArray;
            std::cout<<"data_volume: "<<data_volume<<std::endl;
            sys->broadcast_p2p(sys->getAddress(tile,0,0),precision_input, v, data_volume, requests);

        }
    }
}