// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t mv_sensitivity_compiled_4_16(System *sys) {
  void* _1 = nullptr;
  // int32_t c_global_rf[1024], 0
  // int8_t a_global[256], 1024
  // int32_t c_global[1024], 1280
  // cram-tile axis
  for (int32_t x_outer_outer = 0; x_outer_outer < 4; ++x_outer_outer) {
    for (int32_t x_outer_inner = 0; x_outer_inner < 32; ++x_outer_inner) {
      // cram-array axis
      {
        int32_t k_outer = 0;
        {
          // initialization skipped
        }
        for (int32_t k_inner = 0, acc_offset_k_inner = 1, precision_cnt_k_inner; k_inner < 256; ++k_inner) {
          {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((0) * 1/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT8); // dst
            request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT8); // src
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowStore);
            int32_t _2 = k_outer * 256;
            request.type = Request::Type::RowAdd;
            request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((_2) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
            {
              Request request(Request::Type::RowMul);
              request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((0) * 1/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT8);
              int32_t _3 = _2 + k_inner;
              // implicitly load to cram register file
              if (_3 % 128 == 0) {
                Request request(Request::Type::RowLoad_RF);
                request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT8);
                request.addOperand(x_outer_outer * 32 /*tile no*/, 0, PrecisionT::INT8);
                sys->sendRequest(request);
              }
              request.type = Request::Type::RowMul_CRAM_RF;
              request.addOperand(0, 0, PrecisionT::INT8);
              request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((_2) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::Precision{false, 15, 0}/*Intermediate*/);
              sys->sendRequest(request);
            }
            request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((_2) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32);
            request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((_2) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT32/*Store*/);
            precision_cnt_k_inner = k_inner;
            acc_offset_k_inner += (precision_cnt_k_inner & -precision_cnt_k_inner) == precision_cnt_k_inner;
            request.precision_list[0] = PrecisionT::Precision{false, 15 + acc_offset_k_inner, 0};
            request.precision_list[1] = PrecisionT::Precision{false, 15 + acc_offset_k_inner, 0};
            request.precision_list[2] = PrecisionT::Precision{false, 15 + acc_offset_k_inner, 0};
            sys->sendRequest(request);
          }
        }
      }
      {
        // initialization skipped
      }
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t k_outer_v = 16, k_outer_v_cnt = 1; k_outer_v >= 1; k_outer_v -= max(k_outer_v / 2, 1), ++k_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::RowStore);
          request.type = Request::Type::RowAdd;
          request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 40/*cram buffer*/), 0, PrecisionT::Precision{false, 24 + k_outer_v_cnt, 0});
          int32_t _4 = k_outer_v * 256;
          {
            Request request(Request::Type::BlockSend_Receive);
            request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((_4) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::Precision{false, 24 + k_outer_v_cnt, 0});
            request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((_4) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::Precision{false, 24 + k_outer_v_cnt, 0});
            sys->sendRequest(request);
          }
          request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((_4) * 4/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::Precision{false, 24 + k_outer_v_cnt, 0});
          request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 40/*cram buffer*/), 0, PrecisionT::INT32/*Store*/);
          request.precision_list[0] = PrecisionT::Precision{false, (24 + k_outer_v_cnt) + (k_outer_v != 1) * 0 + 0, 0};
          request.precision_list[1] = PrecisionT::Precision{false, (24 + k_outer_v_cnt) + (k_outer_v != 1) * 0 + 0, 0};
          request.precision_list[2] = PrecisionT::Precision{false, (24 + k_outer_v_cnt) + (k_outer_v != 1) * 0 + 0, 0};
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(x_outer_outer/*tile no*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 40/*cram buffer*/), 0, PrecisionT::INT32); // src
        request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT32); // dst
        sys->sendRequest(request);
      }
    }
  }
  // freed c_global
  // freed a_global
  // freed c_global_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &__mv_sensitivity_compiled_4_16__ = pimsim::registerFunc("mv_sensitivity_compiled_4_16", mv_sensitivity_compiled_4_16);
