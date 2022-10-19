// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t gemm4_120_256_256(System *sys) {
  void* _1 = nullptr;
  // int16_t compute_global_rf[512], 0
  // int16_t compute_global[512], 512
  for (int32_t xo_outer = 0; xo_outer < 120; ++xo_outer) {
    for (int32_t xo_inner = 0; xo_inner < 2; ++xo_inner) {
      // cram-array axis
      {
        int32_t y_outer = 0;
        for (int32_t y_inner = 0; y_inner < 16; ++y_inner) {
          // cram-array axis
          {
            int32_t rv_outer = 0;
            void* _2 = (void*) "compute.global.rf[ramp((rv.outer*256), 1, 256)] = x256((int16)0)/*skip-init*/";
            for (int32_t rv_inner = 0; rv_inner < 16; ++rv_inner) {
              int32_t _3 = y_outer * 32768;
              int32_t _4 = y_inner * 2048;
              int32_t _5 = _3 + _4;
              int32_t _6 = rv_outer * 16;
              int32_t _7 = _5 + _6;
              int32_t _8 = _7 + rv_inner;
              int32_t _9 = _8 % 256;
              bool _10 = _9 == 0;
              if (_10) {
                {
                  Request request(Request::Type::RowLoad_RF);
                  request.addOperand(xo_outer * 32, 256, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
                  request.addOperand(sys->getAddress(xo_outer, 0, 0), 256, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
                  sys->sendRequest(request);
                }
              }
              {
                Request request(Request::Type::RowLoad);
                request.addOperand(sys->getAddress(xo_outer, 0, 0), 256, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
                request.addOperand(sys->getAddress(xo_outer, 0, 32), 256, PrecisionT::Precision{0, 4, 0} /*a[ramp(((((xo.outer*1048576) + (xo.inner*524288)) + (rv.outer*4096)) + (rv.inner*256)), 1, 256)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul_CRAM_RF);
                request.addOperand(sys->getAddress(xo_outer, 0, 40), 256, PrecisionT::Precision{0, 8, 0} /**/);
                request.addOperand(sys->getAddress(xo_outer, 0, 32), 256, PrecisionT::Precision{0, 4, 0} /*a[ramp(((((xo.outer*1048576) + (xo.inner*524288)) + (rv.outer*4096)) + (rv.inner*256)), 1, 256)]*/);
                request.addOperand(xo_outer * 32, 256, PrecisionT::Precision{0, 4, 0} /*b[((((y.outer*32768) + (y.inner*2048)) + (rv.outer*16)) + rv.inner)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(xo_outer, 0, 0), 256, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(xo_outer, 0, 40), 256, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(xo_outer, 0, 0), 256, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer*256), 1, 256)]*/);
                sys->sendRequest(request);
              }
            }
          }
          void* _11 = (void*) "compute.global[ramp(0, 1, 256)] = x256((int16)0)/*skip-init*/";
          #define max(a,b) ((a)>(b)?(a):(b))
          for (int32_t rv_outer_v = 128, rv_outer_v_cnt = 1; rv_outer_v >= 1; rv_outer_v -= max(rv_outer_v / 2, 1), ++rv_outer_v_cnt) {
          #undef max
            {
              Request request(Request::Type::BlockSend_Receive);
              request.addOperand(sys->getAddress(xo_outer, 0, 16), 2, PrecisionT::Precision{0, 16, 0} /*compute.global[ramp(0, 1, 256)]*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 2, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer.v*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(xo_outer, 0, 16), 2, PrecisionT::Precision{0, 16, 0} /**/);
              request.addOperand(sys->getAddress(xo_outer, 0, 16), 2, PrecisionT::Precision{0, 16, 0} /*compute.global[ramp(0, 1, 256)]*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 2, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer.v*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
          }
          {
            Request request(Request::Type::RowStore);
            request.addOperand(sys->getAddress(xo_outer, 0, 0), 2, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            request.addOperand(sys->getAddress(xo_outer, 0, 16), 2, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
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

static __attribute__((unused)) Registry::Entry &_gemm4_120_256_256__ = pimsim::registerFunc("gemm4_120_256_256", gemm4_120_256_256);

