// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t fir_120_512_64(System *sys) {
  void* _1 = nullptr;
  // int16_t b_global[64], 0
  uint64_t _2 = (uint64_t)8254;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int16_t c_global[128], 64
  // int16_t c_global_rf[128], 192
  for (int32_t x_outer = 0; x_outer < 120; ++x_outer) {
    for (int32_t ax0 = 0; ax0 < 32; ++ax0) {
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(x_outer, 0, 0), 1, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
        request.addOperand(sys->getAddress(x_outer, 0, 0), 1, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        sys->sendRequest(request);
      }
    }
    // cram-array axis
    {
      int32_t x_inner_outer_outer = 0;
      for (int32_t ax01 = 0; ax01 < 4127; ++ax01) {
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 16, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 16, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
          sys->sendRequest(request);
        }
      }
      for (int32_t x_inner_outer_inner = 0; x_inner_outer_inner < 64; ++x_inner_outer_inner) {
        void* _6 = (void*) "c.global[ramp(0, 1, 64)] = x64((int16)0)/*skip-init*/";
        // cram-array axis
        {
          int32_t rv = 0;
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
            request.addOperand(sys->getAddress(x_outer, 0, 10), 32768, PrecisionT::Precision{0, 16, 0} /*a.global[ramp(((x.inner.outer.inner*64) + rv), 1, 64)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowMul);
            request.addOperand(sys->getAddress(x_outer, 0, 6), 32768, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 10), 32768, PrecisionT::Precision{0, 16, 0} /*a.global[ramp(((x.inner.outer.inner*64) + rv), 1, 64)]*/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 512, PrecisionT::Precision{0, 16, 0} /*b.global[rv]*/);
            sys->sendRequest(request);
          }
        }
        #define max(a,b) ((a)>(b)?(a):(b))
        for (int32_t rv_v_inner = 32, rv_v_inner_cnt = 1; rv_v_inner >= 1; rv_v_inner -= max(rv_v_inner / 2, 1), ++rv_v_inner_cnt) {
        #undef max
          {
            Request request(Request::Type::BlockSend_Receive);
            request.addOperand(sys->getAddress(x_outer, 0, 2), 32768, PrecisionT::Precision{0, 16, 0} /*c.global[ramp(0, 1, 64)]*/);
            request.addOperand(sys->getAddress(x_outer, 0, 6), 32768, PrecisionT::Precision{0, 16, 0} /*c.global.rf[ramp((rv.v.inner*64), 1, 64)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(x_outer, 0, 2), 32768, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 2), 32768, PrecisionT::Precision{0, 16, 0} /*c.global[ramp(0, 1, 64)]*/);
            request.addOperand(sys->getAddress(x_outer, 0, 6), 32768, PrecisionT::Precision{0, 16, 0} /*c.global.rf[ramp((rv.v.inner*64), 1, 64)]*/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 1024, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(x_outer, 0, 2), 1024, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed c_global_rf
  // freed c_global
  int32_t _7 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _8 = _7 != 0;
  if (_8) {
    return -1;
      }
  // freed b_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_fir_120_512_64__ = pimsim::registerFunc("fir_120_512_64", fir_120_512_64);

