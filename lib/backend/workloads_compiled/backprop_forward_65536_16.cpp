// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t backprop_forward_65536_16(System *sys) {
  void* _1 = nullptr;
  // float32 a_global[262144], 0
  // float32 compute_global[4], 262144
  for (int32_t x = 0; x < 16; ++x) {
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(x, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
      request.addOperand(sys->getAddress(x, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
      sys->sendRequest(request);
    }
    // cram-array axis
    {
      int32_t rv_outer = 0;
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(x, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
        request.addOperand(sys->getAddress(x, 0, 8200), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[ramp(((x*65536) + (rv.outer*256)), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(x, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*a.global[ramp((rv.outer*256), 1, 256)]*/);
        request.addOperand(sys->getAddress(x, 0, 8200), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[ramp(((x*65536) + (rv.outer*256)), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
    void* _2 = (void*) "compute.global[0] = 0f/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t rv_v = 65536, rv_v_cnt = 1; rv_v >= 1; rv_v -= max(rv_v / 2, 1), ++rv_v_cnt) {
    #undef max
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*compute.global[0]*/);
        request.addOperand(sys->getAddress(x, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*a.global[rv.v]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*compute.global[0]*/);
        request.addOperand(sys->getAddress(x, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*a.global[rv.v]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(x, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
      request.addOperand(sys->getAddress(x, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  // freed compute_global
  // freed a_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_backprop_forward_65536_16__ = pimsim::registerFunc("backprop_forward_65536_16", backprop_forward_65536_16);

