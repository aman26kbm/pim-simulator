// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t vgg2(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)6653952;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 2, 32);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  uint64_t _6 = (uint64_t)6422528;
  void* _7 = TVMBackendAllocWorkspace(1, _1, _6, 2, 32);
  bool _8 = _7 == NULL;
  if (_8) {
    return -1;
      }
  uint64_t _10 = (uint64_t)262144;
  void* _11 = TVMBackendAllocWorkspace(1, _1, _10, 2, 32);
  bool _12 = _11 == NULL;
  if (_12) {
    return -1;
      }
  // float32 tensor_global[1024], 0
  // float32 Conv2dOutput[2048], 1024
  // skipped by pragma_skip
  for (int32_t ax1_outer = 0; ax1_outer < 45; ++ax1_outer) {
    for (int32_t ax1_inner = 0; ax1_inner < 3; ++ax1_inner) {
      for (int32_t ax2_outer_outer = 0; ax2_outer_outer < 14; ++ax2_outer_outer) {
        // cram-array axis
        {
          int32_t ax2_outer_inner = 0;
          // cram-array axis
          {
            int32_t rc = 0;
            void* _14 = (void*) "Conv2dOutput.rf[ramp((rc*256), 1, 256)] = x256(0f)/*skip-init*/";
            for (int32_t ry = 0; ry < 3; ++ry) {
              for (int32_t rx = 0; rx < 3; ++rx) {
                int32_t _15 = ax1_outer * 3;
                int32_t _16 = _15 + ax1_inner;
                bool _17 = _16 < 112;
                if (_17) {
                  {
                    Request request(Request::Type::RowMul);
                    request.addOperand(sys->getAddress(ax1_outer, 0, 144), 0, PrecisionT::Precision{1, 23, 8} /**/);
                    request.addOperand(sys->getAddress(ax1_outer, 0, 96), 0, PrecisionT::Precision{1, 23, 8} /*PaddedInput[(((x256((((((ax1.outer*21888) + (ax1.inner*7296)) + (ry*7296)) + (ax2.outer.outer*512)) + (ax2.outer.inner*128))) + ((let rmod = (ramp(0, 1, 256) % x256(128)) in (let rdiv = (ramp(0, 1, 256)/x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(64))) + x256((rx*64))) + x256(rc))]*/);
                    request.addOperand(sys->getAddress(ax1_outer, 0, 128), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[(x256((((ry*24576) + (rx*8192)) + (rc*128))) + (let rmod = (ramp(0, 1, 256) % x256(128)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(128)))))]*/);
                    sys->sendRequest(request);
                  }
                  {
                    Request request(Request::Type::RowAdd);
                    request.addOperand(sys->getAddress(ax1_outer, 0, 144), 0, PrecisionT::Precision{1, 23, 8} /**/);
                    request.addOperand(sys->getAddress(ax1_outer, 0, 96), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput.rf[ramp((rc*256), 1, 256)]*/);
                    request.addOperand(sys->getAddress(ax1_outer, 0, 144), 0, PrecisionT::Precision{1, 23, 8} /**/);
                    sys->sendRequest(request);
                  }
                }
              }
            }
          }
          int32_t _18 = ax1_outer * 3;
          int32_t _19 = _18 + ax1_inner;
          bool _20 = _19 < 112;
          if (_20) {
            void* _21 = (void*) "Conv2dOutput[ramp(((((ax1.outer*43008) + (ax1.inner*14336)) + (ax2.outer.outer*1024)) + (ax2.outer.inner*256)), 1, 256)] = x256(0f)/*skip-init*/";
          }
          #define max(a,b) ((a)>(b)?(a):(b))
          for (int32_t rc_v = 64, rc_v_cnt = 1; rc_v >= 1; rc_v -= max(rc_v / 2, 1), ++rc_v_cnt) {
          #undef max
            int32_t _22 = ax1_outer * 3;
            int32_t _23 = _22 + ax1_inner;
            bool _24 = _23 < 112;
            if (_24) {
              {
                Request request(Request::Type::BlockSend_Receive);
                request.addOperand(sys->getAddress(ax1_outer, 0, 128), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput[ramp(((((ax1.outer*43008) + (ax1.inner*14336)) + (ax2.outer.outer*1024)) + (ax2.outer.inner*256)), 1, 256)]*/);
                request.addOperand(sys->getAddress(ax1_outer, 0, 144), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput.rf[ramp((rc.v*256), 1, 256)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax1_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(ax1_outer, 0, 128), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput[ramp(((((ax1.outer*43008) + (ax1.inner*14336)) + (ax2.outer.outer*1024)) + (ax2.outer.inner*256)), 1, 256)]*/);
                request.addOperand(sys->getAddress(ax1_outer, 0, 144), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput.rf[ramp((rc.v*256), 1, 256)]*/);
                sys->sendRequest(request);
              }
            }
          }
        }
      }
    }
  }
  // freed Conv2dOutput
  // freed tensor_global
  int32_t _37 = TVMBackendFreeWorkspace(1, _1, _11);
  bool _38 = _37 != 0;
  if (_38) {
    return -1;
      }
  int32_t _40 = TVMBackendFreeWorkspace(1, _1, _7);
  bool _41 = _40 != 0;
  if (_41) {
    return -1;
      }
  int32_t _43 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _44 = _43 != 0;
  if (_44) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_vgg2__ = pimsim::registerFunc("vgg2", vgg2);

