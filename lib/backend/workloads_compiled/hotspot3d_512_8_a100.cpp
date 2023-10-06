// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t hotspot3d_512_8_a100(System *sys) {
  void* _1 = nullptr;
  // float32 in_global[370080], 0
  for (int32_t x_outer_outer = 0; x_outer_outer < 32; ++x_outer_outer) {
    // for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < (x_outer_outer == 0 ? 2 : 1); ++ax0_ax1_fused_ax2_fused) {
    //   {
    //     Request request(Request::Type::RowLoad);
    //     request.addOperand(sys->getAddress(x_outer_outer, 0, 0), (x_outer_outer != 0) * 1024, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
    //     request.addOperand(sys->getAddress(x_outer_outer, 0, 0), (x_outer_outer != 0) * 1024, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
    //     sys->sendRequest(request);
    //   }
    // }
    // cram-array axis
    {
      int32_t x_inner_y_outer_fused = 0;
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11568), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[(((x256(((tir.shift_right(x.inner.y.outer.fused, 4)*5140) + (tir.bitwise_and(x.inner.y.outer.fused, 15)*320))) + ((let rmod = (ramp(0, 1, 256) % x256(8)) in (let rdiv = (ramp(0, 1, 256)/x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(10))) + (let rmod = (ramp(0, 1, 256) % x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(8))))) + x256(5151))]*/);
        request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[(((x256(((tir.shift_right(x.inner.y.outer.fused, 4)*5140) + (tir.bitwise_and(x.inner.y.outer.fused, 15)*320))) + ((let rmod = (ramp(0, 1, 256) % x256(8)) in (let rdiv = (ramp(0, 1, 256)/x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(10))) + (let rmod = (ramp(0, 1, 256) % x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(8))))) + x256(5141))]*/);
        request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11568), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[(((x256(((tir.shift_right(x.inner.y.outer.fused, 4)*5140) + (tir.bitwise_and(x.inner.y.outer.fused, 15)*320))) + ((let rmod = (ramp(0, 1, 256) % x256(8)) in (let rdiv = (ramp(0, 1, 256)/x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(10))) + (let rmod = (ramp(0, 1, 256) % x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(8))))) + x256(5161))]*/);
        request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[(((x256(((tir.shift_right(x.inner.y.outer.fused, 4)*5140) + (tir.bitwise_and(x.inner.y.outer.fused, 15)*320))) + ((let rmod = (ramp(0, 1, 256) % x256(8)) in (let rdiv = (ramp(0, 1, 256)/x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(10))) + (let rmod = (ramp(0, 1, 256) % x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(8))))) + x256(5152))]*/);
        request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[(((x256(((tir.shift_right(x.inner.y.outer.fused, 4)*5140) + (tir.bitwise_and(x.inner.y.outer.fused, 15)*320))) + ((let rmod = (ramp(0, 1, 256) % x256(8)) in (let rdiv = (ramp(0, 1, 256)/x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(10))) + (let rmod = (ramp(0, 1, 256) % x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(8))))) + x256(5150))]*/);
        request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[(((x256(((tir.shift_right(x.inner.y.outer.fused, 4)*5140) + (tir.bitwise_and(x.inner.y.outer.fused, 15)*320))) + ((let rmod = (ramp(0, 1, 256) % x256(8)) in (let rdiv = (ramp(0, 1, 256)/x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(10))) + (let rmod = (ramp(0, 1, 256) % x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(8))))) + x256(10291))]*/);
        request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[(((x256(((tir.shift_right(x.inner.y.outer.fused, 4)*5140) + (tir.bitwise_and(x.inner.y.outer.fused, 15)*320))) + ((let rmod = (ramp(0, 1, 256) % x256(8)) in (let rdiv = (ramp(0, 1, 256)/x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(10))) + (let rmod = (ramp(0, 1, 256) % x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(8))))) + x256(11))]*/);
        request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[(((x256(((tir.shift_right(x.inner.y.outer.fused, 4)*5140) + (tir.bitwise_and(x.inner.y.outer.fused, 15)*320))) + ((let rmod = (ramp(0, 1, 256) % x256(8)) in (let rdiv = (ramp(0, 1, 256)/x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rdiv, (rdiv - x256(1)))))*x256(10))) + (let rmod = (ramp(0, 1, 256) % x256(8)) in select(((x256((bool)1) && (x256(0) <= rmod)) || (x256((bool)0) && (rmod <= x256(0)))), rmod, (rmod + x256(8))))) + x256(5151))]*/);
        request.addOperand(x_outer_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd_CRAM_RF);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11616), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11600), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 11616), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed in_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_hotspot3d_512_8__ = pimsim::registerFunc("hotspot3d_512_8_a100", hotspot3d_512_8_a100);

