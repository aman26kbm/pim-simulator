// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t hotspot_4096_4096(System *sys) {
  void* _1 = nullptr;
  // float32 in_global[3096], 0
  for (int32_t x_outer_outer = 0; x_outer_outer < 3; ++x_outer_outer) {
    for (int32_t x_outer_inner = 0; x_outer_inner < 120; ++x_outer_inner) {
      // cram-array axis
      {
        int32_t x_inner = 0;
        // cram-array axis
        {
          int32_t y_outer = 0;
          for (int32_t ax0_ax1_fused = 0; ax0_ax1_fused < 4; ++ax0_ax1_fused) {
            {
              Request request(Request::Type::RowLoad);
              request.addOperand(sys->getAddress(x_outer_inner, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
              request.addOperand(sys->getAddress(x_outer_inner, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _2 = x_outer_outer * 1920;
          int32_t _3 = x_outer_inner * 16;
          int32_t _4 = _2 + _3;
          int32_t _5 = _4 + x_inner;
          bool _6 = _5 < 4096;
          if (_6) {
            int32_t _7 = x_outer_outer * 120;
            int32_t _8 = _7 + x_outer_inner;
            bool _9 = _8 < 256;
            if (_9) {
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 104), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(1, 1, 256)]*/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(517, 1, 256)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul_CRAM_RF);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(259, 1, 256)]*/);
                request.addOperand(x_outer_inner * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 104), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul_CRAM_RF);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 152), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(x_outer_inner * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowLoad);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 104), 0, PrecisionT::Precision{1, 23, 8} /*power[ramp(((((x.outer.outer*7864320) + (x.outer.inner*65536)) + (x.inner*4096)) + (y.outer*256)), 1, 256)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 152), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 104), 0, PrecisionT::Precision{1, 23, 8} /*power[ramp(((((x.outer.outer*7864320) + (x.outer.inner*65536)) + (x.inner*4096)) + (y.outer*256)), 1, 256)]*/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 152), 0, PrecisionT::Precision{1, 23, 8} /**/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 104), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(258, 1, 256)]*/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(260, 1, 256)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul_CRAM_RF);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(259, 1, 256)]*/);
                request.addOperand(x_outer_inner * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 104), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul_CRAM_RF);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 152), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 136), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(x_outer_inner * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 152), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 152), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 152), 0, PrecisionT::Precision{1, 23, 8} /**/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd_CRAM_RF);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 120), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[ramp(259, 1, 256)]*/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul_CRAM_RF);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 120), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 120), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(x_outer_inner * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 152), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 152), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 120), 0, PrecisionT::Precision{1, 23, 8} /**/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul_CRAM_RF);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 168), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 152), 0, PrecisionT::Precision{1, 23, 8} /**/);
                request.addOperand(x_outer_inner * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowStore);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
                request.addOperand(sys->getAddress(x_outer_inner, 0, 168), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
                sys->sendRequest(request);
              }
            }
          }
        }
      }
    }
  }
  // freed in_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_hotspot_4096_4096__ = pimsim::registerFunc("hotspot_4096_4096", hotspot_4096_4096);

