// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t vgg_bnn_13_1(System *sys,std::string param_file) {
  void* _1 = nullptr;
  // int8 placeholder_global[2883], 0
  uint64_t _2 = (uint64_t)8448;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 32);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
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
  // cram-array axis
  {
    int32_t ax11 = 0;
    // cram-array axis
    {
      int32_t ax2_outer = 0;
      // cram-array axis
      {
        int32_t ry = 0;
        void* _6 = (void*) "Conv2dOutput.rf[ramp((ry*192), 1, 192)] = x192(0)/*skip-init*/";
        for (int32_t rx = 0; rx < 11; ++rx) {
          for (int32_t rc = 0; rc < 3; ++rc) {
            {
              Request request(Request::Type::RowLoad);
              request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
              request.addOperand(sys->getAddress(0, 0, 96), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[(x192((((ry*3168) + (rx*288)) + (rc*96))) + (let rmod = (ramp(0, 1, 192) % x192(96)) in select(((x192((bool)1) && (x192(0) <= rmod)) || (x192((bool)0) && (rmod <= x192(0)))), rmod, (rmod + x192(96)))))]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul);
              request.addOperand(sys->getAddress(0, 0, 108), 0, PrecisionT::Precision{0, 16, 0} /**/);
              request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*placeholder.global[(((x192((((ax1*372) + (ry*93)) + (ax2.outer*24))) + ((let rmod = (ramp(0, 1, 192) % x192(96)) in (let rdiv = (ramp(0, 1, 192)/x192(96)) in select(((x192((bool)1) && (x192(0) <= rmod)) || (x192((bool)0) && (rmod <= x192(0)))), rdiv, (rdiv - x192(1)))))*x192(12))) + x192((rx*3))) + x192(rc))]*/);
              request.addOperand(sys->getAddress(0, 0, 96), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[(x192((((ry*3168) + (rx*288)) + (rc*96))) + (let rmod = (ramp(0, 1, 192) % x192(96)) in select(((x192((bool)1) && (x192(0) <= rmod)) || (x192((bool)0) && (rmod <= x192(0)))), rmod, (rmod + x192(96)))))]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(0, 0, 132), 0, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(0, 0, 96), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((ry*192), 1, 192)]*/);
              request.addOperand(sys->getAddress(0, 0, 108), 0, PrecisionT::Precision{0, 32, 0} /**/);
              sys->sendRequest(request);
            }
          }
        }
      }
      void* _7 = (void*) "Conv2dOutput[ramp(((ax1*576) + (ax2.outer*192)), 1, 192)] = x192(0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t ry_v = 11, ry_v_cnt = 1; ry_v >= 1; ry_v -= max(ry_v / 2, 1), ++ry_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(0, 0, 120), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(((ax1*576) + (ax2.outer*192)), 1, 192)]*/);
          request.addOperand(sys->getAddress(0, 0, 132), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((ry.v*192), 1, 192)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(0, 0, 144), 0, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(0, 0, 120), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(((ax1*576) + (ax2.outer*192)), 1, 192)]*/);
          request.addOperand(sys->getAddress(0, 0, 132), 0, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput.rf[ramp((ry.v*192), 1, 192)]*/);
          sys->sendRequest(request);
        }
      }
    }
  }
  int32_t _8 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _9 = _8 != 0;
  if (_9) {
    return -1;
      }
  // freed placeholder_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &_vgg_bnn_13_1__ = pimsim::registerFunc("vgg_bnn_13_1", vgg_bnn_13_1);

