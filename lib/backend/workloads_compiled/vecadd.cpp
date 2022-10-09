// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t vecadd_compiled(System *sys) {
  void* _1 = nullptr;
  // int8_t a_global[256], 0
  // int8_t b_global[256], 256
  // cram-array axis
  {
    int32_t x_outer = 0;
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
      sys->sendRequest(request);
    }
    (void)_1;
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(0, 0, 8), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
      sys->sendRequest(request);
    }
    (void)_1;
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*a.global[ramp(0, 1, 256)]*/);
      request.addOperand(sys->getAddress(0, 0, 8), 0, PrecisionT::Precision{0, 8, 0} /*b.global[ramp(0, 1, 256)]*/);
      sys->sendRequest(request);
    }
    (void)_1;
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
      sys->sendRequest(request);
    }
    (void)_1;
  }
  // freed b_global
  // freed a_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_vecadd_compiled__ = pimsim::registerFunc("vecadd_compiled", vecadd_compiled);

