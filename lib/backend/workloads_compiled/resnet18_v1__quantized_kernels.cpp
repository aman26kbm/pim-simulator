// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

namespace resnet18_v1__quantized {
int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__2(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)131072;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_outer_fused = 0; ax0_ax1_fused_ax2_outer_fused < 98; ++ax0_ax1_fused_ax2_outer_fused) {
    for (int32_t ax2_inner = 0; ax2_inner < 2; ++ax2_inner) {
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
        for (int32_t ry = 0; ry < 3; ++ry) {
          for (int32_t rx = 0; rx < 3; ++rx) {
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 16, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 256, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp((((ry*196608) + (rx*65536)) + (rc.outer*256)), 1, 256)]*/);
              request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 256, PrecisionT::Precision{0, 8, 0} /*PaddedInput[(((((((ax0.ax1.fused.ax2.outer.fused/7)*4096) + (ry*4096)) + ((ax0.ax1.fused.ax2.outer.fused % 7)*512)) + (ax2.inner*256)) + (rx*256)) + rc.outer)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
      void* _7 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 256, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 1, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 1, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 192), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 144), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 192), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 144), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 72), 1, PrecisionT::Precision{0, 40, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 1, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(((ax0.ax1.fused.ax2.outer.fused*512) + (ax2.inner*256)), 1, 256)]*/);
        request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 64, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 72), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 1, PrecisionT::Precision{0, 64, 0} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 1, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
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

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__2__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__2", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__2);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_3(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)215296;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_outer_outer_fused = 0; ax0_ax1_fused_ax2_outer_outer_fused < 28; ++ax0_ax1_fused_ax2_outer_outer_fused) {
    // cram-array axis
    {
      int32_t ax2_outer_inner = 0;
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
        for (int32_t ry = 0; ry < 3; ++ry) {
          for (int32_t rx = 0; rx < 3; ++rx) {
            for (int32_t rc_inner = 0; rc_inner < 4; ++rc_inner) {
              {
                Request request(Request::Type::RowMul_CRAM_RF);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 224, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 224, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(((((ry*98304) + (rx*32768)) + (rc.outer*1024)) + (rc.inner*256)), 1, 256)]*/);
                request.addOperand(ax0_ax1_fused_ax2_outer_outer_fused * 32, 224, PrecisionT::Precision{0, 8, 0} /*PaddedInput[(((((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*7424) + (ry*3712)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256)) + (rx*128)) + (rc.outer*4)) + rc.inner)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 224, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
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
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 7, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
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

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_3__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_3", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_3);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_2(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)230400;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_outer_outer_fused = 0; ax0_ax1_fused_ax2_outer_outer_fused < 56; ++ax0_ax1_fused_ax2_outer_outer_fused) {
    // cram-array axis
    {
      int32_t ax2_outer_inner = 0;
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
        for (int32_t ry = 0; ry < 3; ++ry) {
          for (int32_t rx = 0; rx < 3; ++rx) {
            for (int32_t rc_inner = 0; rc_inner < 4; ++rc_inner) {
              {
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 224, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 224, PrecisionT::Precision{0, 8, 0} /*PaddedInput[((((x256(((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*3840) + (ry*3840)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(128)) in (let rdiv = (ramp(0, 1, 256)/x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(128))) + x256((rx*128))) + x256((rc.outer*4))) + x256(rc.inner))]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 224, PrecisionT::Precision{0, 8, 0} /*placeholder[(x256(((((ry*49152) + (rx*16384)) + (rc.outer*512)) + (rc.inner*128))) + (let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128)))))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 224, PrecisionT::Precision{0, 32, 0} /**/);
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
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 7, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
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

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_2__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_2", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_2);

int32_t tvmgen_default_fused_layout_transform_nn_batch_flatten_cast(System *sys) {
  void* _1 = nullptr;
  for (int32_t ax1_outer = 0; ax1_outer < 32; ++ax1_outer) {
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(0, 0, 0), 1, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(0, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_layout_transform_nn_batch_flatten_cast__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_layout_transform_nn_batch_flatten_cast", tvmgen_default_fused_layout_transform_nn_batch_flatten_cast);

int32_t tvmgen_default_fused_cast_2(System *sys) {
  void* _1 = nullptr;
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_cast_2__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_cast_2", tvmgen_default_fused_cast_2);

int32_t tvmgen_default_fused_divide_round_cast_clip_cast_cast_layout_transform(System *sys) {
  void* _1 = nullptr;
  for (int32_t ax1_outer = 0; ax1_outer < 112; ++ax1_outer) {
    // cram-array axis
    {
      int32_t ax1_inner_ax2_outer_fused = 0;
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 8, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 8, PrecisionT::Precision{1, 23, 8} /*placeholder[(((((let rmod = (ramp(0, 1, 168) % x168(3)) in select(((x168((bool)1) && (x168(0) <= rmod)) || (x168((bool)0) && (rmod <= x168(0)))), rmod, (rmod + x168(3))))*x168(50176)) + x168((ax1.outer*448))) + x168((ax1.inner.ax2.outer.fused*56))) + (let rmod = (ramp(0, 1, 168) % x168(3)) in (let rdiv = (ramp(0, 1, 168)/x168(3)) in select(((x168((bool)1) && (x168(0) <= rmod)) || (x168((bool)0) && (rmod <= x168(0)))), rdiv, (rdiv - x168(1))))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 8, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 8, PrecisionT::Precision{1, 23, 8} /*placeholder[(((((let rmod = (ramp(0, 1, 168) % x168(3)) in select(((x168((bool)1) && (x168(0) <= rmod)) || (x168((bool)0) && (rmod <= x168(0)))), rmod, (rmod + x168(3))))*x168(50176)) + x168((ax1.outer*448))) + x168((ax1.inner.ax2.outer.fused*56))) + (let rmod = (ramp(0, 1, 168) % x168(3)) in (let rdiv = (ramp(0, 1, 168)/x168(3)) in select(((x168((bool)1) && (x168(0) <= rmod)) || (x168((bool)0) && (rmod <= x168(0)))), rdiv, (rdiv - x168(1))))))]*/);
        request.addOperand(ax1_outer * 32, 8, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 8, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 8, PrecisionT::Precision{1, 23, 8} /*placeholder[(((((let rmod = (ramp(0, 1, 168) % x168(3)) in select(((x168((bool)1) && (x168(0) <= rmod)) || (x168((bool)0) && (rmod <= x168(0)))), rmod, (rmod + x168(3))))*x168(50176)) + x168((ax1.outer*448))) + x168((ax1.inner.ax2.outer.fused*56))) + (let rmod = (ramp(0, 1, 168) % x168(3)) in (let rdiv = (ramp(0, 1, 168)/x168(3)) in select(((x168((bool)1) && (x168(0) <= rmod)) || (x168((bool)0) && (rmod <= x168(0)))), rdiv, (rdiv - x168(1))))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 8, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 8, PrecisionT::Precision{1, 23, 8} /*placeholder[(((((let rmod = (ramp(0, 1, 168) % x168(3)) in select(((x168((bool)1) && (x168(0) <= rmod)) || (x168((bool)0) && (rmod <= x168(0)))), rmod, (rmod + x168(3))))*x168(50176)) + x168((ax1.outer*448))) + x168((ax1.inner.ax2.outer.fused*56))) + (let rmod = (ramp(0, 1, 168) % x168(3)) in (let rdiv = (ramp(0, 1, 168)/x168(3)) in select(((x168((bool)1) && (x168(0) <= rmod)) || (x168((bool)0) && (rmod <= x168(0)))), rdiv, (rdiv - x168(1))))))]*/);
        request.addOperand(ax1_outer * 32, 8, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax1_outer, 0, 42), 8, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 8, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 8, PrecisionT::Precision{1, 23, 8} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax1_outer, 0, 21), 8, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 42), 8, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(ax1_outer, 0, -1), 8, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax1_outer, 0, 10), 8, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 21), 8, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax1_outer, 0, -1), 8, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 8, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 10), 8, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_divide_round_cast_clip_cast_cast_layout_transform__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_divide_round_cast_clip_cast_cast_layout_transform", tvmgen_default_fused_divide_round_cast_clip_cast_cast_layout_transform);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1(System *sys) {
  void* _1 = nullptr;
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_outer_outer_fused = 0; ax0_ax1_fused_ax2_outer_outer_fused < 28; ++ax0_ax1_fused_ax2_outer_outer_fused) {
    // cram-array axis
    {
      int32_t ax2_outer_inner = 0;
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _2 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
        for (int32_t rc_inner = 0; rc_inner < 4; ++rc_inner) {
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 224, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 224, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(((rc.outer*1024) + (rc.inner*256)), 1, 256)]*/);
            request.addOperand(ax0_ax1_fused_ax2_outer_outer_fused * 32, 224, PrecisionT::Precision{0, 8, 0} /*placeholder[(((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*7168) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256)) + (rc.outer*4)) + rc.inner)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 224, PrecisionT::Precision{0, 32, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
            sys->sendRequest(request);
          }
        }
      }
      void* _3 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 32, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 7, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(((ax0.ax1.fused.ax2.outer.outer.fused*1792) + (ax2.outer.inner*256)), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast(System *sys) {
  void* _1 = nullptr;
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_outer_outer_fused = 0; ax0_ax1_fused_ax2_outer_outer_fused < 56; ++ax0_ax1_fused_ax2_outer_outer_fused) {
    // cram-array axis
    {
      int32_t ax2_outer_inner = 0;
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _2 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
        for (int32_t rc_inner = 0; rc_inner < 2; ++rc_inner) {
          {
            Request request(Request::Type::RowMul);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 224, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 224, PrecisionT::Precision{0, 8, 0} /*placeholder[(((x256((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*7168) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(128)) in (let rdiv = (ramp(0, 1, 256)/x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(128))) + x256((rc.outer*2))) + x256(rc.inner))]*/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 224, PrecisionT::Precision{0, 8, 0} /*placeholder[(x256(((rc.outer*256) + (rc.inner*128))) + (let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128)))))]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 224, PrecisionT::Precision{0, 32, 0} /**/);
            sys->sendRequest(request);
          }
        }
      }
      void* _3 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 32, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 7, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(((ax0.ax1.fused.ax2.outer.outer.fused*1792) + (ax2.outer.inner*256)), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast);

