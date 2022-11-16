// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t gemm4_120_64_512(System *sys) {
  void* _1 = nullptr;
  // int16_t compute_global_rf[1024], 0
  // int16_t compute_global[1024], 1024
  for (int32_t xo_outer = 0; xo_outer < 120; ++xo_outer) {
    for (int32_t y_inner_outer = 0; y_inner_outer < 4; ++y_inner_outer) {
      for (int32_t y_inner_inner = 0; y_inner_inner < 8; ++y_inner_inner) {
        // cram-array axis
        {
          int32_t rv_outer = 0;
          void* _2 = (void*) "compute.global.rf[ramp((rv.outer*512), 1, 512)] = x512((int16)0)/*skip-init*/";
          for (int32_t rv_inner = 0; rv_inner < 32; ++rv_inner) {
            {
              Request request(Request::Type::RowMul);
              request.addOperand(sys->getAddress(xo_outer, 0, 80), 32768, PrecisionT::Precision{0, 8, 0} /**/);
              request.addOperand(sys->getAddress(xo_outer, 0, 64), 32768, PrecisionT::Precision{0, 4, 0} /*a[ramp((((xo.outer*1048576) + (rv.outer*16384)) + (rv.inner*512)), 1, 512)]*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 64, PrecisionT::Precision{0, 4, 0} /*b[((((y.inner.outer*16384) + (y.inner.inner*2048)) + (rv.outer*32)) + rv.inner)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /**/);
              request.addOperand(sys->getAddress(xo_outer, 0, 80), 32768, PrecisionT::Precision{0, 16, 0} /**/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer*512), 1, 512)]*/);
              sys->sendRequest(request);
            }
          }
        }
        void* _3 = (void*) "compute.global[ramp(0, 1, 512)] = x512((int16)0)/*skip-init*/";
        #define max(a,b) ((a)>(b)?(a):(b))
        for (int32_t rv_outer_v = 64, rv_outer_v_cnt = 1; rv_outer_v >= 1; rv_outer_v -= max(rv_outer_v / 2, 1), ++rv_outer_v_cnt) {
        #undef max
          {
            Request request(Request::Type::BlockSend_Receive);
            request.addOperand(sys->getAddress(xo_outer, 0, 32), 32768, PrecisionT::Precision{0, 16, 0} /*compute.global[ramp(0, 1, 512)]*/);
            request.addOperand(sys->getAddress(xo_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer.v*512), 1, 512)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(xo_outer, 0, 32), 32768, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(xo_outer, 0, 32), 32768, PrecisionT::Precision{0, 16, 0} /*compute.global[ramp(0, 1, 512)]*/);
            request.addOperand(sys->getAddress(xo_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer.v*512), 1, 512)]*/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(xo_outer, 0, 0), 512, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(xo_outer, 0, 32), 512, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed compute_global
  // freed compute_global_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_gemm4_120_64_512__ = pimsim::registerFunc("gemm4_120_64_512", gemm4_120_64_512);

