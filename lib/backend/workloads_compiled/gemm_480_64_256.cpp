// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t gemm_480_64_256(System *sys) {
  void* _1 = nullptr;
  // int32_t compute_global_rf[1024], 0
  // int8_t a_global[256], 1024
  // int32_t compute_global[1024], 1280
  for (int32_t xo_y_outer_fused = 0; xo_y_outer_fused < 480; ++xo_y_outer_fused) {
    for (int32_t y_inner = 0; y_inner < 16; ++y_inner) {
      // cram-array axis
      {
        int32_t rv_outer = 0;
        void* _2 = (void*) "compute.global.rf[ramp((rv.outer*256), 1, 256)] = x256(0)/*skip-init*/";
        for (int32_t rv_inner = 0; rv_inner < 32; ++rv_inner) {
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(xo_y_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
            request.addOperand(sys->getAddress(xo_y_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
          int32_t _3 = xo_y_outer_fused & 1;
          int32_t _4 = _3 * 32768;
          int32_t _5 = y_inner * 2048;
          int32_t _6 = _4 + _5;
          int32_t _7 = rv_outer * 32;
          int32_t _8 = _6 + _7;
          int32_t _9 = _8 + rv_inner;
          int32_t _10 = _9 % 128;
          bool _11 = _10 == 0;
          if (_11) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_y_outer_fused * 32, 0, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_y_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(xo_y_outer_fused, 0, 72), 0, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(xo_y_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /*a.global[ramp(0, 1, 256)]*/);
            request.addOperand(xo_y_outer_fused * 32, 0, PrecisionT::Precision{0, 8, 0} /*b[((((tir.bitwise_and(xo.y.outer.fused, 1)*32768) + (y.inner*2048)) + (rv.outer*32)) + rv.inner)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(xo_y_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
            request.addOperand(sys->getAddress(xo_y_outer_fused, 0, 72), 0, PrecisionT::Precision{0, 32, 0} /**/);
            request.addOperand(sys->getAddress(xo_y_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*compute.global.rf[ramp((rv.outer*256), 1, 256)]*/);
            sys->sendRequest(request);
          }
        }
      }
      void* _12 = (void*) "compute.global[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rv_outer_v = 64, rv_outer_v_cnt = 1; rv_outer_v >= 1; rv_outer_v -= max(rv_outer_v / 2, 1), ++rv_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(xo_y_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /*compute.global[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(xo_y_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*compute.global.rf[ramp((rv.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(xo_y_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(xo_y_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /*compute.global[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(xo_y_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*compute.global.rf[ramp((rv.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(xo_y_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(xo_y_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed compute_global
  // freed a_global
  // freed compute_global_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_gemm_480_64_256__ = pimsim::registerFunc("gemm_480_64_256", gemm_480_64_256);

