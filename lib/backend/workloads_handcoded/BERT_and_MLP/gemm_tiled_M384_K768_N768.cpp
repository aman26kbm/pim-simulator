// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <numeric>

#include "backend/System.h"
#include "gemm_tiled.h"

int32_t gemm_tiled_M384_K768_N768(System* sys, std::string param_file){
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    int matrixARowNum = 384;
    int matrixAColNum = 768;
    int matrixBRowNum = matrixAColNum;
    int matrixBColNum = 768;

    gemm_tiled(384, 768, 768, PrecisionT::INT8, PrecisionT::INT16, PrecisionT::INT32, PrecisionT::INT8, requests, sys);
    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);

    return 0;
}



static __attribute__((unused)) Registry::Entry &__gemm_tiled_M384_K768_N768__ = pimsim::registerFunc("gemm_tiled_M384_K768_N768", gemm_tiled_M384_K768_N768);
