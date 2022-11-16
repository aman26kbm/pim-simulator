// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t gemm4_60_512_256(System *sys) {
  void* _1 = nullptr;
  // int16_t compute_global_rf[512], 0
  // int16_t compute_global[512], 512
  for (int32_t xo_outer = 0; xo_outer < 60; ++xo_outer) {
    for (int32_t xo_inner = 0; xo_inner < 4; ++xo_inner) {
      // cram-array axis
      {
        int32_t y_outer = 0;
        for (int32_t y_inner_inner = 0; y_inner_inner < 8; ++y_inner_inner) {
          // cram-array axis
          {
            int32_t rv_outer = 0;
            void* _2 = (void*) "compute.global.rf[ramp((rv.outer*256), 1, 256)] = x256((int16)0)/*skip-init*/";
            for (int32_t rv_inner = 0; rv_inner < 4; ++rv_inner) {
              {
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(xo_outer, 0, 40), 524288, PrecisionT::Precision{0, 8, 0} /**/);
                request.addOperand(sys->getAddress(xo_outer, 0, 32), 524288, PrecisionT::Precision{0, 4, 0} /*a[ramp(((((xo.outer*2097152) + (xo.inner*524288)) + (rv.outer*1024)) + (rv.inner*256)), 1, 256)]*/);
                request.addOperand(sys->getAddress(xo_outer, 0, 0), 2048, PrecisionT::Precision{0, 4, 0} /*b[((((y.outer*16384) + (y.inner.inner*2048)) + (rv.outer*4)) + rv.inner)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(xo_outer, 0, 0), 524288, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(xo_outer, 0, 40), 524288, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(xo_outer, 0, 0), 524288, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer*256), 1, 256)]*/);
                sys->sendRequest(request);
              }
            }
          }
          void* _3 = (void*) "compute.global[ramp(0, 1, 256)] = x256((int16)0)/*skip-init*/";
          #define max(a,b) ((a)>(b)?(a):(b))
          for (int32_t rv_outer_v = 512, rv_outer_v_cnt = 1; rv_outer_v >= 1; rv_outer_v -= max(rv_outer_v / 2, 1), ++rv_outer_v_cnt) {
          #undef max
            {
              Request request(Request::Type::BlockSend_Receive);
              request.addOperand(sys->getAddress(xo_outer, 0, 16), 524288, PrecisionT::Precision{0, 16, 0} /*compute.global[ramp(0, 1, 256)]*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 524288, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer.v*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(xo_outer, 0, 16), 524288, PrecisionT::Precision{0, 16, 0} /**/);
              request.addOperand(sys->getAddress(xo_outer, 0, 16), 524288, PrecisionT::Precision{0, 16, 0} /*compute.global[ramp(0, 1, 256)]*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 524288, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer.v*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
          }
          {
            Request request(Request::Type::RowStore);
            request.addOperand(sys->getAddress(xo_outer, 0, 0), 1024, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            request.addOperand(sys->getAddress(xo_outer, 0, 16), 1024, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
            sys->sendRequest(request);
          }
        }
      }
    }
  }
  // freed compute_global
  // freed compute_global_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_gemm4_60_512_256__ = pimsim::registerFunc("gemm4_60_512_256", gemm4_60_512_256);

