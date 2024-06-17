// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t len_bnn_3(System *sys,std::string param_file) {
  void* _1 = nullptr;
  // int1 b_global[0], 0
  // int8 Conv2dOutput_rf[160], 512
  // cram-array axis
  {
    int32_t ax0 = 0;
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*CRAM*/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*DRAM*/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t ax1 = 0;
    // cram-array axis
    {
      int32_t ax2_outer = 0;
      // cram-array axis
      {
        int32_t ry = 0;
        void* _2 = (void*) "Conv2dOutput.rf[ramp((ry*32), 1, 32)] = x32((int8)0)/*skip-init*/";
        for (int32_t rx = 0; rx < 5; ++rx) {
          for (int32_t rc = 0; rc < 6; ++rc) {
            {
              Request request(Request::Type::RowMul);
              request.addOperand(sys->getAddress(0, 0, 25), 0, PrecisionT::Precision{0, 2, 0} /**/);
              request.addOperand(sys->getAddress(0, 0, 24), 0, PrecisionT::Precision{0, 1, 0} /*a[(((x32((((ax1*84) + (ry*84)) + (ax2.outer*12))) + ((let rmod = (ramp(0, 1, 32) % x32(16)) in (let rdiv = (ramp(0, 1, 32)/x32(16)) in select(((x32((bool)1) && (x32(0) <= rmod)) || (x32((bool)0) && (rmod <= x32(0)))), rdiv, (rdiv - x32(1)))))*x32(6))) + x32((rx*6))) + x32(rc))]*/);
              request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*b.global[(x32((((ry*80) + (rx*16)) + (rc*16))) + (let rmod = (ramp(0, 1, 32) % x32(16)) in select(((x32((bool)1) && (x32(0) <= rmod)) || (x32((bool)0) && (rmod <= x32(0)))), rmod, (rmod + x32(16)))))]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /**/);
              request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput.rf[ramp((ry*32), 1, 32)]*/);
              request.addOperand(sys->getAddress(0, 0, 25), 0, PrecisionT::Precision{0, 8, 0} /**/);
              sys->sendRequest(request);
            }
          }
        }
      }
      void* _3 = (void*) "Conv2dOutput[ramp(((ax1*160) + (ax2.outer*32)), 1, 32)] = x32((int8)0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t ry_v = 5, ry_v_cnt = 1; ry_v >= 1; ry_v -= max(ry_v / 2, 1), ++ry_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(0, 0, 24), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput[ramp(((ax1*160) + (ax2.outer*32)), 1, 32)]*/);
          request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput.rf[ramp((ry.v*32), 1, 32)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(0, 0, 26), 0, PrecisionT::Precision{0, 8, 0} /**/);
          request.addOperand(sys->getAddress(0, 0, 24), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput[ramp(((ax1*160) + (ax2.outer*32)), 1, 32)]*/);
          request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput.rf[ramp((ry.v*32), 1, 32)]*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed Conv2dOutput_rf
  // freed b_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_len_bnn_3__ = pimsim::registerFunc("len_bnn_3", len_bnn_3);

