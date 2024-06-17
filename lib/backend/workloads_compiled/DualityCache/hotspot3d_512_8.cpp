// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t hotspot3d_512_8(System *sys,std::string param_file) {
  void* _1 = nullptr;
  // float32 in_global[480], 0
  for (int32_t x_outer = 0; x_outer < 45; ++x_outer) {
    for (int32_t x_inner = 0; x_inner < 12; ++x_inner) {
      // cram-array axis
      {
        int32_t y_outer = 0;
        for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 1; ++ax0_ax1_fused_ax2_fused) {
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _2 = x_outer * 12;
        int32_t _3 = _2 + x_inner;
        bool _4 = _3 < 512;
        if (_4) {
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(x_outer, 0, 16), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 16) % x16(8)) in (let rdiv = (ramp(0, 1, 16)/x16(8)) in select(((x16((bool)1) && (x16(0) <= rmod)) || (x16((bool)0) && (rmod <= x16(0)))), rdiv, (rdiv - x16(1)))))*x16(10)) + (let rmod = (ramp(0, 1, 16) % x16(8)) in select(((x16((bool)1) && (x16(0) <= rmod)) || (x16((bool)0) && (rmod <= x16(0)))), rmod, (rmod + x16(8))))) + x16(51))]*/);
            request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 16) % x16(8)) in (let rdiv = (ramp(0, 1, 16)/x16(8)) in select(((x16((bool)1) && (x16(0) <= rmod)) || (x16((bool)0) && (rmod <= x16(0)))), rdiv, (rdiv - x16(1)))))*x16(10)) + (let rmod = (ramp(0, 1, 16) % x16(8)) in select(((x16((bool)1) && (x16(0) <= rmod)) || (x16((bool)0) && (rmod <= x16(0)))), rmod, (rmod + x16(8))))) + x16(41))]*/);
            request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 16), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 16) % x16(8)) in (let rdiv = (ramp(0, 1, 16)/x16(8)) in select(((x16((bool)1) && (x16(0) <= rmod)) || (x16((bool)0) && (rmod <= x16(0)))), rdiv, (rdiv - x16(1)))))*x16(10)) + (let rmod = (ramp(0, 1, 16) % x16(8)) in select(((x16((bool)1) && (x16(0) <= rmod)) || (x16((bool)0) && (rmod <= x16(0)))), rmod, (rmod + x16(8))))) + x16(61))]*/);
            request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 16) % x16(8)) in (let rdiv = (ramp(0, 1, 16)/x16(8)) in select(((x16((bool)1) && (x16(0) <= rmod)) || (x16((bool)0) && (rmod <= x16(0)))), rdiv, (rdiv - x16(1)))))*x16(10)) + (let rmod = (ramp(0, 1, 16) % x16(8)) in select(((x16((bool)1) && (x16(0) <= rmod)) || (x16((bool)0) && (rmod <= x16(0)))), rmod, (rmod + x16(8))))) + x16(52))]*/);
            request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 16) % x16(8)) in (let rdiv = (ramp(0, 1, 16)/x16(8)) in select(((x16((bool)1) && (x16(0) <= rmod)) || (x16((bool)0) && (rmod <= x16(0)))), rdiv, (rdiv - x16(1)))))*x16(10)) + (let rmod = (ramp(0, 1, 16) % x16(8)) in select(((x16((bool)1) && (x16(0) <= rmod)) || (x16((bool)0) && (rmod <= x16(0)))), rmod, (rmod + x16(8))))) + x16(50))]*/);
            request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 16) % x16(8)) in (let rdiv = (ramp(0, 1, 16)/x16(8)) in select(((x16((bool)1) && (x16(0) <= rmod)) || (x16((bool)0) && (rmod <= x16(0)))), rdiv, (rdiv - x16(1)))))*x16(10)) + (let rmod = (ramp(0, 1, 16) % x16(8)) in select(((x16((bool)1) && (x16(0) <= rmod)) || (x16((bool)0) && (rmod <= x16(0)))), rmod, (rmod + x16(8))))) + x16(91))]*/);
            request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 16) % x16(8)) in (let rdiv = (ramp(0, 1, 16)/x16(8)) in select(((x16((bool)1) && (x16(0) <= rmod)) || (x16((bool)0) && (rmod <= x16(0)))), rdiv, (rdiv - x16(1)))))*x16(10)) + (let rmod = (ramp(0, 1, 16) % x16(8)) in select(((x16((bool)1) && (x16(0) <= rmod)) || (x16((bool)0) && (rmod <= x16(0)))), rmod, (rmod + x16(8))))) + x16(11))]*/);
            request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 16) % x16(8)) in (let rdiv = (ramp(0, 1, 16)/x16(8)) in select(((x16((bool)1) && (x16(0) <= rmod)) || (x16((bool)0) && (rmod <= x16(0)))), rdiv, (rdiv - x16(1)))))*x16(10)) + (let rmod = (ramp(0, 1, 16) % x16(8)) in select(((x16((bool)1) && (x16(0) <= rmod)) || (x16((bool)0) && (rmod <= x16(0)))), rmod, (rmod + x16(8))))) + x16(51))]*/);
            request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd_CRAM_RF);
            request.addOperand(sys->getAddress(x_outer, 0, 19), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, 18), 0, PrecisionT::Precision{1, 23, 8} /**/);
            request.addOperand(sys->getAddress(x_outer, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowStore);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
            request.addOperand(sys->getAddress(x_outer, 0, 19), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
            sys->sendRequest(request);
          }
        }
      }
    }
  }
  // freed in_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_hotspot3d_512_8__ = pimsim::registerFunc("hotspot3d_512_8", hotspot3d_512_8);

