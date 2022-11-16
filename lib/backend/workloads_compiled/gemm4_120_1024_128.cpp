// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t gemm4_120_1024_128(System *sys) {
  void* _1 = nullptr;
  // int16_t compute_global_rf[256], 0
  // int16_t compute_global[256], 256
  for (int32_t xo_outer = 0; xo_outer < 120; ++xo_outer) {
    for (int32_t xo_inner = 0; xo_inner < 4; ++xo_inner) {
      // cram-array axis
      {
        int32_t y_outer = 0;
        for (int32_t y_inner_inner = 0; y_inner_inner < 8; ++y_inner_inner) {
          // cram-array axis
          {
            int32_t rv_outer = 0;
            void* _2 = (void*) "compute.global.rf[ramp((rv.outer*128), 1, 128)] = x128((int16)0)/*skip-init*/";
            for (int32_t rv_inner = 0; rv_inner < 2; ++rv_inner) {
              {
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(xo_outer, 0, 20), 524288, PrecisionT::Precision{0, 8, 0} /**/);
                request.addOperand(sys->getAddress(xo_outer, 0, 16), 524288, PrecisionT::Precision{0, 4, 0} /*a[ramp(((((xo.outer*1048576) + (xo.inner*262144)) + (rv.outer*256)) + (rv.inner*128)), 1, 128)]*/);
                request.addOperand(sys->getAddress(xo_outer, 0, 0), 4096, PrecisionT::Precision{0, 4, 0} /*b[((((y.outer*16384) + (y.inner.inner*2048)) + (rv.outer*2)) + rv.inner)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(xo_outer, 0, 0), 524288, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(xo_outer, 0, 20), 524288, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(xo_outer, 0, 0), 524288, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer*128), 1, 128)]*/);
                sys->sendRequest(request);
              }
            }
          }
          void* _3 = (void*) "compute.global[ramp(0, 1, 128)] = x128((int16)0)/*skip-init*/";
          #define max(a,b) ((a)>(b)?(a):(b))
          for (int32_t rv_outer_v = 1024, rv_outer_v_cnt = 1; rv_outer_v >= 1; rv_outer_v -= max(rv_outer_v / 2, 1), ++rv_outer_v_cnt) {
          #undef max
            {
              Request request(Request::Type::BlockSend_Receive);
              request.addOperand(sys->getAddress(xo_outer, 0, 8), 524288, PrecisionT::Precision{0, 16, 0} /*compute.global[ramp(0, 1, 128)]*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 524288, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer.v*128), 1, 128)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(xo_outer, 0, 8), 524288, PrecisionT::Precision{0, 16, 0} /**/);
              request.addOperand(sys->getAddress(xo_outer, 0, 8), 524288, PrecisionT::Precision{0, 16, 0} /*compute.global[ramp(0, 1, 128)]*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 524288, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer.v*128), 1, 128)]*/);
              sys->sendRequest(request);
            }
          }
          {
            Request request(Request::Type::RowStore);
            request.addOperand(sys->getAddress(xo_outer, 0, 0), 512, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            request.addOperand(sys->getAddress(xo_outer, 0, 8), 512, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
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

static __attribute__((unused)) Registry::Entry &_gemm4_120_1024_128__ = pimsim::registerFunc("gemm4_120_1024_128", gemm4_120_1024_128);

