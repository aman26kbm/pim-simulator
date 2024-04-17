#include <cstdint>
#include <cmath>
#include <numeric>
#include <sstream>
#include "backend/System.h"

struct Conv_layer_params{
    int N;
    int H;
    int W;
    int C;
    int stride;
    int R;
    int S;
    int M;
    int E;
    int F;
};

void print_conv_layer_params(Conv_layer_params conv_layer_params);

void conv2d_stream_load_weight(Conv_layer_params conv_layer_params,  \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<int> tiles, \
                                    std::vector<Request> &requests, System* sys);


void conv2d_stream_load_input(int N, int M, int E, int F, int C, int R, int S, int stride, \
                                    int N_p, int M_p, int H_Yp, int W_Yp, int n_, \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System* sys);

void conv2d_stream_compute(Conv_layer_params conv_layer_params,  \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<int> tiles_receive_from, std::vector<int> tiles, std::vector<int> tiles_send_to, \
                                    std::vector<Request> &requests, System* sys);