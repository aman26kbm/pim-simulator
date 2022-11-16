// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t fir_120_1024_128(System *sys) {
  void* _1 = nullptr;
  // int16_t c_global[256], 0
  for (int32_t x_outer = 0; x_outer < 120; ++x_outer) {
    // cram-array axis
    {
      int32_t x_inner_outer_outer = 0;
      // cram-array axis
      {
        int32_t x_inner_outer_inner = 0;
        void* _2 = (void*) "c.global[ramp(0, 1, 128)] = x128((int16)0)/*skip-init*/";
        for (int32_t rv_inner = 0; rv_inner < 32; ++rv_inner) {
          {
            Request request(Request::Type::RowShift);
            request.addOperand(sys->getAddress(x_outer, 0, 8), 65536, PrecisionT::Precision{0, 16, 0} /*Unaligned Shift*/);
            request.addOperand(sys->getAddress(x_outer, 0, 8), 65536, PrecisionT::Precision{0, 16, 0} /*Unaligned Shift*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(x_outer, 0, 16), 65536, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 8), 65536, PrecisionT::Precision{0, 16, 0} /*a[ramp(((((x.outer*65536) + (x.inner.outer.outer*2048)) + (x.inner.outer.inner*128)) + rv.inner), 1, 128)]*/);
            request.addOperand(x_outer * 32, 512, PrecisionT::Precision{0, 16, 0} /*b[rv.inner]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 65536, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 65536, PrecisionT::Precision{0, 16, 0} /*c.global[ramp(0, 1, 128)]*/);
            request.addOperand(sys->getAddress(x_outer, 0, 16), 65536, PrecisionT::Precision{0, 16, 0} /**/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 65536, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 65536, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed c_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_fir_120_1024_128__ = pimsim::registerFunc("fir_120_1024_128", fir_120_1024_128);

