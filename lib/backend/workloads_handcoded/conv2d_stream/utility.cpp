#include "conv2d_stream.h"

void print_conv_layer_params(Conv_layer_params conv_layer_params){
    std::cout<<"Conv layer params:"<<std::endl;
    std::cout<<"N: "<<conv_layer_params.N<<std::endl;
    std::cout<<"H: "<<conv_layer_params.H<<std::endl;
    std::cout<<"W: "<<conv_layer_params.W<<std::endl;
    std::cout<<"C: "<<conv_layer_params.C<<std::endl;
    std::cout<<"Stride: "<<conv_layer_params.stride<<std::endl;
    std::cout<<"R: "<<conv_layer_params.R<<std::endl;
    std::cout<<"S: "<<conv_layer_params.S<<std::endl;
    std::cout<<"M: "<<conv_layer_params.M<<std::endl;
}