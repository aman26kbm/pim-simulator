// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t gemm_120_256_256_int8_int28(System *sys) {
  void* _1 = nullptr;
  // int28 compute_global_rf[6144], 0
  // int28 compute_global[6144], 8192
  for (int32_t xo_outer = 0; xo_outer < 120; ++xo_outer) {
    for (int32_t xo_inner = 0; xo_inner < 2; ++xo_inner) {
      for (int32_t y_outer = 0; y_outer < 2; ++y_outer) {
        // cram-array axis
        {
          int32_t y_inner_outer = 0;
          // cram-array axis
          {
            int32_t rv_outer = 0;
            for (int32_t y_c_init = 0; y_c_init < 8; ++y_c_init) {
              void* _2 = (void*) "compute.global.rf[ramp(((rv.outer*2048) + (y.c.init*256)), 1, 256)] = x256((int28)0)/*skip-init*/";
            }
            for (int32_t rv_inner = 0; rv_inner < 16; ++rv_inner) {
              for (int32_t y_c = 0; y_c < 8; ++y_c) {
                {
                  Request request(Request::Type::RowMul);
                  request.addOperand(sys->getAddress(xo_outer, 0, 520), 65536, PrecisionT::Precision{0, 16, 0} /**/);
                  request.addOperand(sys->getAddress(xo_outer, 0, 512), 32768, PrecisionT::Precision{0, 8, 0} /*a[ramp(((((xo.outer*1048576) + (xo.inner*524288)) + (rv.outer*4096)) + (rv.inner*256)), 1, 256)]*/);
                  request.addOperand(sys->getAddress(xo_outer, 0, 0), 256, PrecisionT::Precision{0, 8, 0} /*b[(((((y.outer*32768) + (y.inner.outer*16384)) + (y.c*2048)) + (rv.outer*16)) + rv.inner)]*/);
                  sys->sendRequest(request);
                }
                {
                  Request request(Request::Type::RowAdd);
                  request.addOperand(sys->getAddress(xo_outer, 0, 0), 65536, PrecisionT::Precision{0, 28, 0} /**/);
                  request.addOperand(sys->getAddress(xo_outer, 0, 520), 65536, PrecisionT::Precision{0, 28, 0} /**/);
                  request.addOperand(sys->getAddress(xo_outer, 0, 0), 32768, PrecisionT::Precision{0, 28, 0} /*compute.global.rf[ramp(((rv.outer*2048) + (y.c*256)), 1, 256)]*/);
                  sys->sendRequest(request);
                }
              }
            }
          }
          for (int32_t ax1_init = 0; ax1_init < 8; ++ax1_init) {
            void* _3 = (void*) "compute.global[ramp((ax1.init*256), 1, 256)] = x256((int28)0)/*skip-init*/";
          }
          #define max(a,b) ((a)>(b)?(a):(b))
          for (int32_t rv_outer_v = 128, rv_outer_v_cnt = 1; rv_outer_v >= 1; rv_outer_v -= max(rv_outer_v / 2, 1), ++rv_outer_v_cnt) {
          #undef max
            for (int32_t ax1 = 0; ax1 < 8; ++ax1) {
              {
                Request request(Request::Type::BlockSend_Receive);
                request.addOperand(sys->getAddress(xo_outer, 0, 256), 256, PrecisionT::Precision{0, 28, 0} /*compute.global[ramp((ax1*256), 1, 256)]*/);
                request.addOperand(sys->getAddress(xo_outer, 0, 0), 256, PrecisionT::Precision{0, 28, 0} /*compute.global.rf[ramp(((rv.outer.v*2048) + (ax1*256)), 1, 256)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(xo_outer, 0, 256), 512, PrecisionT::Precision{0, 28, 0} /**/);
                request.addOperand(sys->getAddress(xo_outer, 0, 256), 256, PrecisionT::Precision{0, 28, 0} /*compute.global[ramp((ax1*256), 1, 256)]*/);
                request.addOperand(sys->getAddress(xo_outer, 0, 0), 256, PrecisionT::Precision{0, 28, 0} /*compute.global.rf[ramp(((rv.outer.v*2048) + (ax1*256)), 1, 256)]*/);
                sys->sendRequest(request);
              }
            }
          }
          for (int32_t y_inner_inner = 0; y_inner_inner < 8; ++y_inner_inner) {
            {
              Request request(Request::Type::RowStore);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 256, PrecisionT::Precision{0, 28, 0} /*DRAM*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 256), 256, PrecisionT::Precision{0, 28, 0} /*CRAM*/);
              sys->sendRequest(request);
            }
          }
        }
      }
    }
  }
  // freed compute_global
  // freed compute_global_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_gemm_120_256_256_int8_int28__ = pimsim::registerFunc("gemm_120_256_256_int8_int28", gemm_120_256_256_int8_int28);

