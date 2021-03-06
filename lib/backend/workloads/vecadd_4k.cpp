// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t vecadd_4k(System *sys) {
  void* _1 = nullptr;
  // int8_t a_global[1024], 0
  // int8_t b_global[1024], 1024
  // cram-array axis
  for (int32_t x_outer_outer = 0; x_outer_outer < 4; ++x_outer_outer) {
    for (int32_t ax0_outer = 0; ax0_outer < 4; ++ax0_outer) {
      {
        int32_t _2 = ax0_outer * 256;
        Request request(Request::Type::RowLoad);
        request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0, ((_2) * 1/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT8);
        request.addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT8);
        sys->sendRequest(request);
      }
    }
    for (int32_t ax0_outer1 = 0; ax0_outer1 < 4; ++ax0_outer1) {
      {
        int32_t _3 = ax0_outer1 * 256;
        Request request(Request::Type::RowLoad); 
        request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0, ((_3) * 1/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT8);
        request.addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT8);
        sys->sendRequest(request);
      }
    }
    for (int32_t x_c_outer = 0; x_c_outer < 4; ++x_c_outer) {
      {
        Request request(Request::Type::RowStore);
        int32_t _4 = x_c_outer * 256;
        request.type = Request::Type::RowAdd;
        request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0, ((_4) * 1/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT8);
        request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0, ((_4) * 1/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT8);
        request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0, ((_4) * 1/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT8);
        request.swapSrcDst(); // for source, dest, source operand order
        sys->sendRequest(request);
      }
    }
    for (int32_t x_outer_inner = 0; x_outer_inner < 4; ++x_outer_inner) {
      {
        int32_t _5 = x_outer_inner * 256;
        Request request(Request::Type::RowStore);
        request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0, ((_5) * 1/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT8);
        request.addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT8);
        sys->sendRequest(request);
      }
    }
  }
  // freed b_global
  // freed a_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &__vecadd_4k__ = pimsim::registerFunc("vecadd_4k", vecadd_4k);
