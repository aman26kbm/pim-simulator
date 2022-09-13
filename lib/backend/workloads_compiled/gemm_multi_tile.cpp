// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t gemm_multi_tile_compiled(System *sys) {
  void* _1 = nullptr;
  // int32_t compute_global[2048], 0
  // int32_t compute_global_rf[2048], 2048
  // int8_t a_global[256], 4096
  // cram-tile axis
  for (int32_t xo = 0; xo < 128; ++xo) {
    // cram-array axis
    {
      int32_t y_outer = 0;
      for (int32_t ax1_init = 0; ax1_init < 2; ++ax1_init) {
        {
          // initialization skipped
        }
      }
      for (int32_t rv_v_outer = 0; rv_v_outer < 256; ++rv_v_outer) {
        // cram-array axis
        {
          int32_t rv = 0;
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((0) * 1/*bytes*/) / 32/*row-number*/ + 128/*cram buffer*/), 0, PrecisionT::INT8); // dst
            request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT8); // src
            sys->sendRequest(request);
          }
          for (int32_t y_c = 0; y_c < 2; ++y_c) {
            {
              Request request(Request::Type::RowStore);
              int32_t _2 = rv * 512;
              int32_t _3 = y_c * 256;
              int32_t _4 = _2 + _3;
              request.type = Request::Type::RowMul;
              request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((0) * 1/*bytes*/) / 32/*row-number*/ + 128/*cram buffer*/), 0, PrecisionT::INT8);
              request.type = Request::Type::RowMul_CRAM_RF;
              request.addOperand(0, 0, PrecisionT::INT8);
              request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_4) * 4/*bytes*/) / 32/*row-number*/ + 64/*cram buffer*/), 0, PrecisionT::INT32);
              request.swapSrcDst(); // for source, dest, source operand order
              sys->sendRequest(request);
            }
          }
        }
        for (int32_t ax1 = 0; ax1 < 2; ++ax1) {
          for (int32_t rv_v_inner = 8; rv_v_inner > 1; rv_v_inner -= rv_v_inner / 2) {
            {
              Request request(Request::Type::RowStore);
              int32_t _5 = ax1 * 256;
              request.type = Request::Type::RowAdd;
              request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_5) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
              int32_t _6 = rv_v_inner * 512;
              int32_t _7 = _6 + _5;
              {
                Request request(Request::Type::BlockSend_Receive);
                request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_7) * 4/*bytes*/) / 32/*row-number*/ + 64/*cram buffer*/), 0, PrecisionT::INT32);
                request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_7) * 4/*bytes*/) / 32/*row-number*/ + 64/*cram buffer*/), 0, PrecisionT::INT32);
                sys->sendRequest(request);
              }
              request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_7) * 4/*bytes*/) / 32/*row-number*/ + 64/*cram buffer*/), 0, PrecisionT::INT32);
              request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_5) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
              request.swapSrcDst(); // for source, dest, source operand order
              sys->sendRequest(request);
            }
          }
        }
      }
      for (int32_t y_inner = 0; y_inner < 2; ++y_inner) {
        {
          int32_t _8 = y_inner * 256;
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_8) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32); // src
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT32); // dst
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed a_global
  // freed compute_global_rf
  // freed compute_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &__gemm_multi_tile_compiled__ = pimsim::registerFunc("gemm_multi_tile_compiled", gemm_multi_tile_compiled);
