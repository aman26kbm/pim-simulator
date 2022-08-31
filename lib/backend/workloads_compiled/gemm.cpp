// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t gemm_compiled(System *sys) {
  void* _1 = nullptr;
  // int32_t compute_global[4096], 0
  // int8_t a_global[256], 4096
  // cram-array axis
  {
    int32_t xo = 0;
    for (int32_t y_outer = 0; y_outer < 8; ++y_outer) {
      for (int32_t y_c_inner_init = 0; y_c_inner_init < 4; ++y_c_inner_init) {
        {
          // initialization skipped
        }
      }
      for (int32_t rv = 0; rv < 2048; ++rv) {
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 1/*bytes*/) / 32/*row-number*/ + 128/*cram buffer*/), 0, PrecisionT::INT8); // dst
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT8); // src
          sys->sendRequest(request);
        }
        for (int32_t y_c_inner = 0; y_c_inner < 4; ++y_c_inner) {
          {
            Request request(Request::Type::RowStore);
            int32_t _2 = y_c_inner * 256;
            request.type = Request::Type::RowAdd;
            request.addOperand(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((_2) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
            {
              Request request(Request::Type::RowMul);
              request.addOperand(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 1/*bytes*/) / 32/*row-number*/ + 128/*cram buffer*/), 0, PrecisionT::INT8);
              request.type = Request::Type::RowMul_CRAM_RF;
              request.addOperand(0, 0, PrecisionT::INT8);
              request.addOperand(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((_2) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
              sys->sendRequest(request);
            }
            request.addOperand(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((_2) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
            request.addOperand(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((_2) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
            request.swapSrcDst(); // for source, dest, source operand order
            sys->sendRequest(request);
          }
        }
      }
      for (int32_t y_inner = 0; y_inner < 4; ++y_inner) {
        {
          int32_t _3 = y_inner * 256;
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((_3) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32); // src
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT32); // dst
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed a_global
  // freed compute_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &__gemm__ = pimsim::registerFunc("gemm", gemm_compiled);
