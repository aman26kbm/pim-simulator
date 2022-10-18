// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t conv_480_64_256(System *sys) {
  void* _1 = nullptr;
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput_repl_global[1024], 1024
  for (int32_t ax0_outer = 0; ax0_outer < 2; ++ax0_outer) {
    for (int32_t ax0_inner_ax1_fused_ax2_fused = 0; ax0_inner_ax1_fused_ax2_fused < 448; ++ax0_inner_ax1_fused_ax2_fused) {
      for (int32_t ax3_outer = 0; ax3_outer < 2; ++ax3_outer) {
        // cram-array axis
        {
          int32_t rc_outer = 0;
          void* _2 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
          for (int32_t ry = 0; ry < 3; ++ry) {
            for (int32_t rx = 0; rx < 3; ++rx) {
              for (int32_t rc_inner = 0; rc_inner < 8; ++rc_inner) {
                int32_t _3 = ax0_outer * 368640;
                int32_t _4 = ax0_inner_ax1_fused_ax2_fused / 56;
                int32_t _5 = _4 * 46080;
                int32_t _6 = _3 + _5;
                int32_t _7 = ax0_inner_ax1_fused_ax2_fused % 56;
                int32_t _8 = _7 / 7;
                int32_t _9 = _8 * 4608;
                int32_t _10 = _6 + _9;
                int32_t _11 = ry * 4608;
                int32_t _12 = _10 + _11;
                int32_t _13 = rx * 512;
                int32_t _14 = _12 + _13;
                int32_t _15 = ax0_inner_ax1_fused_ax2_fused % 7;
                int32_t _16 = _15 * 512;
                int32_t _17 = _14 + _16;
                int32_t _18 = rc_outer * 8;
                int32_t _19 = _17 + _18;
                int32_t _20 = _19 + rc_inner;
                int32_t _21 = _20 % 128;
                bool _22 = _21 == 0;
                if (_22) {
                  {
                    Request request(Request::Type::RowLoad_RF);
                    request.addOperand(ax0_inner_ax1_fused_ax2_fused * 32, 0, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
                    request.addOperand(sys->getAddress(ax0_inner_ax1_fused_ax2_fused, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
                    sys->sendRequest(request);
                  }
                }
                {
                  Request request(Request::Type::RowLoad);
                  request.addOperand(sys->getAddress(ax0_inner_ax1_fused_ax2_fused, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
                  request.addOperand(sys->getAddress(ax0_inner_ax1_fused_ax2_fused, 0, 64), 0, PrecisionT::Precision{0, 8, 0} /*w[ramp((((((ry*786432) + (rx*262144)) + (rc.outer*4096)) + (rc.inner*512)) + (ax3.outer*256)), 1, 256)]*/);
                  sys->sendRequest(request);
                }
                {
                  Request request(Request::Type::RowMul_CRAM_RF);
                  request.addOperand(sys->getAddress(ax0_inner_ax1_fused_ax2_fused, 0, 72), 0, PrecisionT::Precision{0, 16, 0} /**/);
                  request.addOperand(sys->getAddress(ax0_inner_ax1_fused_ax2_fused, 0, 64), 0, PrecisionT::Precision{0, 8, 0} /*w[ramp((((((ry*786432) + (rx*262144)) + (rc.outer*4096)) + (rc.inner*512)) + (ax3.outer*256)), 1, 256)]*/);
                  request.addOperand(ax0_inner_ax1_fused_ax2_fused * 32, 0, PrecisionT::Precision{0, 8, 0} /*x[((((((((ax0.outer*368640) + ((ax0.inner.ax1.fused.ax2.fused/56)*46080)) + (((ax0.inner.ax1.fused.ax2.fused % 56)/7)*4608)) + (ry*4608)) + (rx*512)) + ((ax0.inner.ax1.fused.ax2.fused % 7)*512)) + (rc.outer*8)) + rc.inner)]*/);
                  sys->sendRequest(request);
                }
                {
                  Request request(Request::Type::RowAdd);
                  request.addOperand(sys->getAddress(ax0_inner_ax1_fused_ax2_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
                  request.addOperand(sys->getAddress(ax0_inner_ax1_fused_ax2_fused, 0, 72), 0, PrecisionT::Precision{0, 32, 0} /**/);
                  request.addOperand(sys->getAddress(ax0_inner_ax1_fused_ax2_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                  sys->sendRequest(request);
                }
              }
            }
          }
        }
        void* _23 = (void*) "Conv2dOutput.repl.global[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
        #define max(a,b) ((a)>(b)?(a):(b))
        for (int32_t rc_outer_v = 64, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
        #undef max
          {
            Request request(Request::Type::BlockSend_Receive);
            request.addOperand(sys->getAddress(ax0_inner_ax1_fused_ax2_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.repl.global[ramp(0, 1, 256)]*/);
            request.addOperand(sys->getAddress(ax0_inner_ax1_fused_ax2_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(ax0_inner_ax1_fused_ax2_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
            request.addOperand(sys->getAddress(ax0_inner_ax1_fused_ax2_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.repl.global[ramp(0, 1, 256)]*/);
            request.addOperand(sys->getAddress(ax0_inner_ax1_fused_ax2_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(ax0_inner_ax1_fused_ax2_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(ax0_inner_ax1_fused_ax2_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed Conv2dOutput_repl_global
  // freed Conv2dOutput_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_conv_480_64_256__ = pimsim::registerFunc("conv_480_64_256", conv_480_64_256);

