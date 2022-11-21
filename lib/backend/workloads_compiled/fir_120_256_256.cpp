// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t fir_120_256_256(System *sys) {
  void* _1 = nullptr;
  // int16_t c_global[512], 0
  for (int32_t x_outer = 0; x_outer < 120; ++x_outer) {
    // cram-array axis
    {
      int32_t x_inner_outer_outer = 0;
      // cram-array axis
      {
        int32_t x_inner_outer_inner = 0;
        void* _2 = (void*) "c.global[ramp(0, 1, 256)] = x256((int16)0)/*skip-init*/";
        for (int32_t rv = 0; rv < 32; ++rv) {
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(x_outer, 0, 32), 65536, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 16), 65536, PrecisionT::Precision{0, 16, 0} /*a[ramp(((((x.outer*65536) + (x.inner.outer.outer*8192)) + (x.inner.outer.inner*256)) + rv), 1, 256)]*/);
            request.addOperand(x_outer * 32, 1, PrecisionT::Precision{0, 16, 0} /*b[rv]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 65536, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 65536, PrecisionT::Precision{0, 16, 0} /*c.global[ramp(0, 1, 256)]*/);
            request.addOperand(sys->getAddress(x_outer, 0, 32), 65536, PrecisionT::Precision{0, 16, 0} /**/);
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

static __attribute__((unused)) Registry::Entry &_fir_120_256_256__ = pimsim::registerFunc("fir_120_256_256", fir_120_256_256);

