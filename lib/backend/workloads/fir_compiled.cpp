// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t fir_compiled(System *sys) {
  void* _1 = nullptr;
  // int16_t a_global[908], 0
  for (int32_t x_outer = 0; x_outer < 4; ++x_outer) {
    {
      // initialization skipped
    }
    for (int32_t rv = 0; rv < 199; ++rv) {
      {
        Request request(Request::Type::RowStore);
        request.type = Request::Type::RowAdd;
        request.addAddr(0, 0, PrecisionT::INT16);
        {
          Request request(Request::Type::RowMul);
          request.addAddr(sys->getAddress(0, 0, 0), 0, PrecisionT::INT16);
          request.addAddr(0, 0, PrecisionT::INT16);
          request.addAddr(sys->getAddress(0, 0, 0), 0, PrecisionT::INT16);
          sys->sendRequest(request);
          Request shift_req0(Request::Type::RowShift);
          shift_req0.addAddr(0, 0, PrecisionT::INT16);
          shift_req0.addAddr(0, 0, PrecisionT::INT16);
          sys->sendRequest(shift_req0);
        }
        request.addAddr(sys->getAddress(0, 0, 0), 0, PrecisionT::INT16);
        request.addAddr(sys->getAddress(0, 0, 0), 0, PrecisionT::INT16);
        sys->sendRequest(request);
      }
    }
  }
  // freed a_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &__fir_compiled__ = pimsim::registerFunc("fir_compiled", fir_compiled);
