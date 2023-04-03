// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t dwt_1024(System *sys) {
  void* _1 = nullptr;
  // float32 in_global[1024], 0
  for (int32_t x_outer = 0; x_outer < 14; ++x_outer) {
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(0, 1, 256)]*/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(0, 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 48), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(0, 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
      request.addOperand(sys->getAddress(0, 0, 48), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  // freed in_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_dwt_1024__ = pimsim::registerFunc("dwt_1024", dwt_1024);

