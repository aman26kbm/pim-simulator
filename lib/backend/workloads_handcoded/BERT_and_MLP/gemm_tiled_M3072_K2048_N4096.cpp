// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <numeric>

#include "backend/System.h"
#include "gemm_tiled.h"
int32_t gemm_tiled_M3072_K2048_N4096(System* sys, std::string param_file){
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    int matrixARowNum = 3072;
    int matrixAColNum = 2048;
    int matrixBRowNum = matrixAColNum;
    int matrixBColNum = 4096;

    gemm_tiled(3072, 2048, 4096, PrecisionT::INT8, PrecisionT::INT16, PrecisionT::INT32, PrecisionT::INT8, requests, sys);
    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);

    return 0;
}



static __attribute__((unused)) Registry::Entry &__gemm_tiled_M3072_K2048_N4096__ = pimsim::registerFunc("gemm_tiled_M3072_K2048_N4096", gemm_tiled_M3072_K2048_N4096);
