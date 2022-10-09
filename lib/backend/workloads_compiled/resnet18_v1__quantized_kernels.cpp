// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

namespace resnet18_v1__quantized {

void *TVMBackendAllocWorkspace(...) { return (void *) 1; }
int TVMBackendFreeWorkspace(...) { return 0; }

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
            for (int32_t rc_inner = 0; rc_inner < 4; ++rc_inner) {
              {
                Request request(Request::Type::RowLoad);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[(x256(((((ry*24576) + (rx*8192)) + (rc.outer*512)) + (rc.inner*128))) + (let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128)))))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*PaddedInput[((((x256(((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*7296) + (ry*3648)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(128)) in (let rdiv = (ramp(0, 1, 256)/x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(128))) + x256((rx*64))) + x256((rc.outer*4))) + x256(rc.inner))]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[(x256(((((ry*24576) + (rx*8192)) + (rc.outer*512)) + (rc.inner*128))) + (let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128)))))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
                sys->sendRequest(request);
              }
            }
          }
        }
      }
      void* _7 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 16, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
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
            int32_t _7 = ax1 * 1374;
            int32_t _8 = ry * 687;
            int32_t _9 = _7 + _8;
            int32_t _10 = ax2 * 6;
            int32_t _11 = _9 + _10;
            int32_t _12 = rx * 3;
            int32_t _13 = _11 + _12;
            int32_t _14 = _13 + rc;
            int32_t _15 = _14 % 64;
            bool _16 = _15 == 0;
            if (_16) {
              {
                Request request(Request::Type::RowLoad_RF);
                request.addOperand(ax1 * 32, 0, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
                request.addOperand(sys->getAddress(ax1, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                sys->sendRequest(request);
              }
            }
            {
              Request request(Request::Type::RowLoad);
              request.addOperand(sys->getAddress(ax1, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
              request.addOperand(sys->getAddress(ax1, 0, 8), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((ry*1344) + (rx*192)) + (rc*64)), 1, 64)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(ax1, 0, 12), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax1, 0, 8), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((ry*1344) + (rx*192)) + (rc*64)), 1, 64)]*/);
              request.addOperand(ax1 * 32, 0, PrecisionT::Precision{0, 16, 0} /*PaddedInput[(((((ax1*1374) + (ry*687)) + (ax2*6)) + (rx*3)) + rc)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax1, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax1, 0, 12), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax1, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 64)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax1, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax1, 0, 8), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 64)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax1, 0, 12), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 64)]*/);
        request.addOperand(sys->getAddress(ax1, 0, 8), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 64)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax1, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax1, 0, 24), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 64)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax1, 0, 40), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 12), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 24), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 64)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax1, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax1, 0, 24), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 64)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax1, 0, 28), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 40), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 24), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 64)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax1, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax1, 0, 12), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 64)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax1, 0, 16), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 28), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 12), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 64)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax1, 0, 12), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax1, 0, 12), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, 12), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax1, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax1, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax1, 0, 12), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed Conv2dOutput
  int32_t _17 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _18 = _17 != 0;
  if (_18) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_2(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)230400;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput_rf[512], 0
  // int32_t Conv2dOutput[512], 512
  for (int32_t ax0_ax1_fused_ax2_outer_fused = 0; ax0_ax1_fused_ax2_outer_fused < 112; ++ax0_ax1_fused_ax2_outer_fused) {
    for (int32_t ax2_inner = 0; ax2_inner < 7; ++ax2_inner) {
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*128), 1, 128)] = x128(0)/*skip-init*/";
        for (int32_t ry = 0; ry < 3; ++ry) {
          for (int32_t rx = 0; rx < 3; ++rx) {
            int32_t _7 = ax0_ax1_fused_ax2_outer_fused >> 2;
            int32_t _8 = _7 * 3840;
            int32_t _9 = ry * 3840;
            int32_t _10 = _8 + _9;
            int32_t _11 = ax0_ax1_fused_ax2_outer_fused & 3;
            int32_t _12 = _11 * 896;
            int32_t _13 = _10 + _12;
            int32_t _14 = ax2_inner * 128;
            int32_t _15 = _13 + _14;
            int32_t _16 = rx * 128;
            int32_t _17 = _15 + _16;
            int32_t _18 = _17 + rc_outer;
            int32_t _19 = _18 % 64;
            bool _20 = _19 == 0;
            if (_20) {
              {
                Request request(Request::Type::RowLoad_RF);
                request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                sys->sendRequest(request);
              }
            }
            {
              Request request(Request::Type::RowLoad);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((ry*49152) + (rx*16384)) + (rc.outer*128)), 1, 128)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((ry*49152) + (rx*16384)) + (rc.outer*128)), 1, 128)]*/);
              request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*PaddedInput[((((((tir.shift_right(ax0.ax1.fused.ax2.outer.fused, 2)*3840) + (ry*3840)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.fused, 3)*896)) + (ax2.inner*128)) + (rx*128)) + rc.outer)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*128), 1, 128)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
      void* _21 = (void*) "Conv2dOutput[ramp(0, 1, 128)] = x128(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 128, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 128)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*128), 1, 128)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 128)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*128), 1, 128)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 128)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 72), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 48), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 72), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 48), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  int32_t _22 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _23 = _22 != 0;
  if (_23) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_2__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_2", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_2);

int32_t tvmgen_default_fused_nn_global_avg_pool2d_cast(System *sys) {
  void* _1 = nullptr;
  // int32_t tensor[1024], 0
  for (int32_t ax3_outer = 0; ax3_outer < 2; ++ax3_outer) {
    void* _2 = (void*) "tensor[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
    for (int32_t rv0 = 0; rv0 < 7; ++rv0) {
      for (int32_t rv1 = 0; rv1 < 7; ++rv1) {
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(ax3_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(ax3_outer, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((((rv0*3584) + (rv1*512)) + (ax3.outer*256)), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax3_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax3_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*tensor[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax3_outer, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((((rv0*3584) + (rv1*512)) + (ax3.outer*256)), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(ax3_outer, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax3_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*tensor[ramp(0, 1, 256)]*/);
      request.addOperand(ax3_outer * 32, 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(ax3_outer, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax3_outer, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  // freed tensor
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_global_avg_pool2d_cast__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_global_avg_pool2d_cast", tvmgen_default_fused_nn_global_avg_pool2d_cast);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)230400;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput_rf[512], 0
  // int32_t Conv2dOutput[512], 512
  for (int32_t ax0_ax1_fused_ax2_outer_fused = 0; ax0_ax1_fused_ax2_outer_fused < 112; ++ax0_ax1_fused_ax2_outer_fused) {
    for (int32_t ax2_inner = 0; ax2_inner < 7; ++ax2_inner) {
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*128), 1, 128)] = x128(0)/*skip-init*/";
        for (int32_t ry = 0; ry < 3; ++ry) {
          for (int32_t rx = 0; rx < 3; ++rx) {
            int32_t _7 = ax0_ax1_fused_ax2_outer_fused >> 2;
            int32_t _8 = _7 * 3840;
            int32_t _9 = ry * 3840;
            int32_t _10 = _8 + _9;
            int32_t _11 = ax0_ax1_fused_ax2_outer_fused & 3;
            int32_t _12 = _11 * 896;
            int32_t _13 = _10 + _12;
            int32_t _14 = ax2_inner * 128;
            int32_t _15 = _13 + _14;
            int32_t _16 = rx * 128;
            int32_t _17 = _15 + _16;
            int32_t _18 = _17 + rc_outer;
            int32_t _19 = _18 % 64;
            bool _20 = _19 == 0;
            if (_20) {
              {
                Request request(Request::Type::RowLoad_RF);
                request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                sys->sendRequest(request);
              }
            }
            {
              Request request(Request::Type::RowLoad);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((ry*49152) + (rx*16384)) + (rc.outer*128)), 1, 128)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((ry*49152) + (rx*16384)) + (rc.outer*128)), 1, 128)]*/);
              request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*PaddedInput[((((((tir.shift_right(ax0.ax1.fused.ax2.outer.fused, 2)*3840) + (ry*3840)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.fused, 3)*896)) + (ax2.inner*128)) + (rx*128)) + rc.outer)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*128), 1, 128)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
      void* _21 = (void*) "Conv2dOutput[ramp(0, 1, 128)] = x128(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 128, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 128)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*128), 1, 128)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 128)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*128), 1, 128)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 128)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 72), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 48), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 72), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 48), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  int32_t _22 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _23 = _22 != 0;
  if (_23) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast);

int32_t tvmgen_default_fused_layout_transform_nn_batch_flatten_cast(System *sys) {
  void* _1 = nullptr;
  for (int32_t ax1_outer = 0; ax1_outer < 32; ++ax1_outer) {
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_layout_transform_nn_batch_flatten_cast__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_layout_transform_nn_batch_flatten_cast", tvmgen_default_fused_layout_transform_nn_batch_flatten_cast);

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
            for (int32_t rc_inner = 0; rc_inner < 2; ++rc_inner) {
              int32_t _7 = ax0_ax1_fused_ax2_outer_fused / 7;
              int32_t _8 = _7 * 4096;
              int32_t _9 = ry * 4096;
              int32_t _10 = _8 + _9;
              int32_t _11 = ax0_ax1_fused_ax2_outer_fused % 7;
              int32_t _12 = _11 * 512;
              int32_t _13 = _10 + _12;
              int32_t _14 = ax2_inner * 256;
              int32_t _15 = _13 + _14;
              int32_t _16 = rx * 256;
              int32_t _17 = _15 + _16;
              int32_t _18 = rc_outer * 2;
              int32_t _19 = _17 + _18;
              int32_t _20 = _19 + rc_inner;
              int32_t _21 = _20 % 64;
              bool _22 = _21 == 0;
              if (_22) {
                {
                  Request request(Request::Type::RowLoad_RF);
                  request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
                  request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                  sys->sendRequest(request);
                }
              }
              {
                Request request(Request::Type::RowLoad);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp(((((ry*196608) + (rx*65536)) + (rc.outer*512)) + (rc.inner*256)), 1, 256)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul_CRAM_RF);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp(((((ry*196608) + (rx*65536)) + (rc.outer*512)) + (rc.inner*256)), 1, 256)]*/);
                request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*PaddedInput[((((((((ax0.ax1.fused.ax2.outer.fused/7)*4096) + (ry*4096)) + ((ax0.ax1.fused.ax2.outer.fused % 7)*512)) + (ax2.inner*256)) + (rx*256)) + (rc.outer*2)) + rc.inner)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                sys->sendRequest(request);
              }
            }
          }
        }
      }
      void* _23 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 128, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  int32_t _24 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _25 = _24 != 0;
  if (_25) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_1__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_1", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_1);

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
          for (int32_t rc_inner = 0; rc_inner < 4; ++rc_inner) {
            int32_t _7 = ax0_ax1_fused_ax2_fused_ax3_outer_fused / 14;
            int32_t _8 = _7 * 4608;
            int32_t _9 = ry * 4608;
            int32_t _10 = _8 + _9;
            int32_t _11 = ax0_ax1_fused_ax2_fused_ax3_outer_fused % 14;
            int32_t _12 = _11 >> 1;
            int32_t _13 = _12 * 512;
            int32_t _14 = _10 + _13;
            int32_t _15 = rx * 512;
            int32_t _16 = _14 + _15;
            int32_t _17 = rc_outer * 4;
            int32_t _18 = _16 + _17;
            int32_t _19 = _18 + rc_inner;
            int32_t _20 = _19 % 64;
            bool _21 = _20 == 0;
            if (_21) {
              {
                Request request(Request::Type::RowLoad_RF);
                request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                sys->sendRequest(request);
              }
            }
            {
              Request request(Request::Type::RowLoad);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((((ry*786432) + (rx*262144)) + (rc.outer*2048)) + (rc.inner*512)) + (tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256)), 1, 256)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((((ry*786432) + (rx*262144)) + (rc.outer*2048)) + (rc.inner*512)) + (tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256)), 1, 256)]*/);
              request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*PaddedInput[(((((((ax0.ax1.fused.ax2.fused.ax3.outer.fused/14)*4608) + (ry*4608)) + (tir.shift_right((ax0.ax1.fused.ax2.fused.ax3.outer.fused % 14), 1)*512)) + (rx*512)) + (rc.outer*4)) + rc.inner)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
    }
    void* _22 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t rc_outer_v = 128, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  int32_t _23 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _24 = _23 != 0;
  if (_24) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_2__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_2", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_2);

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
        int32_t _3 = k_outer * 4;
        int32_t _4 = _3 + k_inner;
        int32_t _5 = _4 % 64;
        bool _6 = _5 == 0;
        if (_6) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 62), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((ax0.ax1.outer.fused*128000) + (k.outer*1000)) + (k.inner*250)), 1, 250)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 78), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 62), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((ax0.ax1.outer.fused*128000) + (k.outer*1000)) + (k.inner*250)), 1, 250)]*/);
          request.addOperand(ax0_ax1_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*placeholder[((k.outer*4) + k.inner)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 78), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*compute.rf[ramp((k.outer*250), 1, 250)]*/);
          sys->sendRequest(request);
        }
      }
    }
    void* _7 = (void*) "compute[ramp(0, 1, 250)] = x250(0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t k_outer_v = 128, k_outer_v_cnt = 1; k_outer_v >= 1; k_outer_v -= max(k_outer_v / 2, 1), ++k_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 31), 0, PrecisionT::Precision{0, 32, 0} /*compute[ramp(0, 1, 250)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*compute.rf[ramp((k.outer.v*250), 1, 250)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 31), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 31), 0, PrecisionT::Precision{0, 32, 0} /*compute[ramp(0, 1, 250)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*compute.rf[ramp((k.outer.v*250), 1, 250)]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 62), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((ax0.ax1.outer.fused*250), 1, 250)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 93), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 31), 0, PrecisionT::Precision{0, 32, 0} /*compute[ramp(0, 1, 250)]*/);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 62), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((ax0.ax1.outer.fused*250), 1, 250)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 125), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 93), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(ax0_ax1_outer_fused * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_outer_fused, 0, 125), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  // freed compute
  // freed compute_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_contrib_dense_pack_add_cast_multiply__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_contrib_dense_pack_add_cast_multiply", tvmgen_default_fused_nn_contrib_dense_pack_add_cast_multiply);

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
            for (int32_t rc_inner = 0; rc_inner < 2; ++rc_inner) {
              int32_t _7 = ax0_ax1_fused_ax2_outer_fused / 7;
              int32_t _8 = _7 * 4096;
              int32_t _9 = ry * 4096;
              int32_t _10 = _8 + _9;
              int32_t _11 = ax0_ax1_fused_ax2_outer_fused % 7;
              int32_t _12 = _11 * 512;
              int32_t _13 = _10 + _12;
              int32_t _14 = ax2_inner * 256;
              int32_t _15 = _13 + _14;
              int32_t _16 = rx * 256;
              int32_t _17 = _15 + _16;
              int32_t _18 = rc_outer * 2;
              int32_t _19 = _17 + _18;
              int32_t _20 = _19 + rc_inner;
              int32_t _21 = _20 % 64;
              bool _22 = _21 == 0;
              if (_22) {
                {
                  Request request(Request::Type::RowLoad_RF);
                  request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
                  request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                  sys->sendRequest(request);
                }
              }
              {
                Request request(Request::Type::RowLoad);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp(((((ry*196608) + (rx*65536)) + (rc.outer*512)) + (rc.inner*256)), 1, 256)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul_CRAM_RF);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp(((((ry*196608) + (rx*65536)) + (rc.outer*512)) + (rc.inner*256)), 1, 256)]*/);
                request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*PaddedInput[((((((((ax0.ax1.fused.ax2.outer.fused/7)*4096) + (ry*4096)) + ((ax0.ax1.fused.ax2.outer.fused % 7)*512)) + (ax2.inner*256)) + (rx*256)) + (rc.outer*2)) + rc.inner)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                sys->sendRequest(request);
              }
            }
          }
        }
      }
      void* _23 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 128, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(((ax0.ax1.fused.ax2.outer.fused*512) + (ax2.inner*256)), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 72), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(((ax0.ax1.fused.ax2.outer.fused*512) + (ax2.inner*256)), 1, 256)]*/);
        request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 64, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 72), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  int32_t _24 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _25 = _24 != 0;
  if (_25) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__2__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__2", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__2);

int32_t tvmgen_default_fused_nn_softmax(System *sys) {
  void* _1 = nullptr;
  for (int32_t i1 = 0; i1 < 1000; ++i1) {
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_softmax__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_softmax", tvmgen_default_fused_nn_softmax);

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
            for (int32_t rc_inner = 0; rc_inner < 4; ++rc_inner) {
              {
                Request request(Request::Type::RowLoad);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[(x256(((((ry*12288) + (rx*4096)) + (rc.outer*256)) + (rc.inner*64))) + (let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64)))))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*PaddedInput[((((x256(((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*3712) + (ry*3712)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(64)) in (let rdiv = (ramp(0, 1, 256)/x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(64))) + x256((rx*64))) + x256((rc.outer*4))) + x256(rc.inner))]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[(x256(((((ry*12288) + (rx*4096)) + (rc.outer*256)) + (rc.inner*64))) + (let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64)))))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
                sys->sendRequest(request);
              }
            }
          }
        }
      }
      void* _7 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 16, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
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
            request.addOperand(sys->getAddress(ax1, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            request.addOperand(sys->getAddress(ax1, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*tensor[ramp(((ax1*3584) + (ax2*64)), 1, 64)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd_CRAM_RF);
            request.addOperand(sys->getAddress(ax1, 0, 4), 0, PrecisionT::Precision{0, 8, 0} /**/);
            request.addOperand(sys->getAddress(ax1, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*tensor[ramp(((ax1*3584) + (ax2*64)), 1, 64)]*/);
            request.addOperand(sys->getAddress(ax1, 0, -1), 0, PrecisionT::Precision{0, 8, 0} /*ConstInt*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowStore);
            request.addOperand(sys->getAddress(ax1, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            request.addOperand(sys->getAddress(ax1, 0, 4), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
            sys->sendRequest(request);
          }
        }
      }
    }
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_max_pool2d__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_max_pool2d", tvmgen_default_fused_nn_max_pool2d);

int32_t tvmgen_default_fused_cast_1(System *sys) {
  void* _1 = nullptr;
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_cast_1__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_cast_1", tvmgen_default_fused_cast_1);

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
          for (int32_t rc_inner = 0; rc_inner < 2; ++rc_inner) {
            int32_t _7 = ax0_ax1_fused_ax2_fused_ax3_outer_fused / 14;
            int32_t _8 = _7 * 7680;
            int32_t _9 = ry * 3840;
            int32_t _10 = _8 + _9;
            int32_t _11 = ax0_ax1_fused_ax2_fused_ax3_outer_fused % 14;
            int32_t _12 = _11 >> 1;
            int32_t _13 = _12 * 512;
            int32_t _14 = _10 + _13;
            int32_t _15 = rx * 256;
            int32_t _16 = _14 + _15;
            int32_t _17 = rc_outer * 2;
            int32_t _18 = _16 + _17;
            int32_t _19 = _18 + rc_inner;
            int32_t _20 = _19 % 64;
            bool _21 = _20 == 0;
            if (_21) {
              {
                Request request(Request::Type::RowLoad_RF);
                request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                sys->sendRequest(request);
              }
            }
            {
              Request request(Request::Type::RowLoad);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((((ry*393216) + (rx*131072)) + (rc.outer*1024)) + (rc.inner*512)) + (tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256)), 1, 256)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((((ry*393216) + (rx*131072)) + (rc.outer*1024)) + (rc.inner*512)) + (tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256)), 1, 256)]*/);
              request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*PaddedInput[(((((((ax0.ax1.fused.ax2.fused.ax3.outer.fused/14)*7680) + (ry*3840)) + (tir.shift_right((ax0.ax1.fused.ax2.fused.ax3.outer.fused % 14), 1)*512)) + (rx*256)) + (rc.outer*2)) + rc.inner)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
    }
    void* _22 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t rc_outer_v = 128, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  int32_t _23 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _24 = _23 != 0;
  if (_24) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_5__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_5", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_5);

int32_t tvmgen_default_fused_cast_3(System *sys) {
  void* _1 = nullptr;
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_cast_3__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_cast_3", tvmgen_default_fused_cast_3);

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
  for (int32_t ax0_ax1_fused_ax2_outer_fused = 0; ax0_ax1_fused_ax2_outer_fused < 98; ++ax0_ax1_fused_ax2_outer_fused) {
    for (int32_t ax2_inner = 0; ax2_inner < 2; ++ax2_inner) {
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
        for (int32_t ry = 0; ry < 3; ++ry) {
          for (int32_t rx = 0; rx < 3; ++rx) {
            int32_t _7 = ax0_ax1_fused_ax2_outer_fused / 7;
            int32_t _8 = _7 * 7424;
            int32_t _9 = ry * 3712;
            int32_t _10 = _8 + _9;
            int32_t _11 = ax0_ax1_fused_ax2_outer_fused % 7;
            int32_t _12 = _11 * 512;
            int32_t _13 = _10 + _12;
            int32_t _14 = ax2_inner * 256;
            int32_t _15 = _13 + _14;
            int32_t _16 = rx * 128;
            int32_t _17 = _15 + _16;
            int32_t _18 = _17 + rc_outer;
            int32_t _19 = _18 % 64;
            bool _20 = _19 == 0;
            if (_20) {
              {
                Request request(Request::Type::RowLoad_RF);
                request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                sys->sendRequest(request);
              }
            }
            {
              Request request(Request::Type::RowLoad);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((ry*98304) + (rx*32768)) + (rc.outer*256)), 1, 256)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((ry*98304) + (rx*32768)) + (rc.outer*256)), 1, 256)]*/);
              request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*PaddedInput[(((((((ax0.ax1.fused.ax2.outer.fused/7)*7424) + (ry*3712)) + ((ax0.ax1.fused.ax2.outer.fused % 7)*512)) + (ax2.inner*256)) + (rx*128)) + rc.outer)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
      void* _21 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 128, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  int32_t _22 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _23 = _22 != 0;
  if (_23) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_3__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_3", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_3);

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
          for (int32_t rc_inner = 0; rc_inner < 4; ++rc_inner) {
            int32_t _7 = ax0_ax1_fused_ax2_fused_ax3_outer_fused / 14;
            int32_t _8 = _7 * 4608;
            int32_t _9 = ry * 4608;
            int32_t _10 = _8 + _9;
            int32_t _11 = ax0_ax1_fused_ax2_fused_ax3_outer_fused % 14;
            int32_t _12 = _11 >> 1;
            int32_t _13 = _12 * 512;
            int32_t _14 = _10 + _13;
            int32_t _15 = rx * 512;
            int32_t _16 = _14 + _15;
            int32_t _17 = rc_outer * 4;
            int32_t _18 = _16 + _17;
            int32_t _19 = _18 + rc_inner;
            int32_t _20 = _19 % 64;
            bool _21 = _20 == 0;
            if (_21) {
              {
                Request request(Request::Type::RowLoad_RF);
                request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                sys->sendRequest(request);
              }
            }
            {
              Request request(Request::Type::RowLoad);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((((ry*786432) + (rx*262144)) + (rc.outer*2048)) + (rc.inner*512)) + (tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256)), 1, 256)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((((ry*786432) + (rx*262144)) + (rc.outer*2048)) + (rc.inner*512)) + (tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256)), 1, 256)]*/);
              request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*PaddedInput[(((((((ax0.ax1.fused.ax2.fused.ax3.outer.fused/14)*4608) + (ry*4608)) + (tir.shift_right((ax0.ax1.fused.ax2.fused.ax3.outer.fused % 14), 1)*512)) + (rx*512)) + (rc.outer*4)) + rc.inner)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
    }
    void* _22 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t rc_outer_v = 128, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp((ax0.ax1.fused.ax2.fused.ax3.outer.fused*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 72), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp((ax0.ax1.fused.ax2.fused.ax3.outer.fused*256), 1, 256)]*/);
      request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 64, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 72), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /**/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  int32_t _23 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _24 = _23 != 0;
  if (_24) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__3__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__3", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__3);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__1(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)230400;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput_rf[512], 0
  // int32_t Conv2dOutput[512], 512
  for (int32_t ax0_ax1_fused_ax2_outer_fused = 0; ax0_ax1_fused_ax2_outer_fused < 112; ++ax0_ax1_fused_ax2_outer_fused) {
    for (int32_t ax2_inner = 0; ax2_inner < 7; ++ax2_inner) {
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*128), 1, 128)] = x128(0)/*skip-init*/";
        for (int32_t ry = 0; ry < 3; ++ry) {
          for (int32_t rx = 0; rx < 3; ++rx) {
            int32_t _7 = ax0_ax1_fused_ax2_outer_fused >> 2;
            int32_t _8 = _7 * 3840;
            int32_t _9 = ry * 3840;
            int32_t _10 = _8 + _9;
            int32_t _11 = ax0_ax1_fused_ax2_outer_fused & 3;
            int32_t _12 = _11 * 896;
            int32_t _13 = _10 + _12;
            int32_t _14 = ax2_inner * 128;
            int32_t _15 = _13 + _14;
            int32_t _16 = rx * 128;
            int32_t _17 = _15 + _16;
            int32_t _18 = _17 + rc_outer;
            int32_t _19 = _18 % 64;
            bool _20 = _19 == 0;
            if (_20) {
              {
                Request request(Request::Type::RowLoad_RF);
                request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                sys->sendRequest(request);
              }
            }
            {
              Request request(Request::Type::RowLoad);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((ry*49152) + (rx*16384)) + (rc.outer*128)), 1, 128)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((ry*49152) + (rx*16384)) + (rc.outer*128)), 1, 128)]*/);
              request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*PaddedInput[((((((tir.shift_right(ax0.ax1.fused.ax2.outer.fused, 2)*3840) + (ry*3840)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.fused, 3)*896)) + (ax2.inner*128)) + (rx*128)) + rc.outer)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*128), 1, 128)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
      void* _21 = (void*) "Conv2dOutput[ramp(0, 1, 128)] = x128(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 128, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 128)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*128), 1, 128)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 128)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*128), 1, 128)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 16), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 128)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 72), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 48), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 72), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(((ax0.ax1.fused.ax2.outer.fused*896) + (ax2.inner*128)), 1, 128)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 36), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(((ax0.ax1.fused.ax2.outer.fused*896) + (ax2.inner*128)), 1, 128)]*/);
        request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 64, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 36), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 64, 0} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 48), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 40), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  int32_t _22 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _23 = _22 != 0;
  if (_23) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__1__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__1", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_cast_fixed_point_multiply_2938127200454701776__1);

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
            for (int32_t rc_inner = 0; rc_inner < 4; ++rc_inner) {
              {
                Request request(Request::Type::RowLoad);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[(x256(((((ry*12288) + (rx*4096)) + (rc.outer*256)) + (rc.inner*64))) + (let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64)))))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*PaddedInput[((((x256(((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*3712) + (ry*3712)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(64)) in (let rdiv = (ramp(0, 1, 256)/x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(64))) + x256((rx*64))) + x256((rc.outer*4))) + x256(rc.inner))]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[(x256(((((ry*12288) + (rx*4096)) + (rc.outer*256)) + (rc.inner*64))) + (let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64)))))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
                sys->sendRequest(request);
              }
            }
          }
        }
      }
      void* _7 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 16, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(((ax0.ax1.fused.ax2.outer.outer.fused*1792) + (ax2.outer.inner*256)), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 72), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(((ax0.ax1.fused.ax2.outer.outer.fused*1792) + (ax2.outer.inner*256)), 1, 256)]*/);
        request.addOperand(ax0_ax1_fused_ax2_outer_outer_fused * 32, 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 64, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 72), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
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

int32_t tvmgen_default_fused_cast(System *sys) {
  void* _1 = nullptr;
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_cast__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_cast", tvmgen_default_fused_cast);

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
            for (int32_t rc_inner = 0; rc_inner < 4; ++rc_inner) {
              {
                Request request(Request::Type::RowLoad);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[(x256(((((ry*12288) + (rx*4096)) + (rc.outer*256)) + (rc.inner*64))) + (let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64)))))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*PaddedInput[((((x256(((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*3712) + (ry*3712)) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(64)) in (let rdiv = (ramp(0, 1, 256)/x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(64))) + x256((rx*64))) + x256((rc.outer*4))) + x256(rc.inner))]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[(x256(((((ry*12288) + (rx*4096)) + (rc.outer*256)) + (rc.inner*64))) + (let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64)))))]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
                sys->sendRequest(request);
              }
            }
          }
        }
      }
      void* _7 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 16, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(64)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(64))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(((ax0.ax1.fused.ax2.outer.outer.fused*1792) + (ax2.outer.inner*256)), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 72), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(((ax0.ax1.fused.ax2.outer.outer.fused*1792) + (ax2.outer.inner*256)), 1, 256)]*/);
        request.addOperand(ax0_ax1_fused_ax2_outer_outer_fused * 32, 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 64, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 72), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 64, 0} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
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
        for (int32_t rc_inner = 0; rc_inner < 4; ++rc_inner) {
          {
            Request request(Request::Type::RowMul);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[(((x256((((tir.shift_right(ax0.ax1.fused.ax2.outer.outer.fused, 1)*7168) + (tir.bitwise_and(ax0.ax1.fused.ax2.outer.outer.fused, 1)*1792)) + (ax2.outer.inner*256))) + ((let rmod = (ramp(0, 1, 256) % x256(128)) in (let rdiv = (ramp(0, 1, 256)/x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(128))) + x256((rc.outer*4))) + x256(rc.inner))]*/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[(x256(((rc.outer*512) + (rc.inner*128))) + (let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128)))))]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
            sys->sendRequest(request);
          }
        }
      }
      void* _3 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 16, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[(let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(((ax0.ax1.fused.ax2.outer.outer.fused*1792) + (ax2.outer.inner*256)), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast);

int32_t tvmgen_default_fused_divide_round_cast_clip_cast_cast_layout_transform(System *sys) {
  void* _1 = nullptr;
  for (int32_t ax1_outer = 0; ax1_outer < 112; ++ax1_outer) {
    // cram-array axis
    {
      int32_t ax1_inner_ax2_outer_fused = 0;
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[(((((let rmod = (ramp(0, 1, 168) % x168(3)) in select(((x168((bool)1) && (x168(0) <= rmod)) || (x168((bool)0) && (rmod <= x168(0)))), rmod, (rmod + x168(3))))*x168(50176)) + x168((ax1.outer*448))) + x168((ax1.inner.ax2.outer.fused*56))) + (let rmod = (ramp(0, 1, 168) % x168(3)) in (let rdiv = (ramp(0, 1, 168)/x168(3)) in select(((x168((bool)1) && (x168(0) <= rmod)) || (x168((bool)0) && (rmod <= x168(0)))), rdiv, (rdiv - x168(1))))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[(((((let rmod = (ramp(0, 1, 168) % x168(3)) in select(((x168((bool)1) && (x168(0) <= rmod)) || (x168((bool)0) && (rmod <= x168(0)))), rmod, (rmod + x168(3))))*x168(50176)) + x168((ax1.outer*448))) + x168((ax1.inner.ax2.outer.fused*56))) + (let rmod = (ramp(0, 1, 168) % x168(3)) in (let rdiv = (ramp(0, 1, 168)/x168(3)) in select(((x168((bool)1) && (x168(0) <= rmod)) || (x168((bool)0) && (rmod <= x168(0)))), rdiv, (rdiv - x168(1))))))]*/);
        request.addOperand(ax1_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[(((((let rmod = (ramp(0, 1, 168) % x168(3)) in select(((x168((bool)1) && (x168(0) <= rmod)) || (x168((bool)0) && (rmod <= x168(0)))), rmod, (rmod + x168(3))))*x168(50176)) + x168((ax1.outer*448))) + x168((ax1.inner.ax2.outer.fused*56))) + (let rmod = (ramp(0, 1, 168) % x168(3)) in (let rdiv = (ramp(0, 1, 168)/x168(3)) in select(((x168((bool)1) && (x168(0) <= rmod)) || (x168((bool)0) && (rmod <= x168(0)))), rdiv, (rdiv - x168(1))))))]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[(((((let rmod = (ramp(0, 1, 168) % x168(3)) in select(((x168((bool)1) && (x168(0) <= rmod)) || (x168((bool)0) && (rmod <= x168(0)))), rmod, (rmod + x168(3))))*x168(50176)) + x168((ax1.outer*448))) + x168((ax1.inner.ax2.outer.fused*56))) + (let rmod = (ramp(0, 1, 168) % x168(3)) in (let rdiv = (ramp(0, 1, 168)/x168(3)) in select(((x168((bool)1) && (x168(0) <= rmod)) || (x168((bool)0) && (rmod <= x168(0)))), rdiv, (rdiv - x168(1))))))]*/);
        request.addOperand(ax1_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax1_outer, 0, 42), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax1_outer, 0, 21), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 42), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(ax1_outer, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax1_outer, 0, 10), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 21), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax1_outer, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax1_outer, 0, 10), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_divide_round_cast_clip_cast_cast_layout_transform__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_divide_round_cast_clip_cast_cast_layout_transform", tvmgen_default_fused_divide_round_cast_clip_cast_cast_layout_transform);

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
            for (int32_t rc_inner = 0; rc_inner < 2; ++rc_inner) {
              int32_t _7 = ax0_ax1_fused_ax2_outer_fused / 7;
              int32_t _8 = _7 * 4096;
              int32_t _9 = ry * 4096;
              int32_t _10 = _8 + _9;
              int32_t _11 = ax0_ax1_fused_ax2_outer_fused % 7;
              int32_t _12 = _11 * 512;
              int32_t _13 = _10 + _12;
              int32_t _14 = ax2_inner * 256;
              int32_t _15 = _13 + _14;
              int32_t _16 = rx * 256;
              int32_t _17 = _15 + _16;
              int32_t _18 = rc_outer * 2;
              int32_t _19 = _17 + _18;
              int32_t _20 = _19 + rc_inner;
              int32_t _21 = _20 % 64;
              bool _22 = _21 == 0;
              if (_22) {
                {
                  Request request(Request::Type::RowLoad_RF);
                  request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
                  request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                  sys->sendRequest(request);
                }
              }
              {
                Request request(Request::Type::RowLoad);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp(((((ry*196608) + (rx*65536)) + (rc.outer*512)) + (rc.inner*256)), 1, 256)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul_CRAM_RF);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp(((((ry*196608) + (rx*65536)) + (rc.outer*512)) + (rc.inner*256)), 1, 256)]*/);
                request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*PaddedInput[((((((((ax0.ax1.fused.ax2.outer.fused/7)*4096) + (ry*4096)) + ((ax0.ax1.fused.ax2.outer.fused % 7)*512)) + (ax2.inner*256)) + (rx*256)) + (rc.outer*2)) + rc.inner)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
                sys->sendRequest(request);
              }
            }
          }
        }
      }
      void* _23 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 128, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  int32_t _24 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _25 = _24 != 0;
  if (_25) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_4__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_4", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_4);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2(System *sys) {
  void* _1 = nullptr;
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_outer_fused = 0; ax0_ax1_fused_ax2_fused_ax3_outer_fused < 98; ++ax0_ax1_fused_ax2_fused_ax3_outer_fused) {
    // cram-array axis
    {
      int32_t rc_outer = 0;
      void* _2 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
      for (int32_t rc_inner = 0; rc_inner < 2; ++rc_inner) {
        int32_t _3 = ax0_ax1_fused_ax2_fused_ax3_outer_fused / 14;
        int32_t _4 = _3 * 7168;
        int32_t _5 = ax0_ax1_fused_ax2_fused_ax3_outer_fused % 14;
        int32_t _6 = _5 >> 1;
        int32_t _7 = _6 * 512;
        int32_t _8 = _4 + _7;
        int32_t _9 = rc_outer * 2;
        int32_t _10 = _8 + _9;
        int32_t _11 = _10 + rc_inner;
        int32_t _12 = _11 % 64;
        bool _13 = _12 == 0;
        if (_13) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((rc.outer*1024) + (rc.inner*512)) + (tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256)), 1, 256)]*/);
          request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*placeholder[(((((ax0.ax1.fused.ax2.fused.ax3.outer.fused/14)*7168) + (tir.shift_right((ax0.ax1.fused.ax2.fused.ax3.outer.fused % 14), 1)*512)) + (rc.outer*2)) + rc.inner)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
    }
    void* _14 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t rc_outer_v = 128, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((ax0.ax1.fused.ax2.fused.ax3.outer.fused*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2);

int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1(System *sys) {
  void* _1 = nullptr;
  // int32_t Conv2dOutput_rf[1024], 0
  // int32_t Conv2dOutput[1024], 1024
  for (int32_t ax0_ax1_fused_ax2_outer_fused = 0; ax0_ax1_fused_ax2_outer_fused < 98; ++ax0_ax1_fused_ax2_outer_fused) {
    for (int32_t ax2_inner = 0; ax2_inner < 2; ++ax2_inner) {
      // cram-array axis
      {
        int32_t rc_outer = 0;
        void* _2 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256(0)/*skip-init*/";
        int32_t _3 = ax0_ax1_fused_ax2_outer_fused / 7;
        int32_t _4 = _3 * 7168;
        int32_t _5 = ax0_ax1_fused_ax2_outer_fused % 7;
        int32_t _6 = _5 * 512;
        int32_t _7 = _4 + _6;
        int32_t _8 = ax2_inner * 256;
        int32_t _9 = _7 + _8;
        int32_t _10 = _9 + rc_outer;
        int32_t _11 = _10 % 64;
        bool _12 = _11 == 0;
        if (_12) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((rc.outer*256), 1, 256)]*/);
          request.addOperand(ax0_ax1_fused_ax2_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*placeholder[(((((ax0.ax1.fused.ax2.outer.fused/7)*7168) + ((ax0.ax1.fused.ax2.outer.fused % 7)*512)) + (ax2.inner*256)) + rc.outer)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      void* _13 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rc_outer_v = 128, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp(((ax0.ax1.fused.ax2.outer.fused*512) + (ax2.inner*256)), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1);

int32_t tvmgen_default_fused_cast_2(System *sys) {
  void* _1 = nullptr;
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_cast_2__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_cast_2", tvmgen_default_fused_cast_2);

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
          for (int32_t rc_inner = 0; rc_inner < 4; ++rc_inner) {
            int32_t _7 = ax0_ax1_fused_ax2_fused_ax3_outer_fused / 14;
            int32_t _8 = _7 * 4608;
            int32_t _9 = ry * 4608;
            int32_t _10 = _8 + _9;
            int32_t _11 = ax0_ax1_fused_ax2_fused_ax3_outer_fused % 14;
            int32_t _12 = _11 >> 1;
            int32_t _13 = _12 * 512;
            int32_t _14 = _10 + _13;
            int32_t _15 = rx * 512;
            int32_t _16 = _14 + _15;
            int32_t _17 = rc_outer * 4;
            int32_t _18 = _16 + _17;
            int32_t _19 = _18 + rc_inner;
            int32_t _20 = _19 % 64;
            bool _21 = _20 == 0;
            if (_21) {
              {
                Request request(Request::Type::RowLoad_RF);
                request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
                sys->sendRequest(request);
              }
            }
            {
              Request request(Request::Type::RowLoad);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((((ry*786432) + (rx*262144)) + (rc.outer*2048)) + (rc.inner*512)) + (tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256)), 1, 256)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 16, 0} /*placeholder[ramp((((((ry*786432) + (rx*262144)) + (rc.outer*2048)) + (rc.inner*512)) + (tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256)), 1, 256)]*/);
              request.addOperand(ax0_ax1_fused_ax2_fused_ax3_outer_fused * 32, 0, PrecisionT::Precision{0, 16, 0} /*PaddedInput[(((((((ax0.ax1.fused.ax2.fused.ax3.outer.fused/14)*4608) + (ry*4608)) + (tir.shift_right((ax0.ax1.fused.ax2.fused.ax3.outer.fused % 14), 1)*512)) + (rx*512)) + (rc.outer*4)) + rc.inner)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
    }
    void* _22 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t rc_outer_v = 128, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 32), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 192), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 64, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 144), 0, PrecisionT::Precision{0, 64, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 64, 0} /*placeholder[ramp((tir.bitwise_and(ax0.ax1.fused.ax2.fused.ax3.outer.fused, 1)*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd_CRAM_RF);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, -1), 0, PrecisionT::Precision{0, 32, 0} /*ConstInt*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_ax3_outer_fused, 0, 80), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_rf
  int32_t _23 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _24 = _23 != 0;
  if (_24) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_6__ = pimsim::registerFunc("resnet18_v1__quantized_tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_6", tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_clip_cast_cast_6);

} // resnet18_v1__quantized
