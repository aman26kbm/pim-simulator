// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t conv_reduce(System *sys) {
  void* _1 = nullptr;
  // int32_t Conv2dOutput_rf[1024], 0
  // int8_t w_global[2304], 1024
  // cram-array axis
  {
    int32_t ax1_ax2_fused = 0;
    for (int32_t rc = 0; rc < 128; ++rc) {
      for (int32_t ax0 = 0; ax0 < 3; ++ax0) {
        for (int32_t ax1 = 0; ax1 < 3; ++ax1) {
          {
            int32_t _2 = ax0 * 768;
            int32_t _3 = ax1 * 256;
            int32_t _4 = _2 + _3;
            Request request(Request::Type::RowLoad);
            request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((_4) * 1/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT8); // dst
            request.addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT8); // src
            sys->sendRequest(request);
          }
        }
      }
      {
        // initialization skipped
      }
      for (int32_t ry = 0; ry < 3; ++ry) {
        for (int32_t rx = 0; rx < 3; ++rx) {
          {
            Request request(Request::Type::RowStore);
            int32_t _5 = rc * 256;
            request.type = Request::Type::RowAdd;
            request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((_5) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
            {
              Request request(Request::Type::RowMul);
              request.type = Request::Type::RowMul_CRAM_RF;
              request.addAddr(0, 0, PrecisionT::INT8);
              int32_t _6 = ry * 768;
              int32_t _7 = rx * 256;
              int32_t _8 = _6 + _7;
              request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((_8) * 1/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT8);
              request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((_5) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
              sys->sendRequest(request);
            }
            request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((_5) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
            request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((_5) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
            request.swapSrcDst(); // for source, dest, source operand order
            sys->sendRequest(request);
          }
        }
      }
    }
    {
      // initialization skipped
    }
    for (int32_t rc_v = 128, delta_rc_v = 64; rc_v > 1; rc_v -= delta_rc_v, delta_rc_v >>= 1) {
      {
        Request request(Request::Type::RowStore);
        request.type = Request::Type::RowAdd;
        request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT32);
        int32_t _9 = rc_v * 256;
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((_9) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
          request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((_9) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
          sys->sendRequest(request);
        }
        request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((_9) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
        request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT32);
        request.swapSrcDst(); // for source, dest, source operand order
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowStore);
      request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT32); // src
      request.addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT32); // dst
      sys->sendRequest(request);
    }
  }
  // freed w_global
  // freed Conv2dOutput_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &__conv_reduce__ = pimsim::registerFunc("conv_reduce", conv_reduce);
