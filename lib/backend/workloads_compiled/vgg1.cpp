// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t vgg1(System *sys) {
  void* _1 = nullptr;
  // float32 Conv2dOutput[4096], 0
  uint64_t _2 = (uint64_t)65536;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 2, 32);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  for (int32_t ax1_outer = 0; ax1_outer < 56; ++ax1_outer) {
    // cram-array axis
    {
      int32_t ax1_inner = 0;
      // cram-array axis
      {
        int32_t ax2 = 0;
        for (int32_t yy = 0; yy < 4; ++yy) {
          for (int32_t xx_inner_ff_fused = 0; xx_inner_ff_fused < 1; ++xx_inner_ff_fused) {
            void* _6 = (void*) "Conv2dOutput[((yy*256) + xx.inner.ff.fused)] = 0f/*skip-init*/";
            for (int32_t ry = 0; ry < 3; ++ry) {
              for (int32_t rx = 0; rx < 3; ++rx) {
                for (int32_t rc = 0; rc < 3; ++rc) {
                  {
                    Request request(Request::Type::RowMul_CRAM_RF);
                    request.addOperand(sys->getAddress(ax1_outer, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /**/);
                    request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[((((ry*576) + (rx*192)) + (rc*64)) + tir.bitwise_and(xx.inner.ff.fused, 63))]*/);
                    request.addOperand(ax1_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
                    sys->sendRequest(request);
                  }
                  {
                    Request request(Request::Type::RowAdd);
                    request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
                    request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput[((yy*256) + xx.inner.ff.fused)]*/);
                    request.addOperand(sys->getAddress(ax1_outer, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /**/);
                    sys->sendRequest(request);
                  }
                }
              }
            }
          }
        }
        // cram-array axis
        {
          int32_t rc1 = 0;
          for (int32_t nn_yy_fused_xx_fused_ff_fused = 0; nn_yy_fused_xx_fused_ff_fused < 1; ++nn_yy_fused_xx_fused_ff_fused) {
            void* _7 = (void*) "Conv2dOutput.rf[((rc*256) + nn.yy.fused.xx.fused.ff.fused)] = 0f/*skip-init*/";
            for (int32_t ry1 = 0; ry1 < 3; ++ry1) {
              for (int32_t rx1 = 0; rx1 < 3; ++rx1) {
                {
                }
                {
                  Request request(Request::Type::RowMul_CRAM_RF);
                  request.addOperand(sys->getAddress(ax1_outer, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /**/);
                  request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[((((ry*12288) + (rx*4096)) + (rc*64)) + tir.bitwise_and(nn.yy.fused.xx.fused.ff.fused, 63))]*/);
                  request.addOperand(ax1_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
                  sys->sendRequest(request);
                }
                {
                }
                {
                  Request request(Request::Type::RowAdd);
                  request.addOperand(sys->getAddress(ax1_outer, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /**/);
                  request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput.rf[((rc*256) + nn.yy.fused.xx.fused.ff.fused)]*/);
                  request.addOperand(sys->getAddress(ax1_outer, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /**/);
                  sys->sendRequest(request);
                }
                {
                }
              }
            }
          }
        }
        void* _8 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256(0f)/*skip-init*/";
        for (int32_t rc_v = 0; rc_v < 64; ++rc_v) {
          {
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput[ramp(0, 1, 256)]*/);
            request.addOperand(sys->getAddress(ax1_outer, 0, 128), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput.rf[ramp((rc.v*256), 1, 256)]*/);
            sys->sendRequest(request);
          }
        }
        for (int32_t ax3 = 0; ax3 < 1; ++ax3) {
          void* _9 = (void*) "tensor[((((ax1.outer*14336) + (ax1.inner*7168)) + (ax2*64)) + ax3)] = -3.40282e+38f/*skip-init*/";
          for (int32_t rv0 = 0; rv0 < 2; ++rv0) {
            for (int32_t rv1 = 0; rv1 < 2; ++rv1) {
              {
                Request request(Request::Type::RowAdd_CRAM_RF);
                request.addOperand(sys->getAddress(ax1_outer, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput[(((rv0*128) + (rv1*64)) + ax3)]*/);
                request.addOperand(sys->getAddress(ax1_outer, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
                sys->sendRequest(request);
              }
              {
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax1_outer, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(ax1_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*tensor[((((ax1.outer*14336) + (ax1.inner*7168)) + (ax2*64)) + ax3)]*/);
                request.addOperand(sys->getAddress(ax1_outer, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /**/);
                sys->sendRequest(request);
              }
              {
              }
            }
          }
        }
      }
    }
  }
  int32_t _10 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _11 = _10 != 0;
  if (_11) {
    return -1;
      }
  // freed Conv2dOutput
  return 0;
}

static __attribute__((unused)) Registry::Entry &_vgg1__ = pimsim::registerFunc("vgg1", vgg1);

