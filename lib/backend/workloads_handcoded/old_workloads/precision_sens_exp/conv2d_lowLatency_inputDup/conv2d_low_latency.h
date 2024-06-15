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

void conv2d_low_latency_load_weight(Conv_layer_params conv_layer_params, \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System* sys);


void conv2d_low_latency_load_weight_dup(Conv_layer_params conv_layer_params, \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System* sys);


void conv2d_low_latency_load_input(Conv_layer_params conv_layer_params,  \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System* sys);

void conv2d_low_latency_load_input_shuffleOff(Conv_layer_params conv_layer_params,  \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System* sys);


void conv2d_low_latency_load_input_dup(Conv_layer_params conv_layer_params, \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System *sys);

void conv2d_low_latency_load_input_dup_shuffleOff(Conv_layer_params conv_layer_params,  \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System* sys);

void conv2d_low_latency_compute_weight_dup(Conv_layer_params conv_layer_params,  \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System* sys);

void conv2d_low_latency_compute_weight_dup_nap(Conv_layer_params conv_layer_params,  \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System* sys);

void conv2d_low_latency_compute_weight_dup_shuffleOff(Conv_layer_params conv_layer_params,  \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System* sys);

void conv2d_low_latency_compute_input_dup(Conv_layer_params conv_layer_params,  \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System* sys);

void conv2d_low_latency_compute_input_dup_nap(Conv_layer_params conv_layer_params,  \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System* sys);

void conv2d_low_latency_compute_input_dup_shuffleOff(Conv_layer_params conv_layer_params,  \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System* sys);


void conv2d_low_latency_store_weight_dup(Conv_layer_params conv_layer_params,  \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System* sys);

void conv2d_low_latency_store_input_dup(Conv_layer_params conv_layer_params,  \
                                    PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                                    std::vector<Request> &requests, System* sys);