#include "conv2d_low_latency.h"
void conv2d_low_latency_compute_input_dup_nap(Conv_layer_params conv_layer_params,
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

    int EF = E * F;
    int M_p = ceil(M / (float)numTile);

    int weightDupInArr = 1;
    if (numColPerArray / (float)M_p > 1)
        weightDupInArr = floor(numColPerArray / (float)M_p);

    int weightDupAcrossArr = 1;
    if (numArrayPerTile / (float)C > 1)
        weightDupAcrossArr = floor(numArrayPerTile / (float)C);

    Request *request;

    sys->app_param_file << "M_p" << M_p << std::endl;    
    sys->app_param_file << "EF: " << EF << std::endl;
    sys->app_param_file << "weightDupInArr: " << weightDupInArr << std::endl;
    sys->app_param_file << "weightDupAcrossArr: " << weightDupAcrossArr << std::endl;
    sys->app_param_file << "loop bounds: " << std::endl;
    sys->app_param_file << "m_: " << ceil(M / (float)numColPerArray) << std::endl;
    sys->app_param_file << "ef_: " << ceil(E * F / (float)EF) << std::endl;
    sys->app_param_file << "ef__: " << ceil(EF / (float)(weightDupInArr * weightDupAcrossArr)) << std::endl;
    sys->app_param_file << "c_: " << ceil(C / (float)numArrayPerTile) << std::endl;
    sys->app_param_file << "r: " << R << std::endl;
    sys->app_param_file << "s: " << S << std::endl;

    for (int m_ = 0; m_ < ceil(M / (float)M_p); m_++)
    {
        int tile = m_ % numTile;
        for (int m__ = 0; m__ < ceil(M_p / (float)numColPerArray); m__++)
        { // serial
            for (int ef_ = 0; ef_ < ceil(E * F / (float)EF); ef_++)
            { // parallel on tiles
                for (int ef__ = 0; ef__ < ceil(EF / (float)(weightDupInArr * weightDupAcrossArr)); ef__++)
                { // serial
                    // int increase_precision_index = 0;
                    // int two_to_n = 1;
                    // int curr_iter = 0;
                    // PrecisionT::Precision precision_accumulate_temp = PrecisionT::Precision{0,precision_multiply.bits(),0};
                    for (int c_ = 0; c_ < ceil(C / (float)numArrayPerTile); c_++)
                    { // serial, for reduction

                        for (int r = 0; r < R; r++)
                        { // serial
                            for (int s = 0; s < S; s++)
                            { // serial
                                request = new Request(Request::Type::ColBroadcast);
                                request->addOperand(sys->getAddress(tile, 0, 0), 0, precision_input); // src
                                requests.push_back(*request);

                                request = new Request(Request::Type::RowMul);
                                request->addOperand(sys->getAddress(tile, 0, 0), 0, precision_input);    // src
                                request->addOperand(sys->getAddress(tile, 0, 0), 0, precision_input);    // src_
                                request->addOperand(sys->getAddress(tile, 0, 0), 0, precision_multiply); // dst
                                requests.push_back(*request);
                                
                                
                                
                                
                                request = new Request(Request::Type::RowAdd);
                                request->addOperand(sys->getAddress(tile, 0, 0), 0, precision_multiply);   // src
                                request->addOperand(sys->getAddress(tile, 0, 0), 0, precision_accumulate); // src_
                                request->addOperand(sys->getAddress(tile, 0, 0), 0, precision_accumulate); // dst
                                requests.push_back(*request);
                            }
                        }
                    }
                    // reduce can be delayed until all c_ are done
                    int RowReduce_WithinTile_count = log2(std::min(C, numArrayPerTile));
                    request = new Request(Request::Type::RowReduce_WithinTile);
                    request->addOperand(sys->getAddress(tile, 0, 0), RowReduce_WithinTile_count, precision_accumulate); // src
                    request->addOperand(sys->getAddress(tile, 0, 0), RowReduce_WithinTile_count, precision_accumulate); // dst
                    requests.push_back(*request);
                }
            }
        }
    }
}