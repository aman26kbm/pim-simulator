// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t mam(System *sys) {
  void* _1 = nullptr;
  // int16_t tmp0[2048], 0
  // int16_t a_global[2048], 2048
  // int16_t b_global[2048], 4096
  int32_t x_outer_outer = 0; /*array-dummized*/ // {
    for (int32_t x_outer = 0; x_outer < 4; ++x_outer) {
      {
        Request request(Request::Type::RowLoad);
        request.addAddr(sys->getAddress(0, 0, 0), 0, PrecisionT::INT16);
        request.addAddr((0) * 2/*bytes*/ + 2048/*cram buffer*/ + 8192/*array-size*/ * x_outer_outer/*array-id*/, 0, PrecisionT::INT16);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addAddr(sys->getAddress(0, 0, 0), 0, PrecisionT::INT16);
        request.addAddr((0) * 2/*bytes*/ + 4096/*cram buffer*/ + 8192/*array-size*/ * x_outer_outer/*array-id*/, 0, PrecisionT::INT16);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        int32_t _2 = x_outer * 256;
        request.type = Request::Type::RowMul;
        request.addAddr((0) * 2/*bytes*/ + 2048/*cram buffer*/ + 8192/*array-size*/ * x_outer_outer/*array-id*/, 0, PrecisionT::INT16);
        request.addAddr((0) * 2/*bytes*/ + 4096/*cram buffer*/ + 8192/*array-size*/ * x_outer_outer/*array-id*/, 0, PrecisionT::INT16);
        request.addAddr((_2) * 2/*bytes*/ + 0/*cram buffer*/, 0, PrecisionT::INT16);
        sys->sendRequest(request);
      }
    }
    for (int32_t ax0_outer = 0; ax0_outer < 4; ++ax0_outer) {
      {
        int32_t _3 = ax0_outer * 256;
        Request request(Request::Type::RowLoad);
        request.addAddr(sys->getAddress(0, 0, 0), 0, PrecisionT::INT16);
        request.addAddr((_3) * 2/*bytes*/ + 4096/*cram buffer*/ + 8192/*array-size*/ * x_outer_outer/*array-id*/, 0, PrecisionT::INT16);
        sys->sendRequest(request);
      }
    }
    for (int32_t ax0_outer1 = 0; ax0_outer1 < 4; ++ax0_outer1) {
      {
        int32_t _4 = ax0_outer1 * 256;
        Request request(Request::Type::RowLoad);
        request.addAddr(sys->getAddress(0, 0, 0), 0, PrecisionT::INT16);
        request.addAddr((_4) * 2/*bytes*/ + 2048/*cram buffer*/ + 8192/*array-size*/ * x_outer_outer/*array-id*/, 0, PrecisionT::INT16);
        sys->sendRequest(request);
      }
    }
    for (int32_t x_outer1 = 0; x_outer1 < 4; ++x_outer1) {
      {
        Request request(Request::Type::RowStore);
        int32_t _5 = x_outer1 * 256;
        request.type = Request::Type::RowMul;
        request.addAddr((_5) * 2/*bytes*/ + 4096/*cram buffer*/ + 8192/*array-size*/ * x_outer_outer/*array-id*/, 0, PrecisionT::INT16);
        request.addAddr((_5) * 2/*bytes*/ + 2048/*cram buffer*/ + 8192/*array-size*/ * x_outer_outer/*array-id*/, 0, PrecisionT::INT16);
        request.addAddr((_5) * 2/*bytes*/ + 4096/*cram buffer*/ + 8192/*array-size*/ * x_outer_outer/*array-id*/, 0, PrecisionT::INT16);
        sys->sendRequest(request);
      }
    }
    for (int32_t x_c_outer = 0; x_c_outer < 4; ++x_c_outer) {
      {
        Request request(Request::Type::RowStore);
        int32_t _6 = x_c_outer * 256;
        request.type = Request::Type::RowAdd;
        request.addAddr((_6) * 2/*bytes*/ + 0/*cram buffer*/, 0, PrecisionT::INT16);
        request.addAddr((_6) * 2/*bytes*/ + 4096/*cram buffer*/ + 8192/*array-size*/ * x_outer_outer/*array-id*/, 0, PrecisionT::INT16);
        request.addAddr((_6) * 2/*bytes*/ + 0/*cram buffer*/, 0, PrecisionT::INT16);
        sys->sendRequest(request);
      }
    }
    for (int32_t x_outer_inner = 0; x_outer_inner < 4; ++x_outer_inner) {
      {
        int32_t _7 = x_outer_inner * 256;
        Request request(Request::Type::RowStore);
        request.addAddr((_7) * 2/*bytes*/ + 0/*cram buffer*/, 0, PrecisionT::INT16);
        request.addAddr(sys->getAddress(0, 0, 0), 0, PrecisionT::INT16);
        sys->sendRequest(request);
      }
    }
  // }
  // freed b_global
  // freed a_global
  // freed tmp0
  return 0;
}

static __attribute__((unused)) Registry::Entry &__mam__ = pimsim::registerFunc("mam", mam);
