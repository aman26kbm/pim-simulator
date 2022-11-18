// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t mv_120_1024_128(System *sys) {
  void* _1 = nullptr;
  // int32_t c_global_rf[512], 0
  // int8_t a_global[128], 512
  // int32_t c_global[512], 640
  for (int32_t x_outer_outer = 0; x_outer_outer < 120; ++x_outer_outer) {
    for (int32_t x_outer_inner = 0; x_outer_inner < 4; ++x_outer_inner) {
      // cram-array axis
      {
        int32_t k_outer = 0;
        void* _2 = (void*) "c.global.rf[ramp((k.outer*128), 1, 128)] = x128(0)/*skip-init*/";
        for (int32_t k_inner = 0; k_inner < 2; ++k_inner) {
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 16), 131072, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 131072, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1024, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1024, PrecisionT::Precision{0, 8, 0} /*b[((k.outer*2) + k.inner)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowMul);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 36), 131072, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 16), 131072, PrecisionT::Precision{0, 8, 0} /*a.global[ramp(0, 1, 128)]*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1024, PrecisionT::Precision{0, 8, 0} /*b[((k.outer*2) + k.inner)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 131072, PrecisionT::Precision{0, 32, 0} /**/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 36), 131072, PrecisionT::Precision{0, 32, 0} /**/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 131072, PrecisionT::Precision{0, 32, 0} /*c.global.rf[ramp((k.outer*128), 1, 128)]*/);
            sys->sendRequest(request);
          }
        }
      }
      void* _3 = (void*) "c.global[ramp(0, 1, 128)] = x128(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t k_outer_v = 1024, k_outer_v_cnt = 1; k_outer_v >= 1; k_outer_v -= max(k_outer_v / 2, 1), ++k_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 20), 131072, PrecisionT::Precision{0, 32, 0} /*c.global[ramp(0, 1, 128)]*/);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 131072, PrecisionT::Precision{0, 32, 0} /*c.global.rf[ramp((k.outer.v*128), 1, 128)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 20), 131072, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 20), 131072, PrecisionT::Precision{0, 32, 0} /*c.global[ramp(0, 1, 128)]*/);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 131072, PrecisionT::Precision{0, 32, 0} /*c.global.rf[ramp((k.outer.v*128), 1, 128)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 128, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 20), 128, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed c_global
  // freed a_global
  // freed c_global_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_mv_120_1024_128__ = pimsim::registerFunc("mv_120_1024_128", mv_120_1024_128);

