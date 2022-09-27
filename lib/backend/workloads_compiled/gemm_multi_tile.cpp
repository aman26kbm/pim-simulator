// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t gemm_multi_tile_compiled(System *sys) {
  void* _1 = nullptr;
  // int32_t compute_global_rf[2048], 0
  // int8_t a_global[256], 2048
  // int32_t compute_global[2048], 2304
  // cram-tile axis
  for (int32_t xo = 0; xo < 128; ++xo) {
    // cram-array axis
    {
      int32_t y_outer = 0;
      // cram-array axis
      {
        int32_t rv_outer = 0;
        for (int32_t y_c_init = 0; y_c_init < 2; ++y_c_init) {
          {
            // initialization skipped
          }
        }
        for (int32_t rv_inner = 0; rv_inner < 256; ++rv_inner) {
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((0) * 1/*bytes*/) / 32/*row-number*/ + 64/*cram buffer*/), 0, PrecisionT::INT8); // dst
            request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT8); // src
            sys->sendRequest(request);
          }
          for (int32_t y_c = 0; y_c < 2; ++y_c) {
            {
              Request request(Request::Type::RowStore);
              int32_t _2 = rv_outer * 512;
              int32_t _3 = y_c * 256;
              int32_t _4 = _2 + _3;
              request.type = Request::Type::RowAdd;
              request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_4) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
              {
                Request request(Request::Type::RowMul);
                request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((0) * 1/*bytes*/) / 32/*row-number*/ + 64/*cram buffer*/), 0, PrecisionT::INT8);
                int32_t _5 = y_outer * 4096;
                int32_t _6 = y_c * 2048;
                int32_t _7 = _5 + _6;
                int32_t _8 = rv_outer * 256;
                int32_t _9 = _7 + _8;
                int32_t _10 = _9 + rv_inner;
                // implicitly load to cram register file
                if (_10 % 128 == 0) {
                  Request request(Request::Type::RowLoad_RF);
                  request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT8);
                  request.addOperand(xo * 32 /*tile no*/, 0, PrecisionT::INT8);
                  sys->sendRequest(request);
                }
                request.type = Request::Type::RowMul_CRAM_RF;
                request.addOperand(0, 0, PrecisionT::INT8);
                request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_4) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32/*Intermediate*/);
                sys->sendRequest(request);
              }
              request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_4) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
              request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_4) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32/*Store*/);
              sys->sendRequest(request);
            }
          }
        }
      }
      for (int32_t ax1_init = 0; ax1_init < 2; ++ax1_init) {
        {
          // initialization skipped
        }
      }
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rv_outer_v = 8, rv_outer_v_cnt = 1; rv_outer_v >= 1; rv_outer_v -= max(rv_outer_v / 2, 1), ++rv_outer_v_cnt) {
      #undef max
        for (int32_t ax1 = 0; ax1 < 2; ++ax1) {
          {
            Request request(Request::Type::RowStore);
            int32_t _11 = ax1 * 256;
            request.type = Request::Type::RowAdd;
            request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_11) * 4/*bytes*/) / 32/*row-number*/ + 72/*cram buffer*/), 0, PrecisionT::INT32);
            int32_t _12 = rv_outer_v * 512;
            int32_t _13 = _12 + _11;
            {
              Request request(Request::Type::BlockSend_Receive);
              request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_13) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
              request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_13) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
              sys->sendRequest(request);
            }
            request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_13) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
            request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_11) * 4/*bytes*/) / 32/*row-number*/ + 72/*cram buffer*/), 0, PrecisionT::INT32/*Store*/);
            sys->sendRequest(request);
          }
        }
      }
      for (int32_t y_inner = 0; y_inner < 2; ++y_inner) {
        {
          int32_t _14 = y_inner * 256;
          Request request(Request::Type::RowStore);
          request.addOperand(sys->getAddress(xo/*tile no*/, 0/*block-id*/, ((_14) * 4/*bytes*/) / 32/*row-number*/ + 72/*cram buffer*/), 0, PrecisionT::INT32); // src
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT32); // dst
          sys->sendRequest(request);
        }
      }
    }
  }
  // freed compute_global
  // freed a_global
  // freed compute_global_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &__gemm_multi_tile_compiled__ = pimsim::registerFunc("gemm_multi_tile_compiled", gemm_multi_tile_compiled);
