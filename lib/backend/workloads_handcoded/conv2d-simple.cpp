// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t conv2d_simple(System *sys) {
  void* _1 = nullptr;
  // int8_t w_global[4608], 0
  // int32_t Conv2dOutput_repl_global[1024], 4608
  // int32_t Conv2dOutput_rf[1024], 5632
  for (int32_t ax0 = 0; ax0 < 3; ++ax0) {
    for (int32_t ax1 = 0; ax1 < 3; ++ax1) {
      for (int32_t ax2 = 0; ax2 < 2; ++ax2) {
        {
          int32_t _2 = ax0 * 1536;
          int32_t _3 = ax1 * 512;
          int32_t _4 = _2 + _3;
          int32_t _5 = ax2 * 256;
          int32_t _6 = _4 + _5;
          Request request(Request::Type::RowLoad);
          request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((_6) * 1/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT8); // dst
          request.addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT8); // src
          sys->sendRequest(request);
        }
      }
    }
  }
  // cram-array axis
  {
    int32_t ax1_ax2_fused = 0;
    {
      // initialization skipped
    }
    for (int32_t rc_v = 0; rc_v < 2; ++rc_v) {
      {
        // initialization skipped
      }
      for (int32_t ry = 0; ry < 3; ++ry) {
        for (int32_t rx = 0; rx < 3; ++rx) {
          {
            Request request(Request::Type::RowStore);
            request.type = Request::Type::RowAdd;
            request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 176/*cram buffer*/), 0, PrecisionT::INT32);
            {
              Request request(Request::Type::RowMul);
              request.type = Request::Type::RowMul_CRAM_RF;
              request.addAddr(0, 0, PrecisionT::INT8);
              int32_t _7 = ry * 1536;
              int32_t _8 = rx * 512;
              int32_t _9 = _7 + _8;
              int32_t _10 = rc_v * 256;
              int32_t _11 = _9 + _10;
              request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((_11) * 1/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT8);
              request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 176/*cram buffer*/), 0, PrecisionT::INT32);
              sys->sendRequest(request);
            }
            request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 176/*cram buffer*/), 0, PrecisionT::INT32);
            request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 176/*cram buffer*/), 0, PrecisionT::INT32);
            request.swapSrcDst(); // for source, dest, source operand order
            sys->sendRequest(request);
          }
        }
      }
      {
        Request request(Request::Type::RowStore);
        request.type = Request::Type::RowAdd;
        request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 144/*cram buffer*/), 0, PrecisionT::INT32);
        request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 176/*cram buffer*/), 0, PrecisionT::INT32);
        request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 144/*cram buffer*/), 0, PrecisionT::INT32);
        request.swapSrcDst(); // for source, dest, source operand order
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowStore);
      request.addAddr(sys->getAddress(0/*TODO: for multi-tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 144/*cram buffer*/), 0, PrecisionT::INT32); // src
      request.addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT32); // dst
      sys->sendRequest(request);
    }
  }
  // freed Conv2dOutput_rf
  // freed Conv2dOutput_repl_global
  // freed w_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &__conv2d_simple__ = pimsim::registerFunc("conv2d_simple", conv2d_simple);
