// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t vgg_bnn_13_512_256(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)65536;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 8);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int8 Conv2dOutput[256], 0
  // cram-array axis
  {
    int32_t rc_outer = 0;
    void* _6 = (void*) "Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)] = x256((int8)0)/*skip-init*/";
    for (int32_t ry = 0; ry < 3; ++ry) {
      for (int32_t rx = 0; rx < 3; ++rx) {
        for (int32_t rc_inner = 0; rc_inner < 2; ++rc_inner) {
          {
            Request request(Request::Type::BlockBroadCast);
            request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*DRAM*/);
            request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*image[((((ry*1536) + (rx*512)) + (rc.outer*2)) + rc.inner)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*DRAM*/);
            request.addOperand(sys->getAddress(0, 0, 8), 0, PrecisionT::Precision{0, 1, 0} /*kernel[ramp(((((ry*393216) + (rx*131072)) + (rc.outer*512)) + (rc.inner*256)), 1, 256)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowMul);
            request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 2, 0} /**/);
            request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 1, 0} /*image[((((ry*1536) + (rx*512)) + (rc.outer*2)) + rc.inner)]*/);
            request.addOperand(sys->getAddress(0, 0, 8), 0, PrecisionT::Precision{0, 1, 0} /*kernel[ramp(((((ry*393216) + (rx*131072)) + (rc.outer*512)) + (rc.inner*256)), 1, 256)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(0, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /**/);
            request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /**/);
            request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput.rf[ramp((rc.outer*256), 1, 256)]*/);
            sys->sendRequest(request);
          }
        }
      }
    }
  }
  void* _7 = (void*) "Conv2dOutput[ramp(0, 1, 256)] = x256((int8)0)/*skip-init*/";
  #define max(a,b) ((a)>(b)?(a):(b))
  for (int32_t rc_outer_v = 256, rc_outer_v_cnt = 1; rc_outer_v >= 1; rc_outer_v -= max(rc_outer_v / 2, 1), ++rc_outer_v_cnt) {
  #undef max
    {
      Request request(Request::Type::BlockSend_Receive);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
      request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
      request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput.rf[ramp((rc.outer.v*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
  }
  {
    Request request(Request::Type::RowLoad);
    request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
    request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(0, 1, 256)]*/);
    sys->sendRequest(request);
  }
  {
    Request request(Request::Type::RowMul);
    request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /**/);
    request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
    request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(0, 1, 256)]*/);
    sys->sendRequest(request);
  }
  {
    Request request(Request::Type::RowLoad);
    request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
    request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(0, 1, 256)]*/);
    sys->sendRequest(request);
  }
  {
    Request request(Request::Type::RowAdd);
    request.addOperand(sys->getAddress(0, 0, 32), 0, PrecisionT::Precision{0, 8, 0} /**/);
    request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{0, 8, 0} /*Conv2dOutput[ramp(0, 1, 256)]*/);
    request.addOperand(sys->getAddress(0, 0, 16), 0, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp(0, 1, 256)]*/);
    sys->sendRequest(request);
  }
  // freed Conv2dOutput
  int32_t _8 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _9 = _8 != 0;
  if (_9) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_vgg_bnn_13_512_256__ = pimsim::registerFunc("vgg_bnn_13_512_256", vgg_bnn_13_512_256);

