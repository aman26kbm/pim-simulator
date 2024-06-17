// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t vgg_bnn_13_3(System *sys,std::string param_file) {
  void* _1 = nullptr;
  // int8 Conv2dOutput[2304], 0
  uint64_t _2 = (uint64_t)24576;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 8);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int8 tensor[256], 2304
  for (int32_t ax1 = 0; ax1 < 3; ++ax1) {
    for (int32_t ax2_outer = 0; ax2_outer < 2; ++ax2_outer) {
      // cram-array axis
      {
        int32_t ax2_inner = 0;
        // cram-array axis
        {
          int32_t rc = 0;
          void* _6 = (void*) "Conv2dOutput.rf[ramp((rc*256), 1, 256)] = x256((int8)0)/*skip-init*/";
          for (int32_t ry = 0; ry < 5; ++ry) {
            for (int32_t rx = 0; rx < 5; ++rx) {
              {
                Request request(Request::Type::BlockBroadCast);
                request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*DRAM*/);
                request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*image[((((((ax1*768) + (ry*768)) + (ax2.outer*192)) + (ax2.inner*96)) + (rx*96)) + rc)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowLoad);
                request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*DRAM*/);
                request.addOperand(sys->getAddress(0, 0, 80), 0, PrecisionT::Precision{0, 1, 0} /*kernel[ramp((((ry*122880) + (rx*24576)) + (rc*256)), 1, 256)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(0, 0, 88), 0, PrecisionT::Precision{0, 2, 0} /**/);
                request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*image[((((((ax1*768) + (ry*768)) + (ax2.outer*192)) + (ax2.inner*96)) + (rx*96)) + rc)]*/);
                request.addOperand(sys->getAddress(0, 0, 80), 0, PrecisionT::Precision{0, 1, 0} /*kernel[ramp((((ry*122880) + (rx*24576)) + (rc*256)), 1, 256)]*/);
                sys->sendRequest(request);
              }
              {
                Request request(Request::Type::RowAdd);
                request.addOperand(sys->getAddress(0, 0, 104), 0, PrecisionT::Precision{0, 8, 0} /**/);
                request.addOperand(sys->getAddress(0, 0, 88), 0, PrecisionT::Precision{0, 8, 0} /**/);
                request.addOperand(sys->getAddress(0, 0, 88), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput.rf[ramp((rc*256), 1, 256)]*/);
                sys->sendRequest(request);
              }
            }
          }
        }
        int32_t _7 = ax2_outer * 2;
        int32_t _8 = _7 + ax2_inner;
        bool _9 = _8 < 3;
        if (_9) {
          void* _10 = (void*) "Conv2dOutput[ramp((((ax1*768) + (ax2.outer*512)) + (ax2.inner*256)), 1, 256)] = x256((int8)0)/*skip-init*/";
        }
        #define max(a,b) ((a)>(b)?(a):(b))
        for (int32_t rc_v = 96, rc_v_cnt = 1; rc_v >= 1; rc_v -= max(rc_v / 2, 1), ++rc_v_cnt) {
        #undef max
          int32_t _11 = ax2_outer * 2;
          int32_t _12 = _11 + ax2_inner;
          bool _13 = _12 < 3;
          if (_13) {
            {
              Request request(Request::Type::BlockSend_Receive);
              request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput[ramp((((ax1*768) + (ax2.outer*512)) + (ax2.inner*256)), 1, 256)]*/);
              request.addOperand(sys->getAddress(0, 0, 88), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput.rf[ramp((rc.v*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /**/);
              request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput[ramp((((ax1*768) + (ax2.outer*512)) + (ax2.inner*256)), 1, 256)]*/);
              request.addOperand(sys->getAddress(0, 0, 88), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput.rf[ramp((rc.v*256), 1, 256)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
    }
  }
  void* _14 = (void*) "tensor[ramp(0, 1, 256)] = x256((int8)-128)/*skip-init*/";
  for (int32_t rv0 = 0; rv0 < 3; ++rv0) {
    for (int32_t rv1 = 0; rv1 < 3; ++rv1) {
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(0, 0, 72), 0, PrecisionT::Precision{0, 8, 0} /**/);
        request.addOperand(sys->getAddress(0, 0, 72), 0, PrecisionT::Precision{0, 8, 0} /*tensor[ramp(0, 1, 256)]*/);
        request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput[ramp(((rv0*768) + (rv1*256)), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
  }
  {
    Request request(Request::Type::RowLoad);
    request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
    request.addOperand(sys->getAddress(0, 0, 88), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(0, 1, 256)]*/);
    sys->sendRequest(request);
  }
  {
    Request request(Request::Type::RowMul);
    request.addOperand(sys->getAddress(0, 0, 72), 0, PrecisionT::Precision{0, 8, 0} /**/);
    request.addOperand(sys->getAddress(0, 0, 72), 0, PrecisionT::Precision{0, 8, 0} /*tensor[ramp(0, 1, 256)]*/);
    request.addOperand(sys->getAddress(0, 0, 88), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(0, 1, 256)]*/);
    sys->sendRequest(request);
  }
  {
    Request request(Request::Type::RowLoad);
    request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
    request.addOperand(sys->getAddress(0, 0, 88), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(0, 1, 256)]*/);
    sys->sendRequest(request);
  }
  {
    Request request(Request::Type::RowAdd);
    request.addOperand(sys->getAddress(0, 0, 104), 0, PrecisionT::Precision{0, 8, 0} /**/);
    request.addOperand(sys->getAddress(0, 0, 72), 0, PrecisionT::Precision{0, 8, 0} /*tensor[ramp(0, 1, 256)]*/);
    request.addOperand(sys->getAddress(0, 0, 88), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(0, 1, 256)]*/);
    sys->sendRequest(request);
  }
  // freed tensor
  int32_t _15 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _16 = _15 != 0;
  if (_16) {
    return -1;
      }
  // freed Conv2dOutput
  return 0;
}

static __attribute__((unused)) Registry::Entry &_vgg_bnn_13_3__ = pimsim::registerFunc("vgg_bnn_13_3", vgg_bnn_13_3);

