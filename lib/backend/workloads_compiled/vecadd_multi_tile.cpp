// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t vecadd_multi_tile(System *sys) {
  void* _1 = nullptr;
  // int16_t a_global[512], 0
  // int16_t b_global[512], 512
  for (int32_t x_outer_outer = 0; x_outer_outer < 128; ++x_outer_outer) {
    // cram-array axis
    {
      int32_t x_outer_inner = 0;
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        sys->sendRequest(request);
      }
      (void)_1;
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 16), 0, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        sys->sendRequest(request);
      }
      (void)_1;
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*a.global[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 16), 0, PrecisionT::Precision{0, 16, 0} /*b.global[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      (void)_1;
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
      (void)_1;
    }
  }
  // freed b_global
  // freed a_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_vecadd_multi_tile__ = pimsim::registerFunc("vecadd_multi_tile", vecadd_multi_tile);

