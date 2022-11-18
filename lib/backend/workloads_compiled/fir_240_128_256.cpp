// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t fir_240_128_256(System *sys) {
  void* _1 = nullptr;
  // int16_t c_global[512], 0
  for (int32_t x_outer = 0; x_outer < 240; ++x_outer) {
    // cram-array axis
    {
      int32_t x_inner_outer_outer = 0;
      // cram-array axis
      {
        int32_t x_inner_outer_inner = 0;
        void* _2 = (void*) "c.global[ramp(0, 1, 256)] = x256((int16)0)/*skip-init*/";
        for (int32_t rv_inner = 0; rv_inner < 32; ++rv_inner) {
          {
            Request request(Request::Type::RowShift);
            request.addOperand(sys->getAddress(x_outer, 0, 16), 32768, PrecisionT::Precision{0, 16, 0} /*Unaligned Shift*/);
            request.addOperand(sys->getAddress(x_outer, 0, 16), 32768, PrecisionT::Precision{0, 16, 0} /*Unaligned Shift*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(x_outer, 0, 32), 32768, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 16), 32768, PrecisionT::Precision{0, 16, 0} /*a[ramp(((((x.outer*32768) + (x.inner.outer.outer*8192)) + (x.inner.outer.inner*256)) + rv.inner), 1, 256)]*/);
            request.addOperand(x_outer * 32, 128, PrecisionT::Precision{0, 16, 0} /*b[rv.inner]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /*c.global[ramp(0, 1, 256)]*/);
            request.addOperand(sys->getAddress(x_outer, 0, 32), 32768, PrecisionT::Precision{0, 16, 0} /**/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed c_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_fir_240_128_256__ = pimsim::registerFunc("fir_240_128_256", fir_240_128_256);

