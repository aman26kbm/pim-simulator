// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t conv_120_256_256(System *sys) {
  void* _1 = nullptr;
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput_repl_global[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 98; ++ax0_ax1_fused_ax2_fused) {
    // cram-array axis
    {
      int32_t rc_outer = 0;
      void* _2 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
      for (int32_t ry = 0; ry < 3; ++ry) {
        for (int32_t rx = 0; rx < 3; ++rx) {
          int32_t _3 = ax0_ax1_fused_ax2_fused / 49;
          int32_t _4 = _3 * 20736;
          int32_t _5 = ax0_ax1_fused_ax2_fused % 49;
          int32_t _6 = _5 / 7;
          int32_t _7 = _6 * 2304;
          int32_t _8 = _4 + _7;
          int32_t _9 = ry * 2304;
          int32_t _10 = _8 + _9;
          int32_t _11 = rx * 256;
          int32_t _12 = _10 + _11;
          int32_t _13 = ax0_ax1_fused_ax2_fused % 7;
          int32_t _14 = _13 * 256;
          int32_t _15 = _12 + _14;
          int32_t _16 = _15 + rc_outer;
          int32_t _17 = _16 % 128;
          bool _18 = _17 == 0;
          if (_18) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(ax0_ax1_fused_ax2_fused * 32, 256, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 0), 256, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 0), 256, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 64), 256, PrecisionT::Precision{0, 8, 0} /*w[ramp((((ry*196608) + (rx*65536)) + (rc.outer*256)), 1, 256)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 72), 256, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 64), 256, PrecisionT::Precision{0, 8, 0} /*w[ramp((((ry*196608) + (rx*65536)) + (rc.outer*256)), 1, 256)]*/);
            request.addOperand(ax0_ax1_fused_ax2_fused * 32, 256, PrecisionT::Precision{0, 8, 0} /*x[(((((((ax0.ax1.fused.ax2.fused/49)*20736) + (((ax0.ax1.fused.ax2.fused % 49)/7)*2304)) + (ry*2304)) + (rx*256)) + ((ax0.ax1.fused.ax2.fused % 7)*256)) + rc.outer)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 72), 256, PrecisionT::Precision{0, 32, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
            sys->sendRequest(request);
          }
        }
      }
    }
    void* _19 = (void*) "Conv2dOutput.repl.global[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t rc_outer_v = 256, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 32), 1, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.repl.global[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 0), 1, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 32), 1, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 32), 1, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.repl.global[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 0), 1, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 0), 1, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 32), 1, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  // freed Conv2dOutput_repl_global
  // freed Conv2dOutput_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_conv_120_256_256__ = pimsim::registerFunc("conv_120_256_256", conv_120_256_256);

