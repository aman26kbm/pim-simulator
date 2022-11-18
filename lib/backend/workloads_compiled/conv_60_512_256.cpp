// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t conv_60_512_256(System *sys) {
  void* _1 = nullptr;
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput_repl_global[1024], 1024
  for (int32_t ax0 = 0; ax0 < 2; ++ax0) {
    for (int32_t ax1_ax2_fused = 0; ax1_ax2_fused < 49; ++ax1_ax2_fused) {
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _2 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
        for (int32_t ry = 0; ry < 3; ++ry) {
          for (int32_t rx = 0; rx < 3; ++rx) {
            bool _3 = rc_outer < 256;
            if (_3) {
              {
                Request request(Request::Type::RowLoad);
                request.addOperand(sys->getAddress(ax1_ax2_fused, 0, 0), 512, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
                request.addOperand(sys->getAddress(ax1_ax2_fused, 0, 0), 512, PrecisionT::Precision{0, 8, 0} /*x[((((((ax0*20736) + ((ax1.ax2.fused/7)*2304)) + (ry*2304)) + (rx*256)) + ((ax1.ax2.fused % 7)*256)) + rc.outer)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(ax1_ax2_fused, 0, 72), 131072, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(ax1_ax2_fused, 0, 0), 512, PrecisionT::Precision{0, 8, 0} /*x[((((((ax0*20736) + ((ax1.ax2.fused/7)*2304)) + (ry*2304)) + (rx*256)) + ((ax1.ax2.fused % 7)*256)) + rc.outer)]*/);
                request.addOperand(sys->getAddress(ax1_ax2_fused, 0, 64), 131072, PrecisionT::Precision{0, 8, 0} /*w[ramp((((ry*196608) + (rx*65536)) + (rc.outer*256)), 1, 256)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax1_ax2_fused, 0, 0), 131072, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax1_ax2_fused, 0, 72), 131072, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax1_ax2_fused, 0, 0), 131072, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                sys->sendRequest(request);
              }
            }
          }
        }
      }
      void* _4 = (void*) "Conv2dOutput.repl.global[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 512, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax1_ax2_fused, 0, 32), 131072, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.repl.global[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax1_ax2_fused, 0, 0), 131072, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax1_ax2_fused, 0, 32), 131072, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax1_ax2_fused, 0, 32), 131072, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.repl.global[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax1_ax2_fused, 0, 0), 131072, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax1_ax2_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax1_ax2_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed Conv2dOutput_repl_global
  // freed Conv2dOutput_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_conv_60_512_256__ = pimsim::registerFunc("conv_60_512_256", conv_60_512_256);

