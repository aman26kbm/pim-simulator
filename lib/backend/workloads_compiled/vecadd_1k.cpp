// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t vecadd_1k_compiled(System *sys) {
  void* _1 = nullptr;
  for (int32_t x_outer = 0; x_outer < 4; ++x_outer) {
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0/*TODO: support float*/}); /*DRAM*/;
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0/*TODO: support float*/}); /*a[ramp((x.outer*256), 1, 256)]*/;
      sys->sendRequest(request);
    }
    (void)_1;
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0/*TODO: support float*/}); /*DRAM*/;
      request.addOperand(sys->getAddress(0, 0, 8), 0, PrecisionT::Precision{0, 8, 0/*TODO: support float*/}); /*b[ramp((x.outer*256), 1, 256)]*/;
      sys->sendRequest(request);
    }
    (void)_1;
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 8, 0/*TODO: support float*/}); /**/;
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0/*TODO: support float*/}); /*a[ramp((x.outer*256), 1, 256)]*/;
      request.addOperand(sys->getAddress(0, 0, 8), 0, PrecisionT::Precision{0, 8, 0/*TODO: support float*/}); /*b[ramp((x.outer*256), 1, 256)]*/;
      sys->sendRequest(request);
    }
    (void)_1;
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0/*TODO: support float*/}); /*DRAM*/;
      request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 8, 0/*TODO: support float*/}); /*CRAM*/;
      sys->sendRequest(request);
    }
    (void)_1;
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_vecadd_1k_compiled__ = pimsim::registerFunc("vecadd_1k_compiled", vecadd_1k_compiled);

