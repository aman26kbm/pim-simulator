// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t gemm_systolic_120_256_256_int4_int16(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)20480;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  uint64_t _6 = (uint64_t)1310720;
  void* _7 = TVMBackendAllocWorkspace(1, _1, _6, 0, 16);
  bool _8 = _7 == NULL;
  if (_8) {
    return -1;
      }
  for (int32_t xo_outer = 0; xo_outer < 12; ++xo_outer) {
    for (int32_t xo_inner = 0; xo_inner < 20; ++xo_inner) {
      for (int32_t ax1_outer = 0; ax1_outer < 4; ++ax1_outer) {
        // cram-array axis
        {
          int32_t rv_outer = 0;
          for (int32_t y_c = 0; y_c < 10; ++y_c) {
            void* _10 = (void*) "compute.global.rf[ramp(((rv.outer*2560) + (y.c*256)), 1, 256)] = x256((int16)0)/*skip-init*/";
            for (int32_t rv_inner = 0; rv_inner < 8; ++rv_inner) {
              {
                int32_t _11 = xo_outer * 10;
                int32_t _12 = _11 + y_c;
                if (_12 < 12) {
                  Request request(Request::Type::RowLoad);
                  request.addOperand(sys->getAddress(_12, 0, 0), 0, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
                  request.addOperand(sys->getAddress(_12, 0, 0), 0, PrecisionT::Precision{0, 4, 0} /*a[ramp(((((xo.outer*10485760) + (xo.inner*524288)) + (rv.outer*2048)) + (rv.inner*256)), 1, 256)]*/);
                  sys->sendRequest(request);
                }
                std::vector<int> recv;
                for (int i = 1; i < 10; ++i) {
                  recv.push_back(i * 12 + _12);
                }
                // sys->broadcast_p2p(sys->getAddress(_12, 0, 0), PrecisionT::Precision{0, 4, 0},
                //                    recv, 65536);
              }
              {
                Request request(Request::Type::RowMul);
                int32_t _13 = xo_outer * 10;
                int32_t _14 = _13 + y_c;
                request.addOperand(sys->getAddress(_14, 0, 8), 65536, PrecisionT::Precision{0, 8, 0} /**/);
                request.addOperand(sys->getAddress(_14, 0, 0), 65536, PrecisionT::Precision{0, 4, 0} /*a[ramp(((((xo.outer*10485760) + (xo.inner*524288)) + (rv.outer*2048)) + (rv.inner*256)), 1, 256)]*/);
                request.addOperand(sys->getAddress(_14, 0, 0), 256, PrecisionT::Precision{0, 4, 0} /*b[((((ax1.outer*20480) + (y.c*2048)) + (rv.outer*8)) + rv.inner)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                int32_t _15 = xo_outer * 10;
                int32_t _16 = _15 + y_c;
                request.addOperand(sys->getAddress(_16, 0, 32), 65536, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(_16, 0, 8), 65536, PrecisionT::Precision{0, 16, 0} /**/);
                request.addOperand(sys->getAddress(_16, 0, 16), 65536, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp(((rv.outer*2560) + (y.c*256)), 1, 256)]*/);
                sys->sendRequest(request);
              }
            }
          }
        }
        for (int32_t ax1_inner_init = 0; ax1_inner_init < 10; ++ax1_inner_init) {
          void* _17 = (void*) "compute.global[ramp(((ax1.outer*2560) + (ax1.inner.init*256)), 1, 256)] = x256((int16)0)/*skip-init*/";
        }
        #define max(a,b) ((a)>(b)?(a):(b))
        for (int32_t rv_outer_v = 256, rv_outer_v_cnt = 1; rv_outer_v >= 1; rv_outer_v -= max(rv_outer_v / 2, 1), ++rv_outer_v_cnt) {
        #undef max
          for (int32_t ax1_inner = 0; ax1_inner < 10; ++ax1_inner) {
            {
              Request request(Request::Type::BlockSend_Receive);
              int32_t _18 = xo_outer * 10;
              int32_t _19 = _18 + ax1_inner;
              request.addOperand(sys->getAddress(_19, 0, 16), 256, PrecisionT::Precision{0, 16, 0} /*compute.global[ramp(((ax1.outer*2560) + (ax1.inner*256)), 1, 256)]*/);
              request.addOperand(sys->getAddress(_19, 0, 32), 256, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp(((rv.outer.v*2560) + (ax1.inner*256)), 1, 256)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              int32_t _20 = xo_outer * 10;
              int32_t _21 = _20 + ax1_inner;
              request.addOperand(sys->getAddress(_21, 0, 48), 256, PrecisionT::Precision{0, 16, 0} /**/);
              request.addOperand(sys->getAddress(_21, 0, 16), 256, PrecisionT::Precision{0, 16, 0} /*compute.global[ramp(((ax1.outer*2560) + (ax1.inner*256)), 1, 256)]*/);
              request.addOperand(sys->getAddress(_21, 0, 32), 256, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp(((rv.outer.v*2560) + (ax1.inner*256)), 1, 256)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
      for (int32_t y_outer = 0; y_outer < 10; ++y_outer) {
        for (int32_t y_inner = 0; y_inner < 4; ++y_inner) {
          {
            Request request(Request::Type::RowStore);
            int32_t _22 = xo_outer * 10;
            int32_t _23 = _22 + y_outer;
            request.addOperand(sys->getAddress(_23, 0, 0), 256, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            request.addOperand(sys->getAddress(_23, 0, 32), 256, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
            sys->sendRequest(request);
          }
        }
      }
    }
  }
  int32_t _24 = TVMBackendFreeWorkspace(1, _1, _7);
  bool _25 = _24 != 0;
  if (_25) {
    return -1;
      }
  int32_t _27 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _28 = _27 != 0;
  if (_28) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_gemm_systolic_120_256_256_int4_int16__ = pimsim::registerFunc("gemm_systolic_120_256_256_int4_int16", gemm_systolic_120_256_256_int4_int16);

