// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t dwt_4096(System *sys) {
  void* _1 = nullptr;
  // int32 in_global[2048], 0
  for (int32_t x_outer_y_outer_fused_outer = 0; x_outer_y_outer_fused_outer < 120; ++x_outer_y_outer_fused_outer) {
    for (int32_t x_outer_y_outer_fused_inner = 0; x_outer_y_outer_fused_inner < 2; ++x_outer_y_outer_fused_inner) {
      // cram-array axis
      {
        int32_t x_inner = 0;
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(x_outer_y_outer_fused_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
          request.addOperand(sys->getAddress(x_outer_y_outer_fused_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
          sys->sendRequest(request);
        }
        int32_t _2 = x_outer_y_outer_fused_outer * 2;
        int32_t _3 = _2 + x_outer_y_outer_fused_inner;
        int32_t _4 = _3 >> 4;
        int32_t _5 = _4 * 256;
        int32_t _6 = _5 + x_inner;
        bool _7 = _6 < 2048;
        if (_7) {
          int32_t _8 = x_outer_y_outer_fused_outer * 2;
          int32_t _9 = _8 + x_outer_y_outer_fused_inner;
          bool _10 = _9 < 128;
          if (_10) {
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(x_outer_y_outer_fused_outer, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(x_outer_y_outer_fused_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*in.global[ramp(0, 2, 128)]*/);
              request.addOperand(sys->getAddress(x_outer_y_outer_fused_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*in.global[ramp(1, 2, 128)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowStore);
              request.addOperand(sys->getAddress(x_outer_y_outer_fused_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
              request.addOperand(sys->getAddress(x_outer_y_outer_fused_outer, 0, 64), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(x_outer_y_outer_fused_outer, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(x_outer_y_outer_fused_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*in.global[ramp(0, 2, 128)]*/);
              request.addOperand(sys->getAddress(x_outer_y_outer_fused_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*in.global[ramp(1, 2, 128)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowStore);
              request.addOperand(sys->getAddress(x_outer_y_outer_fused_outer, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
              request.addOperand(sys->getAddress(x_outer_y_outer_fused_outer, 0, 80), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
              sys->sendRequest(request);
            }
          }
        }
      }
    }
  }
  for (int32_t x_outer_y_outer_fused = 0; x_outer_y_outer_fused < 64; ++x_outer_y_outer_fused) {
    // cram-array axis
    {
      int32_t x_inner1 = 0;
      for (int32_t ax0 = 0; ax0 < 2; ++ax0) {
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(x_outer_y_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
          request.addOperand(sys->getAddress(x_outer_y_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x_outer_y_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(x_outer_y_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*in.global[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(x_outer_y_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*in.global[ramp(256, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(x_outer_y_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(x_outer_y_outer_fused, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x_outer_y_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(x_outer_y_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*in.global[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(x_outer_y_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*in.global[ramp(256, 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(x_outer_y_outer_fused, 0, 0), 0, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(x_outer_y_outer_fused, 0, 128), 0, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed in_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_dwt_4096__ = pimsim::registerFunc("dwt_4096", dwt_4096);