int32_t tvmgen_default_fused_nn_global_avg_pool2d_cast(System *sys) {
  void* _1 = nullptr;
  // int32_t tensor[1024], 0
  for (int32_t ax3_outer = 0; ax3_outer < 2; ++ax3_outer) {
    void* _2 = (void*) "tensor[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
    for (int32_t rv0 = 0; rv0 < 7; ++rv0) {
      for (int32_t rv1 = 0; rv1 < 7; ++rv1) {
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(ax3_outer, 0, 0), 1, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(ax3_outer, 0, 32), 1, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((((rv0*3584) + (rv1*512)) + (ax3.outer*256)), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax3_outer, 0, 0), 1, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax3_outer, 0, 0), 1, PrecisionT::Precision{0, 32, 0} /*tensor[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax3_outer, 0, 32), 1, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((((rv0*3584) + (rv1*512)) + (ax3.outer*256)), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(ax3_outer, 0, 32), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax3_outer, 0, 0), 1, PrecisionT::Precision{0, 32, 0} /*tensor[ramp(0, 1, 256)]*/);
      request.addOperand(ax3_outer * 32, 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(ax3_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax3_outer, 0, 32), 1, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  // freed tensor
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_global_avg_pool2d_cast__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_global_avg_pool2d_cast", tvmgen_default_fused_nn_global_avg_pool2d_cast);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_2(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)82944;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_outer_fused = 0; ax0_ax1_fused_ax2_fused_ax3_outer_fused < 98; ++ax0_ax1_fused_ax2_fused_ax3_outer_fused) {
    // cram-array axis
    {
      int32_t rc_outer = 0;
      void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
      for (int32_t ry = 0; ry < 3; ++ry) {
        for (int32_t rx = 0; rx < 3; ++rx) {
          for (int32_t rc_inner = 0; rc_inner < 2; ++rc_inner) {
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 16, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 256, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp((((((ry*786432) + (rx*262144)) + (rc.outer*1024)) + (rc.inner*512)) + (tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256)), 1, 256)]*/);
              request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 256, PrecisionT::Precision{0, 8, 0} /*PaddedInput[(((((((ax0.ax1.fused.ax2.fused.ax3.outer.fused/14)*4608) + (ry*4608)) + (tir.shift_right((ax0.ax1.fused.ax2.fused.ax3.outer.fused % 14), 1)*512)) + (rx*512)) + (rc.outer*2)) + rc.inner)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
    }
    void* _7 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t rc_outer_v = 256, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 1, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 1, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
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

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_2__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_2", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_2);

int32_t tvmgen_default_fused_cast_1(System *sys) {
  void* _1 = nullptr;
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_cast_1__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_cast_1", tvmgen_default_fused_cast_1);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2(System *sys) {
  void* _1 = nullptr;
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_outer_fused = 0; ax0_ax1_fused_ax2_fused_ax3_outer_fused < 98; ++ax0_ax1_fused_ax2_fused_ax3_outer_fused) {
    // cram-array axis
    {
      int32_t rc_outer = 0;
      void* _2 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 16, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 256, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(((rc.outer*512) + (tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256)), 1, 256)]*/);
        request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 256, PrecisionT::Precision{0, 8, 0} /*placeholder[((((ax0.ax1.fused.ax2.fused.ax3.outer.fused/14)*7168) + (tir.shift_right((ax0.ax1.fused.ax2.fused.ax3.outer.fused % 14), 1)*512)) + rc.outer)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
    void* _3 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t rc_outer_v = 256, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 1, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 1, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((ax0.ax1.fused.ax2.fused.ax3.outer.fused*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_5603799462163545803_(System *sys) {
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
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 224, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 224, PrecisionT::Precision{0, 8, 0} /*PaddedInput[((((x256(((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*3712) + (ry*3712)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(64)) in (let rdiv = (ramp(0, 1, 256)/x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(64))) + x256((rx*64))) + x256((rc.outer*2))) + x256(rc.inner))]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 224, PrecisionT::Precision{0, 8, 0} /*placeholder[(x256(((((ry*12288) + (rx*4096)) + (rc.outer*128)) + (rc.inner*64))) + (let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64)))))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 224, PrecisionT::Precision{0, 32, 0} /**/);
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
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 7, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 72), 7, PrecisionT::Precision{0, 40, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(((ax0.ax1.fused.ax2.outer.outer.fused*1792) + (ax2.outer.inner*256)), 1, 256)]*/);
        request.addOperand(ax0_ax1_fused_ax2_outer_outer_fused * 32, 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 64, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 72), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 64, 0} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
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

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_5603799462163545803___ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_5603799462163545803_", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_5603799462163545803_);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776_(System *sys) {
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
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 224, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 224, PrecisionT::Precision{0, 8, 0} /*PaddedInput[((((x256(((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*3712) + (ry*3712)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(64)) in (let rdiv = (ramp(0, 1, 256)/x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(64))) + x256((rx*64))) + x256((rc.outer*2))) + x256(rc.inner))]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 224, PrecisionT::Precision{0, 8, 0} /*placeholder[(x256(((((ry*12288) + (rx*4096)) + (rc.outer*128)) + (rc.inner*64))) + (let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64)))))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 224, PrecisionT::Precision{0, 32, 0} /**/);
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
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 7, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 72), 7, PrecisionT::Precision{0, 40, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(((ax0.ax1.fused.ax2.outer.outer.fused*1792) + (ax2.outer.inner*256)), 1, 256)]*/);
        request.addOperand(ax0_ax1_fused_ax2_outer_outer_fused * 32, 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 64, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 72), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 64, 0} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
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

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776___ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776_", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776_);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)314646;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput[256], 0
  for (int32_t ax1 = 0; ax1 < 112; ++ax1) {
    // cram-array axis
    {
      int32_t ax2 = 0;
      void* _6 = (void*) "Conv2dOutput[ramp(0, 1, 64)] = x64(0)/*skip-init*/";
      for (int32_t ry = 0; ry < 7; ++ry) {
        for (int32_t rx = 0; rx < 7; ++rx) {
          for (int32_t rc = 0; rc < 3; ++rc) {
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(ax1, 0, 12), 112, PrecisionT::Precision{0, 16, 0} /**/);
              request.addOperand(sys->getAddress(ax1, 0, 8), 112, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp((((ry*1344) + (rx*192)) + (rc*64)), 1, 64)]*/);
              request.addOperand(ax1 * 32, 112, PrecisionT::Precision{0, 8, 0} /*PaddedInput[(((((ax1*1374) + (ry*687)) + (ax2*6)) + (rx*3)) + rc)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax1, 0, 0), 112, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax1, 0, 12), 112, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax1, 0, 0), 112, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 64)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax1, 0, 12), 112, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 0), 112, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 64)]*/);
        request.addOperand(sys->getAddress(ax1, 0, 8), 112, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 64)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax1, 0, 40), 112, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 12), 112, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 24), 112, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 64)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax1, 0, 28), 112, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 40), 112, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 24), 112, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 64)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax1, 0, 16), 112, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 28), 112, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 12), 112, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 64)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax1, 0, 12), 112, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 16), 112, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, -1), 112, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax1, 0, 12), 112, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 12), 112, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, -1), 112, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed Conv2dOutput
  int32_t _7 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _8 = _7 != 0;
  if (_8) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast);

