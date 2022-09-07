// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t vecadd_multi_tile(System *sys) {
  void* _1 = nullptr;
  // int16_t a_global[512], 0
  // int16_t b_global[512], 512
  // cram-tile axis
  for (int32_t x_outer_outer = 0; x_outer_outer < 128; ++x_outer_outer) {
    // cram-array axis
    {
      int32_t x_outer_inner = 0;
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((0) * 2/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT16); // dst
        request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); // src
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((0) * 2/*bytes*/) / 32/*row-number*/ + 16/*cram buffer*/), 0, PrecisionT::INT16); // dst
        request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); // src
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.type = Request::Type::RowAdd;
        request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((0) * 2/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT16);
        request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((0) * 2/*bytes*/) / 32/*row-number*/ + 16/*cram buffer*/), 0, PrecisionT::INT16);
        request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((0) * 2/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT16);
        request.swapSrcDst(); // for source, dest, source operand order
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((0) * 2/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT16); // src
        request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); // dst
        sys->sendRequest(request);
      }
    }
  }
  // freed b_global
  // freed a_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &__vecadd_multi_tile__ = pimsim::registerFunc("vecadd_multi_tile", vecadd_multi_tile);
