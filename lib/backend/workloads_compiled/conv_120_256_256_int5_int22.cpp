// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t conv_120_256_256_int5_int22(System *sys) {
  void* _1 = nullptr;
  // int22 Conv2dOutput_rf[512], 0
  // int22 Conv2dOutput_repl_global[512], 768
  for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 98; ++ax0_ax1_fused_ax2_fused) {
    // cram-array axis
    {
      int32_t rc_outer = 0;
      void* _2 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256((int22)0)/*skip-init*/";
      for (int32_t ry = 0; ry < 3; ++ry) {
        for (int32_t rx = 0; rx < 3; ++rx) {
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 0), 256, PrecisionT::Precision{0, 5, 0} /*DRAM*/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 0), 256, PrecisionT::Precision{0, 5, 0} /*x[(((((((ax0.ax1.fused.ax2.fused/49)*20736) + (((ax0.ax1.fused.ax2.fused % 49)/7)*2304)) + (ry*2304)) + (rx*256)) + ((ax0.ax1.fused.ax2.fused % 7)*256)) + rc.outer)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowMul);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 56), 65536, PrecisionT::Precision{0, 10, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 0), 256, PrecisionT::Precision{0, 5, 0} /*x[(((((((ax0.ax1.fused.ax2.fused/49)*20736) + (((ax0.ax1.fused.ax2.fused % 49)/7)*2304)) + (ry*2304)) + (rx*256)) + ((ax0.ax1.fused.ax2.fused % 7)*256)) + rc.outer)]*/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 48), 65536, PrecisionT::Precision{0, 5, 0} /*w[ramp((((ry*196608) + (rx*65536)) + (rc.outer*256)), 1, 256)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 0), 65536, PrecisionT::Precision{0, 22, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 56), 65536, PrecisionT::Precision{0, 22, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 0), 65536, PrecisionT::Precision{0, 22, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
            sys->sendRequest(request);
          }
        }
      }
    }
    void* _3 = (void*) "Conv2dOutput.repl.global[ramp(0, 1, 256)] = x256((int22)0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t rc_outer_v = 256, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 24), 256, PrecisionT::Precision{0, 22, 0} /*Conv2dOutput.repl.global[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 0), 256, PrecisionT::Precision{0, 22, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 24), 256, PrecisionT::Precision{0, 22, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 24), 256, PrecisionT::Precision{0, 22, 0} /*Conv2dOutput.repl.global[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 0), 256, PrecisionT::Precision{0, 22, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 0), 256, PrecisionT::Precision{0, 22, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused, 0, 24), 256, PrecisionT::Precision{0, 22, 0} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  // freed Conv2dOutput_repl_global
  // freed Conv2dOutput_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_conv_120_256_256_int5_int22__ = pimsim::registerFunc("conv_120_256_256_int5_int22", conv_120_256_256_int5_int22);

