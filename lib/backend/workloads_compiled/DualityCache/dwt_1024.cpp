// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t dwt_1024(System *sys,std::string param_file) {
  void* _1 = nullptr;
  // int32 in_global[2048], 0
  for (int32_t x_outer = 0; x_outer < 2; ++x_outer) {
    for (int32_t y_outer = 0; y_outer < 4; ++y_outer) {
      // cram-array axis
      {
        int32_t x_inner = 0;
        {
          Request request(Request::Type::RowLoad);
          int32_t _2 = x_outer * 4;
          int32_t _3 = _2 + y_outer;
          request.addOperand(sys->getAddress(_3, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
          request.addOperand(sys->getAddress(_3, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          int32_t _4 = x_outer * 4;
          int32_t _5 = _4 + y_outer;
          request.addOperand(sys->getAddress(_5, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(_5, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*in.global[ramp(0, 2, 128)]*/);
          request.addOperand(sys->getAddress(_5, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*in.global[ramp(1, 2, 128)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowStore);
          int32_t _6 = x_outer * 4;
          int32_t _7 = _6 + y_outer;
          request.addOperand(sys->getAddress(_7, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(_7, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          int32_t _8 = x_outer * 4;
          int32_t _9 = _8 + y_outer;
          request.addOperand(sys->getAddress(_9, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(_9, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*in.global[ramp(0, 2, 128)]*/);
          request.addOperand(sys->getAddress(_9, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*in.global[ramp(1, 2, 128)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowStore);
          int32_t _10 = x_outer * 4;
          int32_t _11 = _10 + y_outer;
          request.addOperand(sys->getAddress(_11, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(_11, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer1 = 0; x_outer1 < 2; ++x_outer1) {
    for (int32_t y_outer1 = 0; y_outer1 < 2; ++y_outer1) {
      // cram-array axis
      {
        int32_t x_inner1 = 0;
        for (int32_t ax0 = 0; ax0 < 2; ++ax0) {
          {
            Request request(Request::Type::RowLoad);
            int32_t _12 = x_outer1 * 2;
            int32_t _13 = _12 + y_outer1;
            request.addOperand(sys->getAddress(_13, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
            request.addOperand(sys->getAddress(_13, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowAdd);
          int32_t _14 = x_outer1 * 2;
          int32_t _15 = _14 + y_outer1;
          request.addOperand(sys->getAddress(_15, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(_15, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*in.global[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(_15, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*in.global[ramp(256, 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowStore);
          int32_t _16 = x_outer1 * 2;
          int32_t _17 = _16 + y_outer1;
          request.addOperand(sys->getAddress(_17, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(_17, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          int32_t _18 = x_outer1 * 2;
          int32_t _19 = _18 + y_outer1;
          request.addOperand(sys->getAddress(_19, 0, 128), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(_19, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*in.global[ramp(0, 1, 256)]*/);
          request.addOperand(sys->getAddress(_19, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*in.global[ramp(256, 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowStore);
          int32_t _20 = x_outer1 * 2;
          int32_t _21 = _20 + y_outer1;
          request.addOperand(sys->getAddress(_21, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(_21, 0, 128), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed in_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_dwt_1024__ = pimsim::registerFunc("dwt_1024", dwt_1024);

