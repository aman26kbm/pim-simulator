// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t vecadd_16_256_256_int8(System *sys) {
  void* _1 = nullptr;
  for (int32_t x_outer = 0; x_outer < 16; ++x_outer) {
    // cram-array axis
    {
      int32_t x_inner_outer_outer = 0;
      for (int32_t x_inner_outer_inner = 0; x_inner_outer_inner < 15; ++x_inner_outer_inner) {
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*a[ramp((((x.outer*983040) + (x.inner.outer.outer*3840)) + (x.inner.outer.inner*256)), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(x_outer, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /*b[ramp((((x.outer*983040) + (x.inner.outer.outer*3840)) + (x.inner.outer.inner*256)), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*a[ramp((((x.outer*983040) + (x.inner.outer.outer*3840)) + (x.inner.outer.inner*256)), 1, 256)]*/);
          request.addOperand(sys->getAddress(x_outer, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /*b[ramp((((x.outer*983040) + (x.inner.outer.outer*3840)) + (x.inner.outer.inner*256)), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(x_outer, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_vecadd_16_256_256_int8__ = pimsim::registerFunc("vecadd_16_256_256_int8", vecadd_16_256_256_int8);

