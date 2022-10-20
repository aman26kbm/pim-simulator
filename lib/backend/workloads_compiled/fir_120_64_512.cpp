// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t fir_120_64_512(System *sys) {
  void* _1 = nullptr;
  // int16_t c_global[1024], 0
  // int16_t c_global_rf[1024], 1024
  // int16_t a_global[1024], 2048
  for (int32_t x_outer = 0; x_outer < 120; ++x_outer) {
    // cram-array axis
    {
      int32_t x_inner_outer_outer = 0;
      for (int32_t x_inner_outer_inner = 0; x_inner_outer_inner < 64; ++x_inner_outer_inner) {
        void* _2 = (void*) "c.global[ramp(0, 1, 512)] = x512((int16)0)/*skip-init*/";
        // cram-array axis
        {
          int32_t rv = 0;
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(x_outer, 0, 64), 32768, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(x_outer, 0, 32), 32768, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 64), 32768, PrecisionT::Precision{0, 16, 0} /*a.global[ramp(0, 1, 512)]*/);
            request.addOperand(x_outer * 32, 32768, PrecisionT::Precision{0, 16, 0} /*b[rv]*/);
            sys->sendRequest(request);
          }
        }
        #define max(a,b) ((a)>(b)?(a):(b))
        for (int32_t rv_v_inner = 32, rv_v_inner_cnt = 1; rv_v_inner >= 1; rv_v_inner -= max(rv_v_inner / 2, 1), ++rv_v_inner_cnt) {
        #undef max
          {
            Request request(Request::Type::BlockSend_Receive);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /*c.global[ramp(0, 1, 512)]*/);
            request.addOperand(sys->getAddress(x_outer, 0, 32), 32768, PrecisionT::Precision{0, 16, 0} /*c.global.rf[ramp((rv.v.inner*512), 1, 512)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /*c.global[ramp(0, 1, 512)]*/);
            request.addOperand(sys->getAddress(x_outer, 0, 32), 32768, PrecisionT::Precision{0, 16, 0} /*c.global.rf[ramp((rv.v.inner*512), 1, 512)]*/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 1024, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 1024, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed a_global
  // freed c_global_rf
  // freed c_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_fir_120_64_512__ = pimsim::registerFunc("fir_120_64_512", fir_120_64_512);

