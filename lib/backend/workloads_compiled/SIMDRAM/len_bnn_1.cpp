// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t len_bnn_1(System *sys,std::string param_file) {
  void* _1 = nullptr;
  // int8 placeholder_global[1024], 0
  // int8 placeholder_d_global[150], 1024
  // int32 Conv2dOutput_rf[240], 1280
  // cram-array axis
  {
    int32_t ax1 = 0;
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
      sys->sendRequest(request);
    }
  }
  {
    Request request(Request::Type::RowLoad);
    request.addOperand(sys->getAddress(0, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
    request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
    sys->sendRequest(request);
  }
  // cram-array axis
  {
    int32_t ax11 = 0;
    // cram-array axis
    {
      int32_t ax2_outer = 0;
      // cram-array axis
      {
        int32_t ry = 0;
        void* _2 = (void*) "Conv2dOutput.rf[ramp((ry*12), 1, 12)] = x12(0)/*skip-init*/";
        for (int32_t rx = 0; rx < 5; ++rx) {
          {
            Request request(Request::Type::RowMul);
            request.addOperand(sys->getAddress(0, 0, 48), 0, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*placeholder.global[((x12((((ax1*32) + (ry*32)) + (ax2.outer*2))) + (let rmod = (ramp(0, 1, 12) % x12(6)) in (let rdiv = (ramp(0, 1, 12)/x12(6)) in select(((x12((bool)1) && (x12(0) <= rmod)) || (x12((bool)0) && (rmod <= x12(0)))), rdiv, (rdiv - x12(1)))))) + x12(rx))]*/);
            request.addOperand(sys->getAddress(0, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /*placeholder.d.global[(x12(((ry*30) + (rx*6))) + (let rmod = (ramp(0, 1, 12) % x12(6)) in select(((x12((bool)1) && (x12(0) <= rmod)) || (x12((bool)0) && (rmod <= x12(0)))), rmod, (rmod + x12(6)))))]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(0, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /**/);
            request.addOperand(sys->getAddress(0, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((ry*12), 1, 12)]*/);
            request.addOperand(sys->getAddress(0, 0, 48), 0, PrecisionT::Precision{0, 32, 0} /**/);
            sys->sendRequest(request);
          }
        }
      }
      void* _3 = (void*) "Conv2dOutput[ramp(((ax1*168) + (ax2.outer*12)), 1, 12)] = x12(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t ry_v = 5, ry_v_cnt = 1; ry_v >= 1; ry_v -= max(ry_v / 2, 1), ++ry_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(0, 0, 48), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(((ax1*168) + (ax2.outer*12)), 1, 12)]*/);
          request.addOperand(sys->getAddress(0, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((ry.v*12), 1, 12)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(0, 0, 48), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(0, 0, 48), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(((ax1*168) + (ax2.outer*12)), 1, 12)]*/);
          request.addOperand(sys->getAddress(0, 0, 40), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((ry.v*12), 1, 12)]*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed Conv2dOutput_rf
  // freed placeholder_d_global
  // freed placeholder_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_len_bnn_1__ = pimsim::registerFunc("len_bnn_1", len_bnn_1);

