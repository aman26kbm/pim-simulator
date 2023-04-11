// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t vgg_mv_256_256(System *sys) {
  void* _1 = nullptr;
  // int1 b_local[0], 0
  uint64_t _2 = (uint64_t)65536;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 8);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  {
    Request request(Request::Type::RowLoad);
    request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*CRAM*/);
    request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*DRAM*/);
    sys->sendRequest(request);
  }
  // cram-array axis
  {
    int32_t k = 0;
    {
      Request request(Request::Type::RowMul);
      request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 2, 0} /**/);
      request.addOperand(sys->getAddress(0, 0, 8), 0, PrecisionT::Precision{0, 1, 0} /*a[ramp((k*256), 1, 256)]*/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*b.local[k]*/);
      sys->sendRequest(request);
    }
  }
  void* _6 = (void*) "c[ramp(0, 1, 256)] = x256((int8)0)/*skip-init*/";
  #define max(a,b) ((a)>(b)?(a):(b))
  for (int32_t k_v = 256, k_v_cnt = 1; k_v >= 1; k_v -= max(k_v / 2, 1), ++k_v_cnt) {
  #undef max
    {
      Request request(Request::Type::BlockSend_Receive);
      request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /*c[ramp(0, 1, 256)]*/);
      request.addOperand(sys->getAddress(0, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /*c.rf[ramp((k.v*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 48), 0, PrecisionT::Precision{0, 8, 0} /**/);
      request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /*c[ramp(0, 1, 256)]*/);
      request.addOperand(sys->getAddress(0, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /*c.rf[ramp((k.v*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
  }
  int32_t _7 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _8 = _7 != 0;
  if (_8) {
    return -1;
      }
  // freed b_local
  return 0;
}

static __attribute__((unused)) Registry::Entry &_vgg_mv_256_256__ = pimsim::registerFunc("vgg_mv_256_256", vgg_mv_256_256);

