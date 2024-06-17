// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t vgg_mv_4096_4096(System *sys,std::string param_file) {
  void* _1 = nullptr;
  // int1 b_local[0], 0
  // int8 c_rf[65536], 4096
  {
    Request request(Request::Type::RowLoad);
    request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*CRAM*/);
    request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*DRAM*/);
    sys->sendRequest(request);
  }
  // cram-array axis
  {
    int32_t ax0_outer = 0;
    // cram-array axis
    {
      int32_t k_outer = 0;
      void* _2 = (void*) "c.rf[ramp((k.outer*256), 1, 256)] = x256((int8)0)/*skip-init*/";
      for (int32_t k_inner = 0; k_inner < 16; ++k_inner) {
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*DRAM*/);
          request.addOperand(sys->getAddress(0, 0, 2176), 0, PrecisionT::Precision{0, 1, 0} /*a[ramp((((ax0.outer*1048576) + (k.outer*4096)) + (k.inner*256)), 1, 256)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul);
          request.addOperand(sys->getAddress(0, 0, 2184), 0, PrecisionT::Precision{0, 2, 0} /**/);
          request.addOperand(sys->getAddress(0, 0, 2176), 0, PrecisionT::Precision{0, 1, 0} /*a[ramp((((ax0.outer*1048576) + (k.outer*4096)) + (k.inner*256)), 1, 256)]*/);
          request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*b.local[((k.outer*16) + k.inner)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(0, 0, 128), 0, PrecisionT::Precision{0, 8, 0} /**/);
          request.addOperand(sys->getAddress(0, 0, 2184), 0, PrecisionT::Precision{0, 8, 0} /**/);
          request.addOperand(sys->getAddress(0, 0, 128), 0, PrecisionT::Precision{0, 8, 0} /*c.rf[ramp((k.outer*256), 1, 256)]*/);
          sys->sendRequest(request);
        }
      }
    }
    void* _3 = (void*) "c[ramp((ax0.outer*256), 1, 256)] = x256((int8)0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t k_outer_v = 256, k_outer_v_cnt = 1; k_outer_v >= 1; k_outer_v -= max(k_outer_v / 2, 1), ++k_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(0, 0, 2176), 0, PrecisionT::Precision{0, 8, 0} /*c[ramp((ax0.outer*256), 1, 256)]*/);
        request.addOperand(sys->getAddress(0, 0, 128), 0, PrecisionT::Precision{0, 8, 0} /*c.rf[ramp((k.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(0, 0, 2192), 0, PrecisionT::Precision{0, 8, 0} /**/);
        request.addOperand(sys->getAddress(0, 0, 2176), 0, PrecisionT::Precision{0, 8, 0} /*c[ramp((ax0.outer*256), 1, 256)]*/);
        request.addOperand(sys->getAddress(0, 0, 128), 0, PrecisionT::Precision{0, 8, 0} /*c.rf[ramp((k.outer.v*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed c_rf
  // freed b_local
  return 0;
}

static __attribute__((unused)) Registry::Entry &_vgg_mv_4096_4096__ = pimsim::registerFunc("vgg_mv_4096_4096", vgg_mv_4096_4096);

