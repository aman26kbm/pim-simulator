#include <cstdint>
#include <cmath>
#include <numeric>
#include <sstream>

#include "backend/System.h"

void gemm_load(int M, int K, int N,\
                PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                std::vector<Request> &requests, System* sys);

void gemm_compute(int M, int K, int N,\
                PrecisionT::Precision precision_input, PrecisionT::Precision precision_multiply, PrecisionT::Precision precision_accumulate, \
                std::vector<Request> &requests, System* sys);