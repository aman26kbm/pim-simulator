// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

namespace e2e_poc {
int32_t tvmgen_default_fused_layout_transform(System *sys) {
  void* _1 = nullptr;
  for (int32_t ax1 = 0; ax1 < 56; ++ax1) {
    for (int32_t ax2 = 0; ax2 < 56; ++ax2) {
      for (int32_t ax3 = 0; ax3 < 64; ++ax3) {
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); // src
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); // dst
          sys->sendRequest(request);
        }
      }
    }
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_e2e_poc_tvmgen_default_fused_layout_transform__ = pimsim::registerFunc("e2e_poc_tvmgen_default_fused_layout_transform", tvmgen_default_fused_layout_transform);

int32_t tvmgen_default_fused_nn_conv2d_cast(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)430592;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_outer_outer_fused = 0; ax0_ax1_fused_ax2_outer_outer_fused < 112; ++ax0_ax1_fused_ax2_outer_outer_fused) {
    // cram-array axis
    {
      int32_t ax2_outer_inner = 0;
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
        for (int32_t ry = 0; ry < 3; ++ry) {
          for (int32_t rx = 0; rx < 3; ++rx) {
            for (int32_t rc_inner = 0; rc_inner < 2; ++rc_inner) {
              {
                Request request(Request::Type::RowLoad);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 1792, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 1792, PrecisionT::Precision{0, 8, 0} /*PaddedInput[((((x256(((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*3712) + (ry*3712)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(64)) in (let rdiv = (ramp(0, 1, 256)/x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(64))) + x256((rx*64))) + x256((rc.outer*2))) + x256(rc.inner))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowShift);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 1792, PrecisionT::Precision{0, 8, 0} /*Unaligned Shift*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 1792, PrecisionT::Precision{0, 8, 0} /*Unaligned Shift*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowShift);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 8192, PrecisionT::Precision{0, 8, 0} /*Unaligned Shift*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 8192, PrecisionT::Precision{0, 8, 0} /*Unaligned Shift*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 256, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 1792, PrecisionT::Precision{0, 8, 0} /*PaddedInput[((((x256(((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*3712) + (ry*3712)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(64)) in (let rdiv = (ramp(0, 1, 256)/x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(64))) + x256((rx*64))) + x256((rc.outer*2))) + x256(rc.inner))]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 8192, PrecisionT::Precision{0, 8, 0} /*placeholder[(x256(((((ry*12288) + (rx*4096)) + (rc.outer*128)) + (rc.inner*64))) + (let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64)))))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 8192, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 256, PrecisionT::Precision{0, 32, 0} /**/);
                sys->sendRequest(request);
              }
            }
          }
        }
      }
      void* _7 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 32, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  int32_t _8 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _9 = _8 != 0;
  if (_9) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_e2e_poc_tvmgen_default_fused_nn_conv2d_cast__ = pimsim::registerFunc("e2e_poc_tvmgen_default_fused_nn_conv2d_cast", tvmgen_default_fused_nn_conv2d_cast);

int32_t tvmgen_default_fused_layout_transform_1(System *sys) {
  void* _1 = nullptr;
  for (int32_t ax1 = 0; ax1 < 64; ++ax1) {
    for (int32_t ax2 = 0; ax2 < 56; ++ax2) {
      for (int32_t ax3 = 0; ax3 < 56; ++ax3) {
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); // src
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); // dst
          sys->sendRequest(request);
        }
      }
    }
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_e2e_poc_tvmgen_default_fused_layout_transform_1__ = pimsim::registerFunc("e2e_poc_tvmgen_default_fused_layout_transform_1", tvmgen_default_fused_layout_transform_1);

} // e2e_poc