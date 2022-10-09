// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t mv_compiled(System *sys) {
  void* _1 = nullptr;
  // int32_t c_global[1024], 0
  // int8_t a_global[256], 1024
  // cram-array axis
  {
    int32_t x_outer = 0;
    void* _2 = (void*) "c.global[ramp(0, 1, 256)] = x256(0)/*skip-init*/";
    (void)_2;
    for (int32_t k_outer = 0; k_outer < 32; ++k_outer) {
      for (int32_t k_inner = 0; k_inner < 128; ++k_inner) {
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(0, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
          request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
          sys->sendRequest(request);
        }
        (void)_1;
        int32_t _3 = k_outer * 128;
        int32_t _4 = _3 + k_inner;
        int32_t _5 = _4 % 128;
        bool _6 = _5 == 0;
        if (_6) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
          (void)_1;
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(0, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(0, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /*a.global[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*b[((k.outer*128) + k.inner)]*/);
          sys->sendRequest(request);
        }
        (void)_1;
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(0, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*c.global[ramp(0, 1, 256)]*/);
          sys->sendRequest(request);
        }
        (void)_1;
      }
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
      sys->sendRequest(request);
    }
    (void)_1;
  }
  // freed a_global
  // freed c_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_mv_compiled__ = pimsim::registerFunc("mv_compiled", mv_compiled);

