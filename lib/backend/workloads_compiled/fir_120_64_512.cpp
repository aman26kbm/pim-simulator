// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t fir_120_64_512(System *sys) {
  void* _1 = nullptr;
  // int16_t c_global[1024], 0
  for (int32_t x_outer = 0; x_outer < 120; ++x_outer) {
    // cram-array axis
    {
      int32_t x_inner_outer_outer = 0;
      // cram-array axis
      {
        int32_t x_inner_outer_inner = 0;
        void* _2 = (void*) "c.global[ramp(0, 1, 512)] = x512((int16)0)/*skip-init*/";
        for (int32_t rv_inner = 0; rv_inner < 32; ++rv_inner) {
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(x_outer, 0, 64), 512, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 32), 1024, PrecisionT::Precision{0, 16, 0} /*a[ramp(((((x.outer*65536) + (x.inner.outer.outer*32768)) + (x.inner.outer.inner*512)) + rv.inner), 1, 512)]*/);
            request.addOperand(x_outer * 32, 1, PrecisionT::Precision{0, 16, 0} /*b[rv.inner]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 512, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 512, PrecisionT::Precision{0, 16, 0} /*c.global[ramp(0, 1, 512)]*/);
            request.addOperand(sys->getAddress(x_outer, 0, 64), 512, PrecisionT::Precision{0, 16, 0} /**/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 512, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 512, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed c_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_fir_120_64_512__ = pimsim::registerFunc("fir_120_64_512", fir_120_64_512);

