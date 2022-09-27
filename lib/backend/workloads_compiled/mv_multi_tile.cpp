// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t mv_multi_tile_compiled(System *sys) {
  void* _1 = nullptr;
  // int32_t c_global_rf[1024], 0
  // int8_t a_global[256], 1024
  // int32_t c_global[1024], 1280
  // cram-tile axis
  for (int32_t x_outer = 0; x_outer < 128; ++x_outer) {
    // cram-array axis
    {
      int32_t k_outer = 0;
      {
        // initialization skipped
      }
      for (int32_t k_inner = 0; k_inner < 32; ++k_inner) {
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((0) * 1/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT8); // dst
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT8); // src
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowStore);
          int32_t _2 = k_outer * 256;
          request.type = Request::Type::RowAdd;
          request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((_2) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
          {
            Request request(Request::Type::RowMul);
            request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((0) * 1/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT8);
            int32_t _3 = k_outer * 32;
            int32_t _4 = _3 + k_inner;
            // implicitly load to cram register file
            if (_4 % 128 == 0) {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT8);
              request.addOperand(x_outer * 32 /*tile no*/, 0, PrecisionT::INT8);
              sys->sendRequest(request);
            }
            request.type = Request::Type::RowMul_CRAM_RF;
            request.addOperand(0, 0, PrecisionT::INT8);
            request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((_2) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32/*Intermediate*/);
            sys->sendRequest(request);
          }
          request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((_2) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
          request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((_2) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32/*Store*/);
          sys->sendRequest(request);
        }
      }
    }
    {
      // initialization skipped
    }
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t k_outer_v = 128, k_outer_v_cnt = 1; k_outer_v >= 1; k_outer_v -= max(k_outer_v / 2, 1), ++k_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::RowStore);
        request.type = Request::Type::RowAdd;
        request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 40/*cram buffer*/), 0, PrecisionT::INT32);
        int32_t _5 = k_outer_v * 256;
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((_5) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
          request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((_5) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
          sys->sendRequest(request);
        }
        request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((_5) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
        request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 40/*cram buffer*/), 0, PrecisionT::INT32/*Store*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 40/*cram buffer*/), 0, PrecisionT::INT32); // src
      request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT32); // dst
      sys->sendRequest(request);
    }
  }
  // freed c_global
  // freed a_global
  // freed c_global_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &__mv_multi_tile_compiled__ = pimsim::registerFunc("mv_multi_tile_compiled", mv_multi_tile_compiled);
