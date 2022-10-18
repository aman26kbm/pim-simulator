// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t mv_240_64_256(System *sys) {
  void* _1 = nullptr;
  // int32_t c_global_rf[1024], 0
  // int8_t a_global[256], 1024
  // int32_t c_global[1024], 1280
  for (int32_t x_outer_outer = 0; x_outer_outer < 240; ++x_outer_outer) {
    // cram-array axis
    {
      int32_t k_outer = 0;
      void* _2 = (void*) "c.global.rf[ramp((k.outer*256), 1, 256)] = x256(0)/*skip-init*/";
      for (int32_t k_inner = 0; k_inner < 64; ++k_inner) {
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
          sys->sendRequest(request);
        }
        int32_t _3 = k_outer * 64;
        int32_t _4 = _3 + k_inner;
        int32_t _5 = _4 % 128;
        bool _6 = _5 == 0;
        if (_6) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 72), 0, PrecisionT::Precision{0, 16, 0} /**/);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /*a.global[ramp(0, 1, 256)]*/);
          request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{0, 8, 0} /*b[((k.outer*64) + k.inner)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 72), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*c.global.rf[ramp((k.outer*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
    }
    void* _7 = (void*) "c.global[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t k_outer_v = 64, k_outer_v_cnt = 1; k_outer_v >= 1; k_outer_v -= max(k_outer_v / 2, 1), ++k_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /*c.global[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*c.global.rf[ramp((k.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /*c.global[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*c.global.rf[ramp((k.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(x_outer_outer, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  // freed c_global
  // freed a_global
  // freed c_global_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_mv_240_64_256__ = pimsim::registerFunc("mv_240_64_256", mv_240_64_256);

