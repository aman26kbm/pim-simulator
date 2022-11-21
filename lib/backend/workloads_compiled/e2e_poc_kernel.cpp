// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

namespace e2e_poc {
int32_t tvmgen_default_fused_layout_transform(System *sys) {
  void* _1 = nullptr;
  for (int32_t ax1 = 0; ax1 < 224; ++ax1) {
    for (int32_t ax2 = 0; ax2 < 224; ++ax2) {
      for (int32_t ax3 = 0; ax3 < 4; ++ax3) {
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); // src
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); // dst
          sys->sendRequest(request);
        }
      }
    }
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_e2e_poc_tvmgen_default_fused_layout_transform__ = pimsim::registerFunc("e2e_poc_tvmgen_default_fused_layout_transform", tvmgen_default_fused_layout_transform);

int32_t tvmgen_default_fused_layout_transform_1(System *sys) {
  void* _1 = nullptr;
  for (int32_t ax1 = 0; ax1 < 64; ++ax1) {
    for (int32_t ax2 = 0; ax2 < 112; ++ax2) {
      for (int32_t ax3 = 0; ax3 < 112; ++ax3) {
        {
          Request request(Request::Type::RowStore);
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); // src
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); // dst
          sys->sendRequest(request);
        }
      }
    }
  }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_e2e_poc_tvmgen_default_fused_layout_transform_1__ = pimsim::registerFunc("e2e_poc_tvmgen_default_fused_layout_transform_1", tvmgen_default_fused_layout_transform_1);

int32_t tvmgen_default_fused_nn_conv2d_cast(System *sys) {
  void* _1 = nullptr;
  uint64_t _2 = (uint64_t)405000;
  void* _3 = TVMBackendAllocWorkspace(1, _1, _2, 0, 16);
  bool _4 = _3 == NULL;
  if (_4) {
    return -1;
      }
  // int32_t Conv2dOutput[256], 0
  for (int32_t ax1 = 0; ax1 < 112; ++ax1) {
    // cram-array axis
    {
      int32_t ax2 = 0;
      void* _6 = (void*) "Conv2dOutput[ramp(0, 1, 64)] = x64(0)/*skip-init*/";
      for (int32_t ry = 0; ry < 3; ++ry) {
        for (int32_t rx = 0; rx < 3; ++rx) {
          for (int32_t rc = 0; rc < 4; ++rc) {
            {
              Request request(Request::Type::RowLoad);
              request.addOperand(sys->getAddress(ax1, 0, 0), 112, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
              request.addOperand(sys->getAddress(ax1, 0, 0), 112, PrecisionT::Precision{0, 8, 0} /*PaddedInput[(((((ax1*1800) + (ry*900)) + (ax2*8)) + (rx*4)) + rc)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowMul);
              request.addOperand(sys->getAddress(ax1, 0, 12), 7168, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax1, 0, 0), 112, PrecisionT::Precision{0, 8, 0} /*PaddedInput[(((((ax1*1800) + (ry*900)) + (ax2*8)) + (rx*4)) + rc)]*/);
              request.addOperand(sys->getAddress(ax1, 0, 8), 64, PrecisionT::Precision{0, 8, 0} /*placeholder[ramp((((ry*768) + (rx*256)) + (rc*64)), 1, 64)]*/);
              sys->sendRequest(request);
            }
            {
              Request request(Request::Type::RowAdd);
              request.addOperand(sys->getAddress(ax1, 0, 0), 7168, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax1, 0, 12), 7168, PrecisionT::Precision{0, 32, 0} /**/);
              request.addOperand(sys->getAddress(ax1, 0, 0), 7168, PrecisionT::Precision{0, 32, 0} /*Conv2dOutput[ramp(0, 1, 64)]*/);
              sys->sendRequest(request);
            }
          }
        }
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(ax1, 0, 0), 7168, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(ax1, 0, 0), 7168, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed Conv2dOutput
  int32_t _7 = TVMBackendFreeWorkspace(1, _1, _3);
  bool _8 = _7 != 0;
  if (_8) {
    return -1;
      }
  return 0;
}

static __attribute__((unused)) Registry::Entry &_e2e_poc_tvmgen_default_fused_nn_conv2d_cast__ = pimsim::registerFunc("e2e_poc_tvmgen_default_fused_nn_conv2d_cast", tvmgen_default_fused_nn_conv2d_cast);

} // e2e_poc