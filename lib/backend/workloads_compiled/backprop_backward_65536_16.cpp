// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t backprop_backward_65536_16(System *sys) {
  void* _1 = nullptr;
  // float32 compute[1024], 0
  for (int32_t x = 0; x < 16; ++x) {
    // cram-array axis
    {
      int32_t y_outer = 0;
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(x, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[x]*/);
        request.addOperand(x * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(x, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
        request.addOperand(sys->getAddress(x, 0, 32), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[ramp((y.outer*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(x, 0, 64), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x, 0, 32), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[ramp((y.outer*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(x, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
        request.addOperand(sys->getAddress(x, 0, 64), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[ramp(((x*65536) + (y.outer*256)), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(x, 0, 80), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x, 0, 64), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[ramp(((x*65536) + (y.outer*256)), 1, 256)]*/);
        request.addOperand(x * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x, 0, 64), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x, 0, 80), 0, PrecisionT::Precision{1, 23, 8} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(x, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
        request.addOperand(sys->getAddress(x, 0, 32), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[ramp(((x*65536) + (y.outer*256)), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x, 0, 48), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x, 0, 32), 0, PrecisionT::Precision{1, 23, 8} /*placeholder[ramp(((x*65536) + (y.outer*256)), 1, 256)]*/);
        request.addOperand(sys->getAddress(x, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*compute[ramp(0, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(x, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
        request.addOperand(sys->getAddress(x, 0, 48), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed compute
  return 0;
}

static __attribute__((unused)) Registry::Entry &_backprop_backward_65536_16__ = pimsim::registerFunc("backprop_backward_65536_16", backprop_backward_65536_16);

