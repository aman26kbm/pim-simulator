// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t hotspot3d_512_8_100(System *sys,std::string param_file) {
  void* _1 = nullptr;
  // float32 in_global[4896], 0
  for (int32_t x_outer = 0; x_outer < 8; ++x_outer) {
    // cram-array axis
    {
      int32_t x_inner = 0;
      // cram-array axis
      {
        int32_t y_outer = 0;
        for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 5; ++ax0_ax1_fused_ax2_fused) {
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
            request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 200) % x200(100)) in (let rdiv = (ramp(0, 1, 200)/x200(100)) in select(((x200((bool)1) && (x200(0) <= rmod)) || (x200((bool)0) && (rmod <= x200(0)))), rdiv, (rdiv - x200(1)))))*x200(102)) + (let rmod = (ramp(0, 1, 200) % x200(100)) in select(((x200((bool)1) && (x200(0) <= rmod)) || (x200((bool)0) && (rmod <= x200(0)))), rmod, (rmod + x200(100))))) + x200(511))]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 200) % x200(100)) in (let rdiv = (ramp(0, 1, 200)/x200(100)) in select(((x200((bool)1) && (x200(0) <= rmod)) || (x200((bool)0) && (rmod <= x200(0)))), rdiv, (rdiv - x200(1)))))*x200(102)) + (let rmod = (ramp(0, 1, 200) % x200(100)) in select(((x200((bool)1) && (x200(0) <= rmod)) || (x200((bool)0) && (rmod <= x200(0)))), rmod, (rmod + x200(100))))) + x200(409))]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 160), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 200) % x200(100)) in (let rdiv = (ramp(0, 1, 200)/x200(100)) in select(((x200((bool)1) && (x200(0) <= rmod)) || (x200((bool)0) && (rmod <= x200(0)))), rdiv, (rdiv - x200(1)))))*x200(102)) + (let rmod = (ramp(0, 1, 200) % x200(100)) in select(((x200((bool)1) && (x200(0) <= rmod)) || (x200((bool)0) && (rmod <= x200(0)))), rmod, (rmod + x200(100))))) + x200(613))]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 200) % x200(100)) in (let rdiv = (ramp(0, 1, 200)/x200(100)) in select(((x200((bool)1) && (x200(0) <= rmod)) || (x200((bool)0) && (rmod <= x200(0)))), rdiv, (rdiv - x200(1)))))*x200(102)) + (let rmod = (ramp(0, 1, 200) % x200(100)) in select(((x200((bool)1) && (x200(0) <= rmod)) || (x200((bool)0) && (rmod <= x200(0)))), rmod, (rmod + x200(100))))) + x200(512))]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 200) % x200(100)) in (let rdiv = (ramp(0, 1, 200)/x200(100)) in select(((x200((bool)1) && (x200(0) <= rmod)) || (x200((bool)0) && (rmod <= x200(0)))), rdiv, (rdiv - x200(1)))))*x200(102)) + (let rmod = (ramp(0, 1, 200) % x200(100)) in select(((x200((bool)1) && (x200(0) <= rmod)) || (x200((bool)0) && (rmod <= x200(0)))), rmod, (rmod + x200(100))))) + x200(510))]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 200) % x200(100)) in (let rdiv = (ramp(0, 1, 200)/x200(100)) in select(((x200((bool)1) && (x200(0) <= rmod)) || (x200((bool)0) && (rmod <= x200(0)))), rdiv, (rdiv - x200(1)))))*x200(102)) + (let rmod = (ramp(0, 1, 200) % x200(100)) in select(((x200((bool)1) && (x200(0) <= rmod)) || (x200((bool)0) && (rmod <= x200(0)))), rmod, (rmod + x200(100))))) + x200(919))]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 200) % x200(100)) in (let rdiv = (ramp(0, 1, 200)/x200(100)) in select(((x200((bool)1) && (x200(0) <= rmod)) || (x200((bool)0) && (rmod <= x200(0)))), rdiv, (rdiv - x200(1)))))*x200(102)) + (let rmod = (ramp(0, 1, 200) % x200(100)) in select(((x200((bool)1) && (x200(0) <= rmod)) || (x200((bool)0) && (rmod <= x200(0)))), rmod, (rmod + x200(100))))) + x200(103))]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*in.global[((((let rmod = (ramp(0, 1, 200) % x200(100)) in (let rdiv = (ramp(0, 1, 200)/x200(100)) in select(((x200((bool)1) && (x200(0) <= rmod)) || (x200((bool)0) && (rmod <= x200(0)))), rdiv, (rdiv - x200(1)))))*x200(102)) + (let rmod = (ramp(0, 1, 200) % x200(100)) in select(((x200((bool)1) && (x200(0) <= rmod)) || (x200((bool)0) && (rmod <= x200(0)))), rmod, (rmod + x200(100))))) + x200(511))]*/);
          request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer, 0, 197), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, 185), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
          request.addOperand(sys->getAddress(x_outer, 0, 197), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed in_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_hotspot3d_512_8_100__ = pimsim::registerFunc("hotspot3d_512_8_100", hotspot3d_512_8_100);

