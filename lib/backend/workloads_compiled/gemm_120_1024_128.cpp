// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t gemm_120_1024_128(System *sys) {
  void* _1 = nullptr;
  // int32_t compute_global_rf[512], 0
  // int8_t a_global[128], 512
  // int32_t compute_global[512], 640
  for (int32_t xo_outer = 0; xo_outer < 120; ++xo_outer) {
    for (int32_t xo_inner = 0; xo_inner < 4; ++xo_inner) {
      // cram-array axis
      {
        int32_t y_outer = 0;
        for (int32_t y_inner = 0; y_inner < 8; ++y_inner) {
          // cram-array axis
          {
            int32_t rv_outer = 0;
            void* _2 = (void*) "compute.global.rf[ramp((rv.outer*128), 1, 128)] = x128(0)/*skip-init*/";
            for (int32_t rv_inner = 0; rv_inner < 8; ++rv_inner) {
              {
                Request request(Request::Type::RowLoad);
                request.addOperand(sys->getAddress(xo_outer, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
                request.addOperand(sys->getAddress(xo_outer, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
                sys->sendRequest(request);
              }
              int32_t _3 = y_outer * 16384;
              int32_t _4 = y_inner * 2048;
              int32_t _5 = _3 + _4;
              int32_t _6 = rv_outer * 8;
              int32_t _7 = _5 + _6;
              int32_t _8 = _7 + rv_inner;
              int32_t _9 = _8 % 128;
              bool _10 = _9 == 0;
              if (_10) {
                {
                  Request request(Request::Type::RowLoad_RF);
                  request.addOperand(xo_outer * 32, 0, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
                  request.addOperand(sys->getAddress(xo_outer, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
                  sys->sendRequest(request);
                }
              }
              {
                Request request(Request::Type::RowMul_CRAM_RF);
                request.addOperand(sys->getAddress(xo_outer, 0, 36), 0, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(xo_outer, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /*a.global[ramp(0, 1, 128)]*/);
                request.addOperand(xo_outer * 32, 0, PrecisionT::Precision{0, 8, 0} /*b[((((y.outer*16384) + (y.inner*2048)) + (rv.outer*8)) + rv.inner)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(xo_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(xo_outer, 0, 36), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(xo_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*compute.global.rf[ramp((rv.outer*128), 1, 128)]*/);
                sys->sendRequest(request);
              }
            }
          }
          void* _11 = (void*) "compute.global[ramp(0, 1, 128)] = x128(0)/*skip-init*/";
          #define max(a,b) ((a)>(b)?(a):(b))
          for (int32_t rv_outer_v = 256, rv_outer_v_cnt = 1; rv_outer_v >= 1; rv_outer_v -= max(rv_outer_v / 2, 1), ++rv_outer_v_cnt) {
          #undef max
            {
              Request request(Request::Type::BlockSend_Receive);
              request.addOperand(sys->getAddress(xo_outer, 0, 20), 0, PrecisionT::Precision{0, 32, 0} /*compute.global[ramp(0, 1, 128)]*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*compute.global.rf[ramp((rv.outer.v*128), 1, 128)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(xo_outer, 0, 20), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(xo_outer, 0, 20), 0, PrecisionT::Precision{0, 32, 0} /*compute.global[ramp(0, 1, 128)]*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*compute.global.rf[ramp((rv.outer.v*128), 1, 128)]*/);
              sys->sendRequest(request);
            }
          }
          {
            Request request(Request::Type::RowStore);
            request.addOperand(sys->getAddress(xo_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
            request.addOperand(sys->getAddress(xo_outer, 0, 20), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
            sys->sendRequest(request);
          }
        }
      }
    }
  }
  // freed compute_global
  // freed a_global
  // freed compute_global_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_gemm_120_1024_128__ = pimsim::registerFunc("gemm_120_1024_128", gemm_120_1024_128);

