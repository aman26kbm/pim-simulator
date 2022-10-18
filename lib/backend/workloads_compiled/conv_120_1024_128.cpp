// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t conv_120_1024_128(System *sys) {
  void* _1 = nullptr;
  // int32_t Conv2dOutput_rf[512], 0
  // int32_t Conv2dOutput_repl_global[512], 512
  for (int32_t ax0_ax1_fused = 0; ax0_ax1_fused < 112; ++ax0_ax1_fused) {
    for (int32_t ax2 = 0; ax2 < 7; ++ax2) {
      // cram-array axis
      {
        int32_t ax3_outer = 0;
        // cram-array axis
        {
          int32_t rc_outer = 0;
          void* _2 = (void*) "Conv2dOutput.rf[ramp((rc.outer*128), 1, 128)] = x128(0)/*skip-init*/";
          for (int32_t ry = 0; ry < 3; ++ry) {
            for (int32_t rx = 0; rx < 3; ++rx) {
              for (int32_t rc_inner = 0; rc_inner < 2; ++rc_inner) {
                int32_t _3 = ax0_ax1_fused / 7;
                int32_t _4 = _3 * 41472;
                int32_t _5 = ry * 4608;
                int32_t _6 = _4 + _5;
                int32_t _7 = ax0_ax1_fused % 7;
                int32_t _8 = _7 * 4608;
                int32_t _9 = _6 + _8;
                int32_t _10 = ax2 * 512;
                int32_t _11 = _9 + _10;
                int32_t _12 = rx * 512;
                int32_t _13 = _11 + _12;
                int32_t _14 = rc_outer * 2;
                int32_t _15 = _13 + _14;
                int32_t _16 = _15 + rc_inner;
                int32_t _17 = _16 % 128;
                bool _18 = _17 == 0;
                if (_18) {
                  {
                    Request request(Request::Type::RowLoad_RF);
                    request.addOperand(ax0_ax1_fused * 32, 0, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
                    request.addOperand(sys->getAddress(ax0_ax1_fused, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
                    sys->sendRequest(request);
                  }
                }
                {
                  Request request(Request::Type::RowLoad);
                  request.addOperand(sys->getAddress(ax0_ax1_fused, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
                  request.addOperand(sys->getAddress(ax0_ax1_fused, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /*w[ramp((((((ry*786432) + (rx*262144)) + (rc.outer*1024)) + (rc.inner*512)) + (ax3.outer*128)), 1, 128)]*/);
                  sys->sendRequest(request);
                }
                {
                  Request request(Request::Type::RowMul_CRAM_RF);
                  request.addOperand(sys->getAddress(ax0_ax1_fused, 0, 36), 0, PrecisionT::Precision{0, 16, 0} /**/);
                  request.addOperand(sys->getAddress(ax0_ax1_fused, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /*w[ramp((((((ry*786432) + (rx*262144)) + (rc.outer*1024)) + (rc.inner*512)) + (ax3.outer*128)), 1, 128)]*/);
                  request.addOperand(ax0_ax1_fused * 32, 0, PrecisionT::Precision{0, 8, 0} /*x[((((((((ax0.ax1.fused/7)*41472) + (ry*4608)) + ((ax0.ax1.fused % 7)*4608)) + (ax2*512)) + (rx*512)) + (rc.outer*2)) + rc.inner)]*/);
                  sys->sendRequest(request);
                }
                {
                  Request request(Request::Type::RowAdd);
                  request.addOperand(sys->getAddress(ax0_ax1_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
                  request.addOperand(sys->getAddress(ax0_ax1_fused, 0, 36), 0, PrecisionT::Precision{0, 32, 0} /**/);
                  request.addOperand(sys->getAddress(ax0_ax1_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*128), 1, 128)]*/);
                  sys->sendRequest(request);
                }
              }
            }
          }
        }
        void* _19 = (void*) "Conv2dOutput.repl.global[ramp(0, 1, 128)] = x128(0)/*skip-init*/";
        #define max(a,b) ((a)>(b)?(a):(b))
        for (int32_t rc_outer_v = 256, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
        #undef max
          {
            Request request(Request::Type::BlockSend_Receive);
            request.addOperand(sys->getAddress(ax0_ax1_fused, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.repl.global[ramp(0, 1, 128)]*/);
            request.addOperand(sys->getAddress(ax0_ax1_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*128), 1, 128)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(ax0_ax1_fused, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.repl.global[ramp(0, 1, 128)]*/);
            request.addOperand(sys->getAddress(ax0_ax1_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*128), 1, 128)]*/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(ax0_ax1_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed Conv2dOutput_repl_global
  // freed Conv2dOutput_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_conv_120_1024_128__ = pimsim::registerFunc("conv_120_1024_128", conv_120_1024_128);

