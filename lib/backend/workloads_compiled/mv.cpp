// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t mv_compiled(System *sys) {
  void* _1 = nullptr;
  // int32_t c_global[1024], 0
  // int8_t b_local[128], 1024
  // int8_t a_global[256], 1152
  // cram-array axis
  {
    int32_t x_outer = 0;
    {
      // initialization skipped
    }
    for (int32_t k_outer = 0, acc_offset_k_outer = 1, precision_cnt_k_outer; k_outer < 32; ++k_outer) {
      {
        Request request(Request::Type::RowLoad_RF); // Load register
        request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT8);
        request.addOperand(0 /*default tile*/, 0, PrecisionT::INT8);
        sys->sendRequest(request);
      }
      for (int32_t k_inner = 0; k_inner < 128; ++k_inner) {
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(0 /*default tile*/, 0/*block-id*/, ((0) * 1/*bytes*/) / 32/*row-number*/ + 36/*cram buffer*/), 0, PrecisionT::INT8); // dst
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT8); // src
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowStore);
          request.type = Request::Type::RowAdd;
          request.addOperand(sys->getAddress(0 /*default tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
          {
            Request request(Request::Type::RowMul);
            request.addOperand(sys->getAddress(0 /*default tile*/, 0/*block-id*/, ((0) * 1/*bytes*/) / 32/*row-number*/ + 36/*cram buffer*/), 0, PrecisionT::INT8);
            request.type = Request::Type::RowMul_CRAM_RF;
            request.addOperand(0, 0, PrecisionT::INT8);
            request.addOperand(sys->getAddress(0 /*default tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::Precision{false, 15, 0}/*Intermediate*/);
            sys->sendRequest(request);
          }
          request.addOperand(sys->getAddress(0 /*default tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
          request.addOperand(sys->getAddress(0 /*default tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32/*Store*/);
          precision_cnt_k_outer = (k_outer) * 128 + k_inner;
          acc_offset_k_outer += (precision_cnt_k_outer & -precision_cnt_k_outer) == precision_cnt_k_outer;
          request.precision_list[0] = PrecisionT::Precision{false, 15 + acc_offset_k_outer, 0};
          request.precision_list[1] = PrecisionT::Precision{false, 15 + acc_offset_k_outer, 0};
          request.precision_list[2] = PrecisionT::Precision{false, 15 + acc_offset_k_outer, 0};
          sys->sendRequest(request);
        }
      }
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(0 /*default tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32); // src
      request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT32); // dst
      sys->sendRequest(request);
    }
  }
  // freed a_global
  // freed b_local
  // freed c_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &__mv_compiled__ = pimsim::registerFunc("mv_compiled", mv_compiled);
