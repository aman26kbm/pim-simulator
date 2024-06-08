// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <numeric>

#include "backend/System.h"
#include "gemm_tiled.h"
int32_t gemm_tiled_M384_K64_N384(System* sys, std::string param_file){
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    int matrixARowNum = 384;
    int matrixAColNum = 64;
    int matrixBRowNum = matrixAColNum;
    int matrixBColNum = 384;

    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT32;
    PrecisionT::Precision precision_result = PrecisionT::INT8;

    func_gemm_tiled(384, 64, 384, precision_input, precision_multiply, precision_accumulate, precision_result, requests, sys);
    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);

    return 0;
}



static __attribute__((unused)) Registry::Entry &__gemm_tiled_M384_K64_N384__ = pimsim::registerFunc("gemm_tiled_M384_K64_N384", gemm_tiled_M384_K64_N384);
