// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t fir_120_256_256_int7_int28(System *sys) {
  void* _1 = nullptr;
  // int28 c_global[768], 0
  // int28 c_global_rf[768], 1024
  for (int32_t x_outer = 0; x_outer < 120; ++x_outer) {
    // cram-array axis
    {
      int32_t x_inner_outer_outer = 0;
      for (int32_t x_inner_outer_inner = 0; x_inner_outer_inner < 32; ++x_inner_outer_inner) {
        void* _2 = (void*) "c.global[ramp(0, 1, 256)] = x256((int28)0)/*skip-init*/";
        // cram-array axis
        {
          int32_t rv = 0;
          {
            Request request(Request::Type::RowMul);
            request.addOperand(sys->getAddress(x_outer, 0, 32), 65536, PrecisionT::Precision{0, 14, 0} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 64), 65536, PrecisionT::Precision{0, 7, 0} /*a[ramp(((((x.outer*65536) + (x.inner.outer.outer*8192)) + (x.inner.outer.inner*256)) + rv), 1, 256)]*/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 32, PrecisionT::Precision{0, 7, 0} /*b[rv]*/);
            sys->sendRequest(request);
          }
        }
        #define max(a,b) ((a)>(b)?(a):(b))
        for (int32_t rv_v_inner = 32, rv_v_inner_cnt = 1; rv_v_inner >= 1; rv_v_inner -= max(rv_v_inner / 2, 1), ++rv_v_inner_cnt) {
        #undef max
          {
            Request request(Request::Type::BlockSend_Receive);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 2048, PrecisionT::Precision{0, 28, 0} /*c.global[ramp(0, 1, 256)]*/);
            request.addOperand(sys->getAddress(x_outer, 0, 32), 256, PrecisionT::Precision{0, 28, 0} /*c.global.rf[ramp((rv.v.inner*256), 1, 256)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 2048, PrecisionT::Precision{0, 28, 0} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 2048, PrecisionT::Precision{0, 28, 0} /*c.global[ramp(0, 1, 256)]*/);
            request.addOperand(sys->getAddress(x_outer, 0, 32), 256, PrecisionT::Precision{0, 28, 0} /*c.global.rf[ramp((rv.v.inner*256), 1, 256)]*/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 2048, PrecisionT::Precision{0, 28, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 2048, PrecisionT::Precision{0, 28, 0} /*CRAM*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed c_global_rf
  // freed c_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_fir_120_256_256_int7_int28__ = pimsim::registerFunc("fir_120_256_256_int7_int28", fir_120_256_256_int7_int28);

