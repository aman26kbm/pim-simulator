// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

namespace e2e_poc {
int32_t tvmgen_default_fused_nn_conv2d_add_cast(System *sys) {
  void* _1 = nullptr;
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_outer_outer_fused = 0; ax0_ax1_fused_ax2_outer_outer_fused < 112; ++ax0_ax1_fused_ax2_outer_outer_fused) {
    // cram-array axis
    {
      int32_t ax2_outer_inner = 0;
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _2 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
        (void)_2;
        for (int32_t ry = 0; ry < 3; ++ry) {
          for (int32_t rx = 0; rx < 3; ++rx) {
            for (int32_t rc_inner = 0; rc_inner < 4; ++rc_inner) {
              {
                Request request(Request::Type::RowLoad);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[((((x256(((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*3712) + (ry*3712)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(64)) in (let rdiv = (ramp(0, 1, 256)/x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(64))) + x256((rx*64))) + x256((rc.outer*4))) + x256(rc.inner))]*/);
                sys->sendRequest(request);
              }
              (void)_1;
              {
                Request request(Request::Type::RowLoad);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[(x256(((((ry*12288) + (rx*4096)) + (rc.outer*256)) + (rc.inner*64))) + (let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64)))))]*/);
                sys->sendRequest(request);
              }
              (void)_1;
              {
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 112), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[((((x256(((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*3712) + (ry*3712)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(64)) in (let rdiv = (ramp(0, 1, 256)/x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(64))) + x256((rx*64))) + x256((rc.outer*4))) + x256(rc.inner))]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[(x256(((((ry*12288) + (rx*4096)) + (rc.outer*256)) + (rc.inner*64))) + (let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64)))))]*/);
                sys->sendRequest(request);
              }
              (void)_1;
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 112), 0, PrecisionT::Precision{0, 32, 0} /**/);
                sys->sendRequest(request);
              }
              (void)_1;
            }
          }
        }
      }
      void* _3 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      (void)_3;
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 16, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        (void)_1;
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        (void)_1;
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      (void)_1;
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      (void)_1;
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
      (void)_1;
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_e2e_poc_tvmgen_default_fused_nn_conv2d_add_cast__ = pimsim::registerFunc("e2e_poc_tvmgen_default_fused_nn_conv2d_add_cast", tvmgen_default_fused_nn_conv2d_add_cast);

} // e2e_poc