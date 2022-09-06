// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t mv(System *sys) {
  void* _1 = nullptr;
  // int32_t c_global[1024], 0
  // int8_t a_global[256], 1024
  // cram-array axis
  {
    int32_t x_outer = 0;
    {
      // initialization skipped
    }
    for (int32_t k_outer = 0; k_outer < 256; ++k_outer) {
      for (int32_t k_inner = 0; k_inner < 16; ++k_inner) {
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 1/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT8); // dst
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT8); // src
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowStore);
          request.type = Request::Type::RowAdd;
          request.addOperand(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
          {
            Request request(Request::Type::RowMul);
            request.addOperand(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 1/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT8);
            request.type = Request::Type::RowMul_CRAM_RF;
            request.addOperand(0, 0, PrecisionT::INT8);
            request.addOperand(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
            sys->sendRequest(request);
          }
          request.addOperand(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
          request.addOperand(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
          request.swapSrcDst(); // for source, dest, source operand order
          sys->sendRequest(request);
        }
      }
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32); // src
      request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT32); // dst
      sys->sendRequest(request);
    }
  }
  // freed a_global
  // freed c_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &__mv__ = pimsim::registerFunc("mv", mv);
