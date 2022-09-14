// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t mv_multi_tile_compiled(System *sys) {
  void* _1 = nullptr;
  // int8_t b_local[4096], 0
  // int32_t c_global[1024], 4096
  // int32_t c_global_rf[1024], 5120
  // int8_t a_global[256], 6144
  // cram-tile axis
  for (int32_t x_outer = 0; x_outer < 128; ++x_outer) {
    {
      Request request(Request::Type::RowLoad_RF); // Load register
      request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT8);
      request.addOperand(x_outer * 32 /*tile no*/, 0, PrecisionT::INT8);
      sys->sendRequest(request);
    }
    // skipped by pragma_cram-load-rf
    {
      // initialization skipped
    }
    for (int32_t k_v_outer = 0, acc_offset_k_v_outer = 1, precision_cnt_k_v_outer; k_v_outer < 32; ++k_v_outer) {
      // cram-array axis
      {
        int32_t k_inner = 0;
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((0) * 1/*bytes*/) / 32/*row-number*/ + 192/*cram buffer*/), 0, PrecisionT::INT8); // dst
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT8); // src
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowStore);
          int32_t _2 = k_inner * 256;
          request.type = Request::Type::RowMul;
          request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((0) * 1/*bytes*/) / 32/*row-number*/ + 192/*cram buffer*/), 0, PrecisionT::INT8);
          request.type = Request::Type::RowMul_CRAM_RF;
          request.addOperand(0, 0, PrecisionT::INT8);
          request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((_2) * 4/*bytes*/) / 32/*row-number*/ + 160/*cram buffer*/), 0, PrecisionT::INT32/*Store*/);
          request.precision_list[2] = PrecisionT::Precision{false, 15, 0};
          sys->sendRequest(request);
        }
      }
      for (int32_t k_v_inner = 128, k_v_inner_cnt = 1; k_v_inner > 1; k_v_inner -= k_v_inner / 2, ++k_v_inner_cnt) {
        {
          Request request(Request::Type::RowStore);
          request.type = Request::Type::RowAdd;
          request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 128/*cram buffer*/), 0, PrecisionT::Precision{false, 15 + k_v_inner_cnt, 0});
          int32_t _3 = k_v_inner * 256;
          {
            Request request(Request::Type::BlockSend_Receive);
            request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((_3) * 4/*bytes*/) / 32/*row-number*/ + 160/*cram buffer*/), 0, PrecisionT::Precision{false, 15 + k_v_inner_cnt, 0});
            request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((_3) * 4/*bytes*/) / 32/*row-number*/ + 160/*cram buffer*/), 0, PrecisionT::Precision{false, 15 + k_v_inner_cnt, 0});
            sys->sendRequest(request);
          }
          request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((_3) * 4/*bytes*/) / 32/*row-number*/ + 160/*cram buffer*/), 0, PrecisionT::Precision{false, 15 + k_v_inner_cnt, 0});
          request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 128/*cram buffer*/), 0, PrecisionT::INT32/*Store*/);
          precision_cnt_k_v_outer = k_v_outer;
          acc_offset_k_v_outer += (precision_cnt_k_v_outer & -precision_cnt_k_v_outer) == precision_cnt_k_v_outer;
          request.precision_list[0] = PrecisionT::Precision{false, k_v_inner != 2 ? (15 + k_v_inner_cnt) : (15 + k_v_inner_cnt) + acc_offset_k_v_outer, 0};
          request.precision_list[1] = PrecisionT::Precision{false, k_v_inner != 2 ? (15 + k_v_inner_cnt) : (15 + k_v_inner_cnt) + acc_offset_k_v_outer, 0};
          request.precision_list[2] = PrecisionT::Precision{false, k_v_inner != 2 ? (15 + k_v_inner_cnt) : (15 + k_v_inner_cnt) + acc_offset_k_v_outer, 0};
          sys->sendRequest(request);
        }
      }
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(x_outer/*tile no*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 128/*cram buffer*/), 0, PrecisionT::INT32); // src
      request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT32); // dst
      sys->sendRequest(request);
    }
  }
  // freed a_global
  // freed c_global_rf
  // freed c_global
  // freed b_local
  return 0;
}

static __attribute__((unused)) Registry::Entry &__mv_multi_tile_compiled__ = pimsim::registerFunc("mv_multi_tile_compiled", mv_multi_tile_compiled);
