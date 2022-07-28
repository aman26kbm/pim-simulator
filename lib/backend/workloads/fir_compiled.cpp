// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t fir_compiled(System *sys) {
  void* _1 = nullptr;
  // int16_t a_global[908], 0
  // int16_t c_global[512], 908
  // cram-array axis
  {
    int32_t x_outer = 0;
    // pragma skipped!
    {
      // initialization skipped
    }
    for (int32_t rv = 0; rv < 199; ++rv) {
      {
        Request request(Request::Type::RowStore);
        request.type = Request::Type::RowAdd;
        request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 2/*bytes*/) / 32/*row-number*/ + 28/*cram buffer*/), 0, PrecisionT::INT16);
        {
          Request request(Request::Type::RowMul);
          request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 2/*bytes*/) / 32/*row-number*/ + 28/*cram buffer*/), 0, PrecisionT::INT16);
          request.addAddr(0, 0, PrecisionT::INT16);
          request.type = Request::Type::RowMul_CRAM_RF;
          request.addAddr(0, 0, PrecisionT::INT16);
          request.swapSrcDst();
          sys->sendRequest(request);
          Request shift_req0(Request::Type::RowShift);
          shift_req0.addAddr(0, 0, PrecisionT::INT16);
          shift_req0.addAddr(0, 0, PrecisionT::INT16);
          sys->sendRequest(shift_req0);
        }
        request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 2/*bytes*/) / 32/*row-number*/ + 28/*cram buffer*/), 0, PrecisionT::INT16);
        request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 2/*bytes*/) / 32/*row-number*/ + 28/*cram buffer*/), 0, PrecisionT::INT16);
        request.swapSrcDst(); // for source, dest, source operand order
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowStore);
      request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 2/*bytes*/) / 32/*row-number*/ + 28/*cram buffer*/), 0, PrecisionT::INT16); // src
      request.addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT16); // dst
      sys->sendRequest(request);
    }
  }
  // freed c_global
  // freed a_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &__fir_compiled__ = pimsim::registerFunc("fir_compiled", fir_compiled);
