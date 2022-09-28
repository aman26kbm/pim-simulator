// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t fir_multi_tile_compiled(System *sys) {
  void* _1 = nullptr;
  // int32_t c_global[1024], 0
  // int32_t c_global_rf[1024], 1024
  // int16_t a_global[512], 2048
  // cram-tile axis
  for (int32_t x_outer = 0; x_outer < 128; ++x_outer) {
    // cram-array axis
    {
      int32_t x_inner_outer = 0;
      {
        // initialization skipped
      }
      for (int32_t rv_v_outer = 0; rv_v_outer < 8; ++rv_v_outer) {
        // cram-array axis
        {
          int32_t rv = 0;
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((0) * 2/*bytes*/) / 32/*row-number*/ + 64/*cram buffer*/), 0, PrecisionT::INT16); // dst
            request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); // src
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowStore);
            int32_t _2 = rv * 256;
            request.type = Request::Type::RowMul;
            request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((0) * 2/*bytes*/) / 32/*row-number*/ + 64/*cram buffer*/), 0, PrecisionT::INT16);
            int32_t _3 = rv_v_outer * 32;
            int32_t _4 = _3 + rv;
            // implicitly load to cram register file
            if (_4 % 64 == 0) {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16);
              request.addOperand(x_outer * 32 /*tile no*/, 0, PrecisionT::INT16);
              sys->sendRequest(request);
            }
            request.type = Request::Type::RowMul_CRAM_RF;
            request.addOperand(0, 0, PrecisionT::INT16);
            request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((_2) * 4/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT32/*Store*/);
            sys->sendRequest(request);
          }
        }
        for (int32_t rv_v_inner = 32, rv_v_inner_cnt = 1; rv_v_inner > 1; rv_v_inner -= rv_v_inner / 2, ++rv_v_inner_cnt) {
          {
            Request request(Request::Type::RowStore);
            request.type = Request::Type::RowAdd;
            request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
            int32_t _5 = rv_v_inner * 256;
            {
              Request request(Request::Type::BlockSend_Receive);
              request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((_5) * 4/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT32);
              request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((_5) * 4/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT32);
              sys->sendRequest(request);
            }
            request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((_5) * 4/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT32);
            request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32/*Store*/);
            sys->sendRequest(request);
          }
        }
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32); // src
        request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT32); // dst
        sys->sendRequest(request);
      }
    }
  }
  // freed a_global
  // freed c_global_rf
  // freed c_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &__fir_multi_tile_compiled__ = pimsim::registerFunc("fir_multi_tile_compiled", fir_multi_tile_compiled);
