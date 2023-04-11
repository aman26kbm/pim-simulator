// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t vgg_bnn_13_2(System *sys) {
  void* _1 = nullptr;
  for (int32_t ax3 = 0; ax3 < 6; ++ax3) {
    void* _2 = (void*) "tensor[ramp(ax3, 6, 196)] = x196(-3.40282e+38f)/*skip-init*/";
    for (int32_t rv0 = 0; rv0 < 2; ++rv0) {
      for (int32_t rv1 = 0; rv1 < 2; ++rv1) {
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(0, 0, 24), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*tensor[ramp(ax3, 6, 196)]*/);
          request.addOperand(sys->getAddress(0, 0, 12), 0, PrecisionT::Precision{1, 23, 8} /*a[((((((let rmod = (ramp(0, 1, 196) % x196(14)) in (let rdiv = (ramp(0, 1, 196)/x196(14)) in select(((x196((bool)1) && (x196(0) <= rmod)) || (x196((bool)0) && (rmod <= x196(0)))), rdiv, (rdiv - x196(1)))))*x196(336)) + x196((rv0*168))) + ((let rmod = (ramp(0, 1, 196) % x196(14)) in select(((x196((bool)1) && (x196(0) <= rmod)) || (x196((bool)0) && (rmod <= x196(0)))), rmod, (rmod + x196(14))))*x196(12))) + x196((rv1*6))) + x196(ax3))]*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_vgg_bnn_13_2__ = pimsim::registerFunc("vgg_bnn_13_2", vgg_bnn_13_2);

