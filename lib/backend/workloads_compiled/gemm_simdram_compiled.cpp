// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t gemm_simdram_compiled(System *sys) {
  void* _1 = nullptr;
  // int8_t b_global[0], 0
  // int16_t compute_global_rf[512], 2048
  // int16_t compute_global[512], 2560
  for (int32_t y_outer_xo_outer_fused = 0; y_outer_xo_outer_fused < 16; ++y_outer_xo_outer_fused) {
    for (int32_t y_inner = 0; y_inner < 16; ++y_inner) {
      for (int32_t ax1_outer = 0; ax1_outer < 8; ++ax1_outer) {
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 0), 256 * 256, PrecisionT::Precision{0, 4, 0} /*CRAM*/);
          request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 0), 256 * 256, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
          sys->sendRequest(request);
        }
      }
      for (int32_t xo_inner = 0; xo_inner < 30; ++xo_inner) {
        // cram-array axis
        {
          int32_t rv_outer = 0;
          void* _2 = (void*) "compute.global.rf[ramp((rv.outer*256), 1, 256)] = x256((int16)0)/*skip-init*/";
          for (int32_t rv_inner = 0; rv_inner < 8; ++rv_inner) {
            static int state = 0;
            int index = (((((y_outer_xo_outer_fused & 7)*15728640) + (xo_inner*524288)) + (rv_outer*2048)) + (rv_inner*256));
            index /= 256;
            {
              Request request(Request::Type::RowLoad);
              request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 0), 65536, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 96), 65536, PrecisionT::Precision{0, 4, 0} /*a[ramp(((((tir.bitwise_and(y.outer.xo.outer.fused, 7)*15728640) + (xo.inner*524288)) + (rv.outer*2048)) + (rv.inner*256)), 1, 256)]*/);
              sys->sendRequest(request);

              // int sender = y_outer_xo_outer_fused;
              // int receiver = sender % 2 == 0 ? sender + 1 : sender - 1;
              // {
              //   Request request(Request::Type::TileReceive);
              //   request.addOperand(sys->getAddress(sender, 0, 0), 0, PrecisionT::Precision{0, 4, 0});
              //   request.addOperand(sys->getAddress(receiver, 0, 0), 0, PrecisionT::Precision{0, 4, 0});
              //   sys->sendRequest(request);
              // }
              // {
              //   Request request(Request::Type::TileSend);
              //   request.addOperand(sys->getAddress(sender, 0, 0), 0, PrecisionT::Precision{0, 4, 0});
              //   request.addOperand(sys->getAddress(receiver, 0, 0), 0, PrecisionT::Precision{0, 4, 0});
              //   sys->sendRequest(request);
              // }
            }
            {
              Request request(Request::Type::RowMul);
              request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 104), 65536, PrecisionT::Precision{0, 8, 0} /**/);
              request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 96), 65536, PrecisionT::Precision{0, 4, 0} /*a[ramp(((((tir.bitwise_and(y.outer.xo.outer.fused, 7)*15728640) + (xo.inner*524288)) + (rv.outer*2048)) + (rv.inner*256)), 1, 256)]*/);
              request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 4, 0} /*b.global[((rv.outer*8) + rv.inner)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 64), 65536, PrecisionT::Precision{0, 16, 0} /**/);
              request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 104), 65536, PrecisionT::Precision{0, 16, 0} /**/);
              request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 64), 65536, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
          }
        }
        void* _3 = (void*) "compute.global[ramp(0, 1, 256)] = x256((int16)0)/*skip-init*/";
        #define max(a,b) ((a)>(b)?(a):(b))
        for (int32_t rv_outer_v = 256, rv_outer_v_cnt = 1; rv_outer_v >= 1; rv_outer_v -= max(rv_outer_v / 2, 1), ++rv_outer_v_cnt) {
        #undef max
          {
            Request request(Request::Type::BlockSend_Receive);
            request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 16, 0} /*compute.global[ramp(0, 1, 256)]*/);
            request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 64), 256, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer.v*256), 1, 256)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 16, 0} /*compute.global[ramp(0, 1, 256)]*/);
            request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 64), 256, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer.v*256), 1, 256)]*/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 0), 256, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(y_outer_xo_outer_fused, 0, 80), 256, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed compute_global
  // freed compute_global_rf
  // freed b_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_gemm_simdram_compiled__ = pimsim::registerFunc("gemm_simdram_compiled", gemm_simdram_compiled);

