// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t reduce(System *sys) {
  void* _1 = nullptr;
  // int8_t b_global[256], 0
  // int8_t a_global[4096], 256
  int32_t x_outer = 0; /*array-dummized*/ // {
    {
      // initialization skipped
    }
    for (int32_t k_outer = 0; k_outer < 64; ++k_outer) {
      for (int32_t ax1 = 0; ax1 < 16; ++ax1) {
        {
          int32_t _2 = ax1 * 256;
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::INT8);
          request.addOperand((_2) * 1/*bytes*/ + 256/*cram buffer*/ + 8192/*array-size*/ * x_outer/*array-id*/, 0, PrecisionT::INT8);
          sys->sendRequest(request);
        }
      }
      for (int32_t k_inner = 0; k_inner < 16; ++k_inner) {
        {
          Request request(Request::Type::RowStore);
          request.type = Request::Type::RowAdd;
          request.addOperand((0) * 1/*bytes*/ + 0/*cram buffer*/, 0, PrecisionT::INT8);
          int32_t _3 = k_inner * 256;
          request.addOperand((_3) * 1/*bytes*/ + 256/*cram buffer*/ + 8192/*array-size*/ * x_outer/*array-id*/, 0, PrecisionT::INT8);
          request.addOperand(sys->getAddress(0, 0, 0)/*FIXME: dst*/, 0, PrecisionT::INT8);
          sys->sendRequest(request);
        }
      }
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand((0) * 1/*bytes*/ + 0/*cram buffer*/, 0, PrecisionT::INT8);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::INT8);
      sys->sendRequest(request);
    }
  // }
  // freed a_global
  // freed b_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &__reduce__ = pimsim::registerFunc("reduce", reduce);
