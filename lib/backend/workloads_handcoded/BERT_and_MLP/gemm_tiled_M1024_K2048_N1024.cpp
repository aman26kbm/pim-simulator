// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <numeric>

#include "backend/System.h"
#include "gemm_tiled.h"
int32_t gemm_tiled_M1024_K2048_N1024(System* sys, std::string param_file){
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    int matrixARowNum = 1024;
    int matrixAColNum = 2048;
    int matrixBRowNum = matrixAColNum;
    int matrixBColNum = 1024;

    gemm_tiled(1024, 2048, 1024, PrecisionT::INT8, PrecisionT::INT16, PrecisionT::INT32, PrecisionT::INT8, requests, sys);
    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);

    return 0;
}



static __attribute__((unused)) Registry::Entry &__gemm_tiled_M1024_K2048_N1024__ = pimsim::registerFunc("gemm_tiled_M1024_K2048_N1024", gemm_tiled_M1024_K2048_N1024);
