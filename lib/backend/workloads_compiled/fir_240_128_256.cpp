// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t fir_240_128_256(System *sys) {
  void* _1 = nullptr;
  // int16_t c_global[512], 0
  // int16_t c_global_rf[512], 512
  // int16_t a_global[512], 1024
  for (int32_t x_outer = 0; x_outer < 240; ++x_outer) {
    for (int32_t x_inner_outer_inner = 0; x_inner_outer_inner < 2; ++x_inner_outer_inner) {
      void* _2 = (void*) "c.global[ramp(0, 1, 256)] = x256((int16)0)/*skip-init*/";
      for (int32_t rv_v_outer = 0; rv_v_outer < 2; ++rv_v_outer) {
        // cram-array axis
        {
          int32_t rv = 0;
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(x_outer, 0, 32), 0, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
          int32_t _3 = rv_v_outer * 128;
          int32_t _4 = _3 + rv;
          int32_t _5 = _4 % 64;
          bool _6 = _5 == 0;
          if (_6) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(x_outer * 32, 0, PrecisionT::Precision{0, 16, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(x_outer, 0, 16), 0, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 32), 0, PrecisionT::Precision{0, 16, 0} /*a.global[ramp(0, 1, 256)]*/);
            request.addOperand(x_outer * 32, 0, PrecisionT::Precision{0, 16, 0} /*b[((rv.v.outer*128) + rv)]*/);
            sys->sendRequest(request);
          }
        }
        #define max(a,b) ((a)>(b)?(a):(b))
        for (int32_t rv_v_inner = 128, rv_v_inner_cnt = 1; rv_v_inner >= 1; rv_v_inner -= max(rv_v_inner / 2, 1), ++rv_v_inner_cnt) {
        #undef max
          {
            Request request(Request::Type::BlockSend_Receive);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*c.global[ramp(0, 1, 256)]*/);
            request.addOperand(sys->getAddress(x_outer, 0, 16), 0, PrecisionT::Precision{0, 16, 0} /*c.global.rf[ramp((rv.v.inner*256), 1, 256)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*c.global[ramp(0, 1, 256)]*/);
            request.addOperand(sys->getAddress(x_outer, 0, 16), 0, PrecisionT::Precision{0, 16, 0} /*c.global.rf[ramp((rv.v.inner*256), 1, 256)]*/);
            sys->sendRequest(request);
          }
        }
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed a_global
  // freed c_global_rf
  // freed c_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_fir_240_128_256__ = pimsim::registerFunc("fir_240_128_256", fir_240_128_256);

