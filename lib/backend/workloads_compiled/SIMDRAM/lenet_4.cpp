// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t lenet_4(System *sys,std::string param_file) {
  void* _1 = nullptr;
  // cram-array axis
  {
    int32_t ax3 = 0;
    void* _2 = (void*) "tensor[ramp(ax3, 16, 25)] = x25((int8)-128)/*skip-init*/";
    for (int32_t rv0 = 0; rv0 < 2; ++rv0) {
      for (int32_t rv1 = 0; rv1 < 2; ++rv1) {
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(0, 0, 3), 0, PrecisionT::Precision{0, 8, 0} /**/);
          request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*tensor[ramp(ax3, 16, 25)]*/);
          request.addOperand(sys->getAddress(0, 0, 1), 0, PrecisionT::Precision{0, 8, 0} /*a[((((((let rmod = (ramp(0, 1, 25) % x25(5)) in (let rdiv = (ramp(0, 1, 25)/x25(5)) in select(((x25((bool)1) && (x25(0) <= rmod)) || (x25((bool)0) && (rmod <= x25(0)))), rdiv, (rdiv - x25(1)))))*x25(320)) + x25((rv0*160))) + ((let rmod = (ramp(0, 1, 25) % x25(5)) in select(((x25((bool)1) && (x25(0) <= rmod)) || (x25((bool)0) && (rmod <= x25(0)))), rmod, (rmod + x25(5))))*x25(32))) + x25((rv1*16))) + x25(ax3))]*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_lenet_4__ = pimsim::registerFunc("lenet_4", lenet_4);