int32_t tvmgen_default_fused_nn_max_pool2d(System *sys) {
  void* _1 = nullptr;
  for (int32_t ax1 = 0; ax1 < 56; ++ax1) {
    // cram-array axis
    {
      int32_t ax2 = 0;
      void* _2 = (void*) "tensor[ramp(((ax1*3584) + (ax2*64)), 1, 64)] = x64((uint8)0)/*skip-init*/";
      for (int32_t rv0 = 0; rv0 < 3; ++rv0) {
        for (int32_t rv1 = 0; rv1 < 3; ++rv1) {
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(ax1, 0, 0), 56, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            request.addOperand(sys->getAddress(ax1, 0, 0), 56, PrecisionT::Precision{0, 8, 0} /*tensor[ramp(((ax1*3584) + (ax2*64)), 1, 64)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd_CRAM_RF);
            request.addOperand(sys->getAddress(ax1, 0, 4), 56, PrecisionT::Precision{0, 8, 0} /**/);
            request.addOperand(sys->getAddress(ax1, 0, 0), 56, PrecisionT::Precision{0, 8, 0} /*tensor[ramp(((ax1*3584) + (ax2*64)), 1, 64)]*/);
            request.addOperand(sys->getAddress(ax1, 0, -1), 56, PrecisionT::Precision{0, 8, 0} /*ConstInt*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowStore);
            request.addOperand(sys->getAddress(ax1, 0, 0), 56, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            request.addOperand(sys->getAddress(ax1, 0, 4), 56, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
            sys->sendRequest(request);
          }
        }
      }
    }
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_max_pool2d__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_max_pool2d", tvmgen_default_fused_nn_max_pool2d);

int32_t tvmgen_default_fused_nn_softmax(System *sys) {
  void* _1 = nullptr;
  for (int32_t i1 = 0; i1 < 1000; ++i1) {
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(0, 0, 0), 1, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
      request.addOperand(sys->getAddress(0, 0, 0), 1, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_softmax__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_softmax", tvmgen_default_fused_nn_softmax);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_1(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)415872;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_outer_outer_fused = 0; ax0_ax1_fused_ax2_outer_outer_fused < 56; ++ax0_ax1_fused_ax2_outer_outer_fused) {
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
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 224, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 224, PrecisionT::Precision{0, 8, 0} /*PaddedInput[((((x256(((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*7296) + (ry*3648)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(128)) in (let rdiv = (ramp(0, 1, 256)/x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(128))) + x256((rx*64))) + x256((rc.outer*2))) + x256(rc.inner))]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 224, PrecisionT::Precision{0, 8, 0} /*placeholder[(x256(((((ry*24576) + (rx*8192)) + (rc.outer*256)) + (rc.inner*128))) + (let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128)))))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 224, PrecisionT::Precision{0, 32, 0} /**/);
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
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 7, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
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

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_1__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_1", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_1);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__3(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)82944;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_outer_fused = 0; ax0_ax1_fused_ax2_fused_ax3_outer_fused < 98; ++ax0_ax1_fused_ax2_fused_ax3_outer_fused) {
    // cram-array axis
    {
      int32_t rc_outer = 0;
      void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
      for (int32_t ry = 0; ry < 3; ++ry) {
        for (int32_t rx = 0; rx < 3; ++rx) {
          for (int32_t rc_inner = 0; rc_inner < 2; ++rc_inner) {
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 16, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 256, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp((((((ry*786432) + (rx*262144)) + (rc.outer*1024)) + (rc.inner*512)) + (tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256)), 1, 256)]*/);
              request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 256, PrecisionT::Precision{0, 8, 0} /*PaddedInput[(((((((ax0.ax1.fused.ax2.fused.ax3.outer.fused/14)*4608) + (ry*4608)) + (tir.shift_right((ax0.ax1.fused.ax2.fused.ax3.outer.fused % 14), 1)*512)) + (rx*512)) + (rc.outer*2)) + rc.inner)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
    }
    void* _7 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t rc_outer_v = 256, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 1, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 1, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 72), 1, PrecisionT::Precision{0, 40, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 1, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp((ax0.ax1.fused.ax2.fused.ax3.outer.fused*256), 1, 256)]*/);
      request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 64, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 72), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 1, PrecisionT::Precision{0, 64, 0} /**/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
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

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__3__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__3", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__3);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_4(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)131072;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_outer_fused = 0; ax0_ax1_fused_ax2_outer_fused < 98; ++ax0_ax1_fused_ax2_outer_fused) {
    for (int32_t ax2_inner = 0; ax2_inner < 2; ++ax2_inner) {
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
        for (int32_t ry = 0; ry < 3; ++ry) {
          for (int32_t rx = 0; rx < 3; ++rx) {
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 16, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 256, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp((((ry*196608) + (rx*65536)) + (rc.outer*256)), 1, 256)]*/);
              request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 256, PrecisionT::Precision{0, 8, 0} /*PaddedInput[(((((((ax0.ax1.fused.ax2.outer.fused/7)*4096) + (ry*4096)) + ((ax0.ax1.fused.ax2.outer.fused % 7)*512)) + (ax2.inner*256)) + (rx*256)) + rc.outer)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
      void* _7 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 256, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 1, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 1, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 192), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 144), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 192), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 144), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 1, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
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

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_4__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_4", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_4);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_6(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)82944;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_outer_fused = 0; ax0_ax1_fused_ax2_fused_ax3_outer_fused < 98; ++ax0_ax1_fused_ax2_fused_ax3_outer_fused) {
    // cram-array axis
    {
      int32_t rc_outer = 0;
      void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
      for (int32_t ry = 0; ry < 3; ++ry) {
        for (int32_t rx = 0; rx < 3; ++rx) {
          for (int32_t rc_inner = 0; rc_inner < 2; ++rc_inner) {
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 16, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 256, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp((((((ry*786432) + (rx*262144)) + (rc.outer*1024)) + (rc.inner*512)) + (tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256)), 1, 256)]*/);
              request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 256, PrecisionT::Precision{0, 8, 0} /*PaddedInput[(((((((ax0.ax1.fused.ax2.fused.ax3.outer.fused/14)*4608) + (ry*4608)) + (tir.shift_right((ax0.ax1.fused.ax2.fused.ax3.outer.fused % 14), 1)*512)) + (rx*512)) + (rc.outer*2)) + rc.inner)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
    }
    void* _7 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t rc_outer_v = 256, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 1, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 1, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
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

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_6__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_6", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_6);

int32_t tvmgen_default_fused_cast_3(System *sys) {
  void* _1 = nullptr;
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_cast_3__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_cast_3", tvmgen_default_fused_cast_3);

int32_t tvmgen_default_fused_cast(System *sys) {
  void* _1 = nullptr;
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_cast__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_cast", tvmgen_default_fused_cast);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__1(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)230400;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_outer_outer_fused = 0; ax0_ax1_fused_ax2_outer_outer_fused < 56; ++ax0_ax1_fused_ax2_outer_outer_fused) {
    // cram-array axis
    {
      int32_t ax2_outer_inner = 0;
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
        for (int32_t ry = 0; ry < 3; ++ry) {
          for (int32_t rx = 0; rx < 3; ++rx) {
            for (int32_t rc_inner = 0; rc_inner < 4; ++rc_inner) {
              {
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 224, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 224, PrecisionT::Precision{0, 8, 0} /*PaddedInput[((((x256(((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*3840) + (ry*3840)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(128)) in (let rdiv = (ramp(0, 1, 256)/x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(128))) + x256((rx*128))) + x256((rc.outer*4))) + x256(rc.inner))]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 224, PrecisionT::Precision{0, 8, 0} /*placeholder[(x256(((((ry*49152) + (rx*16384)) + (rc.outer*512)) + (rc.inner*128))) + (let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128)))))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 224, PrecisionT::Precision{0, 32, 0} /**/);
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
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 7, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 72), 7, PrecisionT::Precision{0, 40, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(((ax0.ax1.fused.ax2.outer.outer.fused*1792) + (ax2.outer.inner*256)), 1, 256)]*/);
        request.addOperand(ax0_ax1_fused_ax2_outer_outer_fused * 32, 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 64, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 72), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 64, 0} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
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

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__1__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__1", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__1);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)230400;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_outer_outer_fused = 0; ax0_ax1_fused_ax2_outer_outer_fused < 56; ++ax0_ax1_fused_ax2_outer_outer_fused) {
    // cram-array axis
    {
      int32_t ax2_outer_inner = 0;
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
        for (int32_t ry = 0; ry < 3; ++ry) {
          for (int32_t rx = 0; rx < 3; ++rx) {
            for (int32_t rc_inner = 0; rc_inner < 4; ++rc_inner) {
              {
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 224, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 224, PrecisionT::Precision{0, 8, 0} /*PaddedInput[((((x256(((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*3840) + (ry*3840)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(128)) in (let rdiv = (ramp(0, 1, 256)/x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(128))) + x256((rx*128))) + x256((rc.outer*4))) + x256(rc.inner))]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 224, PrecisionT::Precision{0, 8, 0} /*placeholder[(x256(((((ry*49152) + (rx*16384)) + (rc.outer*512)) + (rc.inner*128))) + (let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128)))))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 224, PrecisionT::Precision{0, 32, 0} /**/);
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
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 7, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
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

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_1(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)131072;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_outer_fused = 0; ax0_ax1_fused_ax2_outer_fused < 98; ++ax0_ax1_fused_ax2_outer_fused) {
    for (int32_t ax2_inner = 0; ax2_inner < 2; ++ax2_inner) {
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
        for (int32_t ry = 0; ry < 3; ++ry) {
          for (int32_t rx = 0; rx < 3; ++rx) {
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 16, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 256, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp((((ry*196608) + (rx*65536)) + (rc.outer*256)), 1, 256)]*/);
              request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 256, PrecisionT::Precision{0, 8, 0} /*PaddedInput[(((((((ax0.ax1.fused.ax2.outer.fused/7)*4096) + (ry*4096)) + ((ax0.ax1.fused.ax2.outer.fused % 7)*512)) + (ax2.inner*256)) + (rx*256)) + rc.outer)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
      void* _7 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 256, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 1, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 1, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 192), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 144), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 192), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 144), 1, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
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

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_1__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_1", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_1);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast(System *sys) {
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
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 224, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 224, PrecisionT::Precision{0, 8, 0} /*PaddedInput[((((x256(((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*3712) + (ry*3712)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(64)) in (let rdiv = (ramp(0, 1, 256)/x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(64))) + x256((rx*64))) + x256((rc.outer*2))) + x256(rc.inner))]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 224, PrecisionT::Precision{0, 8, 0} /*placeholder[(x256(((((ry*12288) + (rx*4096)) + (rc.outer*128)) + (rc.inner*64))) + (let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64)))))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 224, PrecisionT::Precision{0, 32, 0} /**/);
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
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 224, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 7, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 7, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 7, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 7, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 7, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
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

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_5(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)115200;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_outer_fused = 0; ax0_ax1_fused_ax2_fused_ax3_outer_fused < 98; ++ax0_ax1_fused_ax2_fused_ax3_outer_fused) {
    // cram-array axis
    {
      int32_t rc_outer = 0;
      void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
      for (int32_t ry = 0; ry < 3; ++ry) {
        for (int32_t rx = 0; rx < 3; ++rx) {
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 256, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(((((ry*393216) + (rx*131072)) + (rc.outer*512)) + (tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256)), 1, 256)]*/);
            request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 256, PrecisionT::Precision{0, 8, 0} /*PaddedInput[((((((ax0.ax1.fused.ax2.fused.ax3.outer.fused/14)*7680) + (ry*3840)) + (tir.shift_right((ax0.ax1.fused.ax2.fused.ax3.outer.fused % 14), 1)*512)) + (rx*256)) + rc.outer)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 32, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
            sys->sendRequest(request);
          }
        }
      }
    }
    void* _7 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t rc_outer_v = 256, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 1, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 1, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 1, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 1, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
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

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_5__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_5", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_5);

int32_t tvmgen_default_fused_nn_contrib_dense_pack_add_cast_multiply(System *sys) {
  void* _1 = nullptr;
  // int32_t compute_rf[1000], 0
  // int32_t compute[1000], 1000
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 4; ++ax0_ax1_outer_fused) {
    // cram-array axis
    {
      int32_t k_outer = 0;
      void* _2 = (void*) "compute.rf[ramp((k.outer*250), 1, 250)] = x250(0)/*skip-init*/";
      for (int32_t k_inner = 0; k_inner < 4; ++k_inner) {
        int32_t _3 = 0 * 4;
        int32_t _4 = _3 + k_inner;
        int32_t _5 = _4 % 64;
        bool _6 = _5 == 0;
        if (_6) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _7 = 1 * 4;
        int32_t _8 = _7 + k_inner;
        int32_t _9 = _8 % 64;
        bool _10 = _9 == 0;
        if (_10) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _11 = 2 * 4;
        int32_t _12 = _11 + k_inner;
        int32_t _13 = _12 % 64;
        bool _14 = _13 == 0;
        if (_14) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _15 = 3 * 4;
        int32_t _16 = _15 + k_inner;
        int32_t _17 = _16 % 64;
        bool _18 = _17 == 0;
        if (_18) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _19 = 4 * 4;
        int32_t _20 = _19 + k_inner;
        int32_t _21 = _20 % 64;
        bool _22 = _21 == 0;
        if (_22) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _23 = 5 * 4;
        int32_t _24 = _23 + k_inner;
        int32_t _25 = _24 % 64;
        bool _26 = _25 == 0;
        if (_26) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _27 = 6 * 4;
        int32_t _28 = _27 + k_inner;
        int32_t _29 = _28 % 64;
        bool _30 = _29 == 0;
        if (_30) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _31 = 7 * 4;
        int32_t _32 = _31 + k_inner;
        int32_t _33 = _32 % 64;
        bool _34 = _33 == 0;
        if (_34) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _35 = 8 * 4;
        int32_t _36 = _35 + k_inner;
        int32_t _37 = _36 % 64;
        bool _38 = _37 == 0;
        if (_38) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _39 = 9 * 4;
        int32_t _40 = _39 + k_inner;
        int32_t _41 = _40 % 64;
        bool _42 = _41 == 0;
        if (_42) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _43 = 10 * 4;
        int32_t _44 = _43 + k_inner;
        int32_t _45 = _44 % 64;
        bool _46 = _45 == 0;
        if (_46) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _47 = 11 * 4;
        int32_t _48 = _47 + k_inner;
        int32_t _49 = _48 % 64;
        bool _50 = _49 == 0;
        if (_50) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _51 = 12 * 4;
        int32_t _52 = _51 + k_inner;
        int32_t _53 = _52 % 64;
        bool _54 = _53 == 0;
        if (_54) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _55 = 13 * 4;
        int32_t _56 = _55 + k_inner;
        int32_t _57 = _56 % 64;
        bool _58 = _57 == 0;
        if (_58) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _59 = 14 * 4;
        int32_t _60 = _59 + k_inner;
        int32_t _61 = _60 % 64;
        bool _62 = _61 == 0;
        if (_62) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _63 = 15 * 4;
        int32_t _64 = _63 + k_inner;
        int32_t _65 = _64 % 64;
        bool _66 = _65 == 0;
        if (_66) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _67 = 16 * 4;
        int32_t _68 = _67 + k_inner;
        int32_t _69 = _68 % 64;
        bool _70 = _69 == 0;
        if (_70) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _71 = 17 * 4;
        int32_t _72 = _71 + k_inner;
        int32_t _73 = _72 % 64;
        bool _74 = _73 == 0;
        if (_74) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _75 = 18 * 4;
        int32_t _76 = _75 + k_inner;
        int32_t _77 = _76 % 64;
        bool _78 = _77 == 0;
        if (_78) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _79 = 19 * 4;
        int32_t _80 = _79 + k_inner;
        int32_t _81 = _80 % 64;
        bool _82 = _81 == 0;
        if (_82) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _83 = 20 * 4;
        int32_t _84 = _83 + k_inner;
        int32_t _85 = _84 % 64;
        bool _86 = _85 == 0;
        if (_86) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _87 = 21 * 4;
        int32_t _88 = _87 + k_inner;
        int32_t _89 = _88 % 64;
        bool _90 = _89 == 0;
        if (_90) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _91 = 22 * 4;
        int32_t _92 = _91 + k_inner;
        int32_t _93 = _92 % 64;
        bool _94 = _93 == 0;
        if (_94) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _95 = 23 * 4;
        int32_t _96 = _95 + k_inner;
        int32_t _97 = _96 % 64;
        bool _98 = _97 == 0;
        if (_98) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _99 = 24 * 4;
        int32_t _100 = _99 + k_inner;
        int32_t _101 = _100 % 64;
        bool _102 = _101 == 0;
        if (_102) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _103 = 25 * 4;
        int32_t _104 = _103 + k_inner;
        int32_t _105 = _104 % 64;
        bool _106 = _105 == 0;
        if (_106) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _107 = 26 * 4;
        int32_t _108 = _107 + k_inner;
        int32_t _109 = _108 % 64;
        bool _110 = _109 == 0;
        if (_110) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _111 = 27 * 4;
        int32_t _112 = _111 + k_inner;
        int32_t _113 = _112 % 64;
        bool _114 = _113 == 0;
        if (_114) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _115 = 28 * 4;
        int32_t _116 = _115 + k_inner;
        int32_t _117 = _116 % 64;
        bool _118 = _117 == 0;
        if (_118) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _119 = 29 * 4;
        int32_t _120 = _119 + k_inner;
        int32_t _121 = _120 % 64;
        bool _122 = _121 == 0;
        if (_122) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _123 = 30 * 4;
        int32_t _124 = _123 + k_inner;
        int32_t _125 = _124 % 64;
        bool _126 = _125 == 0;
        if (_126) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _127 = 31 * 4;
        int32_t _128 = _127 + k_inner;
        int32_t _129 = _128 % 64;
        bool _130 = _129 == 0;
        if (_130) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _131 = 32 * 4;
        int32_t _132 = _131 + k_inner;
        int32_t _133 = _132 % 64;
        bool _134 = _133 == 0;
        if (_134) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _135 = 33 * 4;
        int32_t _136 = _135 + k_inner;
        int32_t _137 = _136 % 64;
        bool _138 = _137 == 0;
        if (_138) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _139 = 34 * 4;
        int32_t _140 = _139 + k_inner;
        int32_t _141 = _140 % 64;
        bool _142 = _141 == 0;
        if (_142) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _143 = 35 * 4;
        int32_t _144 = _143 + k_inner;
        int32_t _145 = _144 % 64;
        bool _146 = _145 == 0;
        if (_146) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _147 = 36 * 4;
        int32_t _148 = _147 + k_inner;
        int32_t _149 = _148 % 64;
        bool _150 = _149 == 0;
        if (_150) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _151 = 37 * 4;
        int32_t _152 = _151 + k_inner;
        int32_t _153 = _152 % 64;
        bool _154 = _153 == 0;
        if (_154) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _155 = 38 * 4;
        int32_t _156 = _155 + k_inner;
        int32_t _157 = _156 % 64;
        bool _158 = _157 == 0;
        if (_158) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _159 = 39 * 4;
        int32_t _160 = _159 + k_inner;
        int32_t _161 = _160 % 64;
        bool _162 = _161 == 0;
        if (_162) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _163 = 40 * 4;
        int32_t _164 = _163 + k_inner;
        int32_t _165 = _164 % 64;
        bool _166 = _165 == 0;
        if (_166) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _167 = 41 * 4;
        int32_t _168 = _167 + k_inner;
        int32_t _169 = _168 % 64;
        bool _170 = _169 == 0;
        if (_170) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _171 = 42 * 4;
        int32_t _172 = _171 + k_inner;
        int32_t _173 = _172 % 64;
        bool _174 = _173 == 0;
        if (_174) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _175 = 43 * 4;
        int32_t _176 = _175 + k_inner;
        int32_t _177 = _176 % 64;
        bool _178 = _177 == 0;
        if (_178) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _179 = 44 * 4;
        int32_t _180 = _179 + k_inner;
        int32_t _181 = _180 % 64;
        bool _182 = _181 == 0;
        if (_182) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _183 = 45 * 4;
        int32_t _184 = _183 + k_inner;
        int32_t _185 = _184 % 64;
        bool _186 = _185 == 0;
        if (_186) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _187 = 46 * 4;
        int32_t _188 = _187 + k_inner;
        int32_t _189 = _188 % 64;
        bool _190 = _189 == 0;
        if (_190) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _191 = 47 * 4;
        int32_t _192 = _191 + k_inner;
        int32_t _193 = _192 % 64;
        bool _194 = _193 == 0;
        if (_194) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _195 = 48 * 4;
        int32_t _196 = _195 + k_inner;
        int32_t _197 = _196 % 64;
        bool _198 = _197 == 0;
        if (_198) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _199 = 49 * 4;
        int32_t _200 = _199 + k_inner;
        int32_t _201 = _200 % 64;
        bool _202 = _201 == 0;
        if (_202) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _203 = 50 * 4;
        int32_t _204 = _203 + k_inner;
        int32_t _205 = _204 % 64;
        bool _206 = _205 == 0;
        if (_206) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _207 = 51 * 4;
        int32_t _208 = _207 + k_inner;
        int32_t _209 = _208 % 64;
        bool _210 = _209 == 0;
        if (_210) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _211 = 52 * 4;
        int32_t _212 = _211 + k_inner;
        int32_t _213 = _212 % 64;
        bool _214 = _213 == 0;
        if (_214) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _215 = 53 * 4;
        int32_t _216 = _215 + k_inner;
        int32_t _217 = _216 % 64;
        bool _218 = _217 == 0;
        if (_218) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _219 = 54 * 4;
        int32_t _220 = _219 + k_inner;
        int32_t _221 = _220 % 64;
        bool _222 = _221 == 0;
        if (_222) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _223 = 55 * 4;
        int32_t _224 = _223 + k_inner;
        int32_t _225 = _224 % 64;
        bool _226 = _225 == 0;
        if (_226) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _227 = 56 * 4;
        int32_t _228 = _227 + k_inner;
        int32_t _229 = _228 % 64;
        bool _230 = _229 == 0;
        if (_230) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _231 = 57 * 4;
        int32_t _232 = _231 + k_inner;
        int32_t _233 = _232 % 64;
        bool _234 = _233 == 0;
        if (_234) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _235 = 58 * 4;
        int32_t _236 = _235 + k_inner;
        int32_t _237 = _236 % 64;
        bool _238 = _237 == 0;
        if (_238) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _239 = 59 * 4;
        int32_t _240 = _239 + k_inner;
        int32_t _241 = _240 % 64;
        bool _242 = _241 == 0;
        if (_242) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _243 = 60 * 4;
        int32_t _244 = _243 + k_inner;
        int32_t _245 = _244 % 64;
        bool _246 = _245 == 0;
        if (_246) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _247 = 61 * 4;
        int32_t _248 = _247 + k_inner;
        int32_t _249 = _248 % 64;
        bool _250 = _249 == 0;
        if (_250) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _251 = 62 * 4;
        int32_t _252 = _251 + k_inner;
        int32_t _253 = _252 % 64;
        bool _254 = _253 == 0;
        if (_254) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _255 = 63 * 4;
        int32_t _256 = _255 + k_inner;
        int32_t _257 = _256 % 64;
        bool _258 = _257 == 0;
        if (_258) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _259 = 64 * 4;
        int32_t _260 = _259 + k_inner;
        int32_t _261 = _260 % 64;
        bool _262 = _261 == 0;
        if (_262) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _263 = 65 * 4;
        int32_t _264 = _263 + k_inner;
        int32_t _265 = _264 % 64;
        bool _266 = _265 == 0;
        if (_266) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _267 = 66 * 4;
        int32_t _268 = _267 + k_inner;
        int32_t _269 = _268 % 64;
        bool _270 = _269 == 0;
        if (_270) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _271 = 67 * 4;
        int32_t _272 = _271 + k_inner;
        int32_t _273 = _272 % 64;
        bool _274 = _273 == 0;
        if (_274) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _275 = 68 * 4;
        int32_t _276 = _275 + k_inner;
        int32_t _277 = _276 % 64;
        bool _278 = _277 == 0;
        if (_278) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _279 = 69 * 4;
        int32_t _280 = _279 + k_inner;
        int32_t _281 = _280 % 64;
        bool _282 = _281 == 0;
        if (_282) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _283 = 70 * 4;
        int32_t _284 = _283 + k_inner;
        int32_t _285 = _284 % 64;
        bool _286 = _285 == 0;
        if (_286) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _287 = 71 * 4;
        int32_t _288 = _287 + k_inner;
        int32_t _289 = _288 % 64;
        bool _290 = _289 == 0;
        if (_290) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _291 = 72 * 4;
        int32_t _292 = _291 + k_inner;
        int32_t _293 = _292 % 64;
        bool _294 = _293 == 0;
        if (_294) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _295 = 73 * 4;
        int32_t _296 = _295 + k_inner;
        int32_t _297 = _296 % 64;
        bool _298 = _297 == 0;
        if (_298) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _299 = 74 * 4;
        int32_t _300 = _299 + k_inner;
        int32_t _301 = _300 % 64;
        bool _302 = _301 == 0;
        if (_302) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _303 = 75 * 4;
        int32_t _304 = _303 + k_inner;
        int32_t _305 = _304 % 64;
        bool _306 = _305 == 0;
        if (_306) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _307 = 76 * 4;
        int32_t _308 = _307 + k_inner;
        int32_t _309 = _308 % 64;
        bool _310 = _309 == 0;
        if (_310) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _311 = 77 * 4;
        int32_t _312 = _311 + k_inner;
        int32_t _313 = _312 % 64;
        bool _314 = _313 == 0;
        if (_314) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _315 = 78 * 4;
        int32_t _316 = _315 + k_inner;
        int32_t _317 = _316 % 64;
        bool _318 = _317 == 0;
        if (_318) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _319 = 79 * 4;
        int32_t _320 = _319 + k_inner;
        int32_t _321 = _320 % 64;
        bool _322 = _321 == 0;
        if (_322) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _323 = 80 * 4;
        int32_t _324 = _323 + k_inner;
        int32_t _325 = _324 % 64;
        bool _326 = _325 == 0;
        if (_326) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _327 = 81 * 4;
        int32_t _328 = _327 + k_inner;
        int32_t _329 = _328 % 64;
        bool _330 = _329 == 0;
        if (_330) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _331 = 82 * 4;
        int32_t _332 = _331 + k_inner;
        int32_t _333 = _332 % 64;
        bool _334 = _333 == 0;
        if (_334) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _335 = 83 * 4;
        int32_t _336 = _335 + k_inner;
        int32_t _337 = _336 % 64;
        bool _338 = _337 == 0;
        if (_338) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _339 = 84 * 4;
        int32_t _340 = _339 + k_inner;
        int32_t _341 = _340 % 64;
        bool _342 = _341 == 0;
        if (_342) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _343 = 85 * 4;
        int32_t _344 = _343 + k_inner;
        int32_t _345 = _344 % 64;
        bool _346 = _345 == 0;
        if (_346) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _347 = 86 * 4;
        int32_t _348 = _347 + k_inner;
        int32_t _349 = _348 % 64;
        bool _350 = _349 == 0;
        if (_350) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _351 = 87 * 4;
        int32_t _352 = _351 + k_inner;
        int32_t _353 = _352 % 64;
        bool _354 = _353 == 0;
        if (_354) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _355 = 88 * 4;
        int32_t _356 = _355 + k_inner;
        int32_t _357 = _356 % 64;
        bool _358 = _357 == 0;
        if (_358) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _359 = 89 * 4;
        int32_t _360 = _359 + k_inner;
        int32_t _361 = _360 % 64;
        bool _362 = _361 == 0;
        if (_362) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _363 = 90 * 4;
        int32_t _364 = _363 + k_inner;
        int32_t _365 = _364 % 64;
        bool _366 = _365 == 0;
        if (_366) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _367 = 91 * 4;
        int32_t _368 = _367 + k_inner;
        int32_t _369 = _368 % 64;
        bool _370 = _369 == 0;
        if (_370) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _371 = 92 * 4;
        int32_t _372 = _371 + k_inner;
        int32_t _373 = _372 % 64;
        bool _374 = _373 == 0;
        if (_374) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _375 = 93 * 4;
        int32_t _376 = _375 + k_inner;
        int32_t _377 = _376 % 64;
        bool _378 = _377 == 0;
        if (_378) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _379 = 94 * 4;
        int32_t _380 = _379 + k_inner;
        int32_t _381 = _380 % 64;
        bool _382 = _381 == 0;
        if (_382) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _383 = 95 * 4;
        int32_t _384 = _383 + k_inner;
        int32_t _385 = _384 % 64;
        bool _386 = _385 == 0;
        if (_386) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _387 = 96 * 4;
        int32_t _388 = _387 + k_inner;
        int32_t _389 = _388 % 64;
        bool _390 = _389 == 0;
        if (_390) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _391 = 97 * 4;
        int32_t _392 = _391 + k_inner;
        int32_t _393 = _392 % 64;
        bool _394 = _393 == 0;
        if (_394) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _395 = 98 * 4;
        int32_t _396 = _395 + k_inner;
        int32_t _397 = _396 % 64;
        bool _398 = _397 == 0;
        if (_398) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _399 = 99 * 4;
        int32_t _400 = _399 + k_inner;
        int32_t _401 = _400 % 64;
        bool _402 = _401 == 0;
        if (_402) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _403 = 100 * 4;
        int32_t _404 = _403 + k_inner;
        int32_t _405 = _404 % 64;
        bool _406 = _405 == 0;
        if (_406) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _407 = 101 * 4;
        int32_t _408 = _407 + k_inner;
        int32_t _409 = _408 % 64;
        bool _410 = _409 == 0;
        if (_410) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _411 = 102 * 4;
        int32_t _412 = _411 + k_inner;
        int32_t _413 = _412 % 64;
        bool _414 = _413 == 0;
        if (_414) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _415 = 103 * 4;
        int32_t _416 = _415 + k_inner;
        int32_t _417 = _416 % 64;
        bool _418 = _417 == 0;
        if (_418) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _419 = 104 * 4;
        int32_t _420 = _419 + k_inner;
        int32_t _421 = _420 % 64;
        bool _422 = _421 == 0;
        if (_422) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _423 = 105 * 4;
        int32_t _424 = _423 + k_inner;
        int32_t _425 = _424 % 64;
        bool _426 = _425 == 0;
        if (_426) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _427 = 106 * 4;
        int32_t _428 = _427 + k_inner;
        int32_t _429 = _428 % 64;
        bool _430 = _429 == 0;
        if (_430) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _431 = 107 * 4;
        int32_t _432 = _431 + k_inner;
        int32_t _433 = _432 % 64;
        bool _434 = _433 == 0;
        if (_434) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _435 = 108 * 4;
        int32_t _436 = _435 + k_inner;
        int32_t _437 = _436 % 64;
        bool _438 = _437 == 0;
        if (_438) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _439 = 109 * 4;
        int32_t _440 = _439 + k_inner;
        int32_t _441 = _440 % 64;
        bool _442 = _441 == 0;
        if (_442) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _443 = 110 * 4;
        int32_t _444 = _443 + k_inner;
        int32_t _445 = _444 % 64;
        bool _446 = _445 == 0;
        if (_446) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _447 = 111 * 4;
        int32_t _448 = _447 + k_inner;
        int32_t _449 = _448 % 64;
        bool _450 = _449 == 0;
        if (_450) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _451 = 112 * 4;
        int32_t _452 = _451 + k_inner;
        int32_t _453 = _452 % 64;
        bool _454 = _453 == 0;
        if (_454) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _455 = 113 * 4;
        int32_t _456 = _455 + k_inner;
        int32_t _457 = _456 % 64;
        bool _458 = _457 == 0;
        if (_458) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _459 = 114 * 4;
        int32_t _460 = _459 + k_inner;
        int32_t _461 = _460 % 64;
        bool _462 = _461 == 0;
        if (_462) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _463 = 115 * 4;
        int32_t _464 = _463 + k_inner;
        int32_t _465 = _464 % 64;
        bool _466 = _465 == 0;
        if (_466) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _467 = 116 * 4;
        int32_t _468 = _467 + k_inner;
        int32_t _469 = _468 % 64;
        bool _470 = _469 == 0;
        if (_470) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _471 = 117 * 4;
        int32_t _472 = _471 + k_inner;
        int32_t _473 = _472 % 64;
        bool _474 = _473 == 0;
        if (_474) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _475 = 118 * 4;
        int32_t _476 = _475 + k_inner;
        int32_t _477 = _476 % 64;
        bool _478 = _477 == 0;
        if (_478) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _479 = 119 * 4;
        int32_t _480 = _479 + k_inner;
        int32_t _481 = _480 % 64;
        bool _482 = _481 == 0;
        if (_482) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _483 = 120 * 4;
        int32_t _484 = _483 + k_inner;
        int32_t _485 = _484 % 64;
        bool _486 = _485 == 0;
        if (_486) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _487 = 121 * 4;
        int32_t _488 = _487 + k_inner;
        int32_t _489 = _488 % 64;
        bool _490 = _489 == 0;
        if (_490) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _491 = 122 * 4;
        int32_t _492 = _491 + k_inner;
        int32_t _493 = _492 % 64;
        bool _494 = _493 == 0;
        if (_494) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _495 = 123 * 4;
        int32_t _496 = _495 + k_inner;
        int32_t _497 = _496 % 64;
        bool _498 = _497 == 0;
        if (_498) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _499 = 124 * 4;
        int32_t _500 = _499 + k_inner;
        int32_t _501 = _500 % 64;
        bool _502 = _501 == 0;
        if (_502) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _503 = 125 * 4;
        int32_t _504 = _503 + k_inner;
        int32_t _505 = _504 % 64;
        bool _506 = _505 == 0;
        if (_506) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _507 = 126 * 4;
        int32_t _508 = _507 + k_inner;
        int32_t _509 = _508 % 64;
        bool _510 = _509 == 0;
        if (_510) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _511 = 127 * 4;
        int32_t _512 = _511 + k_inner;
        int32_t _513 = _512 % 64;
        bool _514 = _513 == 0;
        if (_514) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
                {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 62), 128, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((ax0.ax1.outer.fused*128000) + (k.outer*1000)) + (k.inner*250)), 1, 250)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 78), 128, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 62), 128, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((ax0.ax1.outer.fused*128000) + (k.outer*1000)) + (k.inner*250)), 1, 250)]*/);
          request.addOperand(ax0_ax1_outer_fused * 32, 128, PrecisionT::Precision{0, 16, 0} /*placeholder[((k.outer*4) + k.inner)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 78), 128, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 32, 0} /*compute.rf[ramp((k.outer*250), 1, 250)]*/);
          sys->sendRequest(request);
        }
      }
    }
    void* _516 = (void*) "compute[ramp(0, 1, 250)] = x250(0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t k_outer_v = 128, k_outer_v_cnt = 1; k_outer_v >= 1; k_outer_v -= max(k_outer_v / 2, 1), ++k_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 31), 128, PrecisionT::Precision{0, 32, 0} /*compute[ramp(0, 1, 250)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 32, 0} /*compute.rf[ramp((k.outer.v*250), 1, 250)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 31), 128, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 31), 128, PrecisionT::Precision{0, 32, 0} /*compute[ramp(0, 1, 250)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 128, PrecisionT::Precision{0, 32, 0} /*compute.rf[ramp((k.outer.v*250), 1, 250)]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 1, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 62), 1, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((ax0.ax1.outer.fused*250), 1, 250)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 93), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 31), 1, PrecisionT::Precision{0, 32, 0} /*compute[ramp(0, 1, 250)]*/);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 62), 1, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((ax0.ax1.outer.fused*250), 1, 250)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 125), 1, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 93), 1, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(ax0_ax1_outer_fused * 32, 1, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 1, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 125), 1, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  // freed compute
  // freed compute_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_contrib_dense_pack_add_cast_multiply__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_contrib_dense_pack_add_cast_multiply", tvmgen_default_fused_nn_contrib_dense_pack_add_cast_multiply);

} // resnet18_v1__quantized