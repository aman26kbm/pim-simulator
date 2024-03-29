// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t vgg5(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)401408;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 2, 32);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  uint64_t _6 = (uint64_t)524288;
  void* _7 = TVMBackendAllocWorkspace(1, _1, _6, 2, 32);
  bool _8 = _7 == NULL;
  if (_8) {
    return -1;
      }
  // float32 Conv2dOutput_repl_global[2048], 0
  // float32 Conv2dOutput[2048], 2048
  for (int32_t ax0_ax1_fused_ax2_fused_outer_outer = 0; ax0_ax1_fused_ax2_fused_outer_outer < 40; ++ax0_ax1_fused_ax2_fused_outer_outer) {
    for (int32_t ax0_ax1_fused_ax2_fused_outer_inner = 0; ax0_ax1_fused_ax2_fused_outer_inner < 3; ++ax0_ax1_fused_ax2_fused_outer_inner) {
      // cram-array axis
      {
        int32_t ax0_ax1_fused_ax2_fused_inner = 0;
        // cram-array axis
        {
          int32_t rc_outer = 0;
          void* _10 = (void*) "Conv2dOutput.rf[ramp((rc.outer*512), 1, 512)] = x512(0f)/*skip-init*/";
          for (int32_t ry = 0; ry < 3; ++ry) {
            for (int32_t rx = 0; rx < 3; ++rx) {
              for (int32_t rc_inner = 0; rc_inner < 2; ++rc_inner) {
                int32_t _11 = ax0_ax1_fused_ax2_fused_outer_outer * 6;
                int32_t _12 = ax0_ax1_fused_ax2_fused_outer_inner * 2;
                int32_t _13 = _11 + _12;
                int32_t _14 = _13 + ax0_ax1_fused_ax2_fused_inner;
                bool _15 = _14 < 196;
                if (_15) {
                  {
                    Request request(Request::Type::RowMul_CRAM_RF);
                    request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
                    request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer_outer, 0, 128), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[ramp(((((ry*786432) + (rx*262144)) + (rc.outer*1024)) + (rc.inner*512)), 1, 512)]*/);
                    request.addOperand(ax0_ax1_fused_ax2_fused_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
                    sys->sendRequest(request);
                  }
                  {
                    Request request(Request::Type::RowAdd);
                    request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer_outer, 0, 224), 0, PrecisionT::Precision{1, 23, 8} /**/);
                    request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer_outer, 0, 128), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput.rf[ramp((rc.outer*512), 1, 512)]*/);
                    request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
                    sys->sendRequest(request);
                  }
                }
              }
            }
          }
        }
        void* _16 = (void*) "Conv2dOutput.repl.global[ramp(0, 1, 512)] = x512(0f)/*skip-init*/";
        #define max(a,b) ((a)>(b)?(a):(b))
        for (int32_t rc_outer_v = 256, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
        #undef max
          int32_t _17 = ax0_ax1_fused_ax2_fused_outer_outer * 6;
          int32_t _18 = ax0_ax1_fused_ax2_fused_outer_inner * 2;
          int32_t _19 = _17 + _18;
          int32_t _20 = _19 + ax0_ax1_fused_ax2_fused_inner;
          bool _21 = _20 < 196;
          if (_21) {
            {
              Request request(Request::Type::BlockSend_Receive);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput.repl.global[ramp(0, 1, 512)]*/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer_outer, 0, 192), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput.rf[ramp((rc.outer.v*512), 1, 512)]*/);
              sys->sendRequest(request);
            }
            {
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput.repl.global[ramp(0, 1, 512)]*/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer_outer, 0, 192), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput.rf[ramp((rc.outer.v*512), 1, 512)]*/);
              sys->sendRequest(request);
            }
          }
        }
        int32_t _22 = ax0_ax1_fused_ax2_fused_outer_outer * 6;
        int32_t _23 = ax0_ax1_fused_ax2_fused_outer_inner * 2;
        int32_t _24 = _22 + _23;
        int32_t _25 = _24 + ax0_ax1_fused_ax2_fused_inner;
        bool _26 = _25 < 196;
        if (_26) {
          int32_t _27 = ax0_ax1_fused_ax2_fused_outer_outer * 3;
          int32_t _28 = _27 + ax0_ax1_fused_ax2_fused_outer_inner;
          bool _29 = _28 < 98;
          if (_29) {
            {
            }
          }
        }
      }
    }
  }
  for (int32_t ax0_ax1_fused_ax2_fused_outer = 0; ax0_ax1_fused_ax2_fused_outer < 45; ++ax0_ax1_fused_ax2_fused_outer) {
    for (int32_t ax0_ax1_fused_ax2_fused_inner1 = 0; ax0_ax1_fused_ax2_fused_inner1 < 2; ++ax0_ax1_fused_ax2_fused_inner1) {
      void* _30 = (void*) "Conv2dOutput.repl.global[ramp(0, 1, 512)] = x512(-3.40282e+38f)/*skip-init*/";
      for (int32_t rv0 = 0; rv0 < 2; ++rv0) {
        for (int32_t rv1 = 0; rv1 < 2; ++rv1) {
          // cram-array axis
          {
            int32_t rc_outer1 = 0;
            void* _31 = (void*) "Conv2dOutput.rf[ramp((rc.outer*512), 1, 512)] = x512(0f)/*skip-init*/";
            for (int32_t ry1 = 0; ry1 < 3; ++ry1) {
              for (int32_t rx1 = 0; rx1 < 3; ++rx1) {
                for (int32_t rc_inner1 = 0; rc_inner1 < 2; ++rc_inner1) {
                  int32_t _32 = ax0_ax1_fused_ax2_fused_outer * 2;
                  int32_t _33 = _32 + ax0_ax1_fused_ax2_fused_inner1;
                  bool _34 = _33 < 49;
                  if (_34) {
                    {
                      Request request(Request::Type::RowMul_CRAM_RF);
                      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer, 0, 224), 0, PrecisionT::Precision{1, 23, 8} /**/);
                      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer, 0, 192), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[ramp(((((ry*786432) + (rx*262144)) + (rc.outer*1024)) + (rc.inner*512)), 1, 512)]*/);
                      request.addOperand(ax0_ax1_fused_ax2_fused_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
                      sys->sendRequest(request);
                    }
                    {
                      Request request(Request::Type::RowAdd);
                      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer, 0, 288), 0, PrecisionT::Precision{1, 23, 8} /**/);
                      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer, 0, 192), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput.rf[ramp((rc.outer*512), 1, 512)]*/);
                      request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer, 0, 224), 0, PrecisionT::Precision{1, 23, 8} /**/);
                      sys->sendRequest(request);
                    }
                  }
                }
              }
            }
          }
          void* _35 = (void*) "Conv2dOutput[ramp(0, 1, 512)] = x512(0f)/*skip-init*/";
          #define max(a,b) ((a)>(b)?(a):(b))
          for (int32_t rc_outer_v1 = 256, rc_outer_v1_cnt = 1; rc_outer_v1 >= 1; rc_outer_v1 -= max(rc_outer_v1 / 2, 1), ++rc_outer_v1_cnt) {
          #undef max
            int32_t _36 = ax0_ax1_fused_ax2_fused_outer * 2;
            int32_t _37 = _36 + ax0_ax1_fused_ax2_fused_inner1;
            bool _38 = _37 < 49;
            if (_38) {
              {
                Request request(Request::Type::BlockSend_Receive);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer, 0, 64), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput[ramp(0, 1, 512)]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer, 0, 256), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput.rf[ramp((rc.outer.v*512), 1, 512)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer, 0, 64), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer, 0, 64), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput[ramp(0, 1, 512)]*/);
                request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer, 0, 256), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput.rf[ramp((rc.outer.v*512), 1, 512)]*/);
                sys->sendRequest(request);
              }
            }
          }
          int32_t _39 = ax0_ax1_fused_ax2_fused_outer * 2;
          int32_t _40 = _39 + ax0_ax1_fused_ax2_fused_inner1;
          bool _41 = _40 < 49;
          if (_41) {
            {
              Request request(Request::Type::RowAdd_CRAM_RF);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer, 0, 256), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer, 0, 64), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput[ramp(0, 1, 512)]*/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*Conv2dOutput.repl.global[ramp(0, 1, 512)]*/);
              request.addOperand(sys->getAddress(ax0_ax1_fused_ax2_fused_outer, 0, 256), 0, PrecisionT::Precision{1, 23, 8} /**/);
              sys->sendRequest(request);
            }
          }
        }
      }
      int32_t _42 = ax0_ax1_fused_ax2_fused_outer * 2;
      int32_t _43 = _42 + ax0_ax1_fused_ax2_fused_inner1;
      bool _44 = _43 < 49;
      if (_44) {
        {
        }
      }
    }
  }
  // freed Conv2dOutput
  // freed Conv2dOutput_repl_global
  int32_t _45 = TVMBackendFreeWorkspace(1, _1, _7);
  bool _46 = _45 != 0;
  if (_46) {
    return -1;
      }
  int32_t _48 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _49 = _48 != 0;
  if (_49) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_vgg5__ = pimsim::registerFunc("vgg5", vgg5);

