#include "conv2d_stream.h"

void conv2d_stream_load_weight(Conv_layer_params conv_layer_params,  \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<int> tiles, \
                                    std::vector<Request> &requests, System* sys){
    Config* cfg = sys->_config;
    int numColPerArray = cfg->_ncols;
    int numArrayPerTile = cfg->_nblocks;
    int numTile = cfg->_ntiles_used;
    int capacityPerTile = cfg->_nrows * cfg->_ncols * cfg->_nblocks;
    int meshWidth = cfg->_meshWidth;
    int meshHeight = cfg->_meshHeight;

    PrecisionT::Precision precision_temp = PrecisionT::INT8;

    Request* request;
    for(int tile : tiles){
        int num_tiles = tiles.size();
        int data_volume = ceil(conv_layer_params.M/(float)num_tiles) * conv_layer_params.R * conv_layer_params.S * conv_layer_params.C;
        request = new Request(Request::Type::RowLoad);
                    request->addOperand(sys->getAddress(tile,0,0), data_volume, precision_input); //cram addr
                    request->addOperand(sys->DRAM_ADDR, data_volume, precision_input); //dram addr
                    request->setShuffle(0,0, 0, 0);
                    requests.push_back(*request); 
        requests.push_back(*request);
    }

}