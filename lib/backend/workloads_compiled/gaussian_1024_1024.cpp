// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t gaussian_1024_1024(System *sys) {
  void* _1 = nullptr;
  // float32 in_global[3096], 0
  for (int32_t x_outer = 0; x_outer < 16; ++x_outer) {
    // cram-array axis
    {
      int32_t x_inner = 0;
      // cram-array axis
      {
        int32_t y_outer = 0;
        for (int32_t ax0_ax1_fused = 0; ax0_ax1_fused < 4; ++ax0_ax1_fused) {
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 104), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(0, 1, 256)]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(1, 1, 256)]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 104), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(2, 1, 256)]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(258, 1, 256)]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(259, 1, 256)]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(260, 1, 256)]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(516, 1, 256)]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(517, 1, 256)]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(518, 1, 256)]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 168), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
          request.addOperand(sys->getAddress(x_outer, 0, 168), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed in_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_gaussian_1024_1024__ = pimsim::registerFunc("gaussian_1024_1024", gaussian_1024_1024);

