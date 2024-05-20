#include <cstdint>
#include <numeric>

#include "backend/System.h"

void gemm_tiled( int M, int K, int N,\
                PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, PrecisionT::Precision precision_result, \
                std::vector<Request> &requests, System* sys);
