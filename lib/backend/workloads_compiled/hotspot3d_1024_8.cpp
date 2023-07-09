// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t hotspot3d_1024_8(System *sys) {
  void* _1 = nullptr;
  // float32 in_global[4080], 0
  for (int32_t x_outer_outer = 0; x_outer_outer < 120; ++x_outer_outer) {
    for (int32_t x_outer_inner = 0; x_outer_inner < 2; ++x_outer_inner) {
      // cram-array axis
      {
        int32_t x_inner_y_outer_fused = 0;
        for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 4; ++ax0_ax1_fused_ax2_fused) {
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _2 = x_outer_outer * 16;
        int32_t _3 = x_outer_inner * 8;
        int32_t _4 = _2 + _3;
        int32_t _5 = x_inner_y_outer_fused >> 5;
        int32_t _6 = _4 + _5;
        bool _7 = _6 < 1024;
        if (_7) {
          int32_t _8 = x_outer_outer * 2;
          int32_t _9 = _8 + x_outer_inner;
          bool _10 = _9 < 128;
          if (_10) {
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 128), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 256) % x256(8)) in (let rdiv = (ramp(0, 1, 256)/x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(10)) + (let rmod = (ramp(0, 1, 256) % x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(8))))) + x256(351))]*/);
              request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 256) % x256(8)) in (let rdiv = (ramp(0, 1, 256)/x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(10)) + (let rmod = (ramp(0, 1, 256) % x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(8))))) + x256(341))]*/);
              request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 128), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 256) % x256(8)) in (let rdiv = (ramp(0, 1, 256)/x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(10)) + (let rmod = (ramp(0, 1, 256) % x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(8))))) + x256(361))]*/);
              request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 256) % x256(8)) in (let rdiv = (ramp(0, 1, 256)/x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(10)) + (let rmod = (ramp(0, 1, 256) % x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(8))))) + x256(352))]*/);
              request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 256) % x256(8)) in (let rdiv = (ramp(0, 1, 256)/x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(10)) + (let rmod = (ramp(0, 1, 256) % x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(8))))) + x256(350))]*/);
              request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 256) % x256(8)) in (let rdiv = (ramp(0, 1, 256)/x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(10)) + (let rmod = (ramp(0, 1, 256) % x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(8))))) + x256(691))]*/);
              request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 256) % x256(8)) in (let rdiv = (ramp(0, 1, 256)/x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(10)) + (let rmod = (ramp(0, 1, 256) % x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(8))))) + x256(11))]*/);
              request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul_CRAM_RF);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 256) % x256(8)) in (let rdiv = (ramp(0, 1, 256)/x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(10)) + (let rmod = (ramp(0, 1, 256) % x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(8))))) + x256(351))]*/);
              request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd_CRAM_RF);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 176), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowStore);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
              request.addOperand(sys->getAddress(x_outer_outer, 0, 176), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
              sys->sendRequest(request);
            }
          }
        }
      }
    }
  }
  // freed in_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_hotspot3d_1024_8__ = pimsim::registerFunc("hotspot3d_1024_8", hotspot3d_1024_8);

