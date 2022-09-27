// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t fir_compiled(System *sys) {
  void* _1 = nullptr;
  // int16_t a_global[1024], 0
  // int32_t c_global[1024], 1024
  // cram-array axis
  {
    int32_t x_outer_outer = 0;
    for (int32_t x_outer_inner = 0; x_outer_inner < 4; ++x_outer_inner) {
      for (int32_t ax0_outer = 0; ax0_outer < 2; ++ax0_outer) {
        {
          int32_t _2 = ax0_outer * 256;
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(0 /*default tile*/, 0/*block-id*/, ((_2) * 2/*bytes*/) / 32/*row-number*/ + 0/*cram buffer*/), 0, PrecisionT::INT16); // dst
          request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); // src
          sys->sendRequest(request);
        }
      }
      {
        // initialization skipped
      }
      for (int32_t rv = 0; rv < 257; ++rv) {
        {
          Request request(Request::Type::RowStore);
          request.type = Request::Type::RowAdd;
          request.addOperand(sys->getAddress(0 /*default tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT32);
          {
            Request request(Request::Type::RowMul);
            request.addOperand(0, 0, PrecisionT::INT16);
            // implicitly load to cram register file
            if (rv % 64 == 0) {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16);
              request.addOperand(0 /*default tile*/, 0, PrecisionT::INT16);
              sys->sendRequest(request);
            }
            request.type = Request::Type::RowMul_CRAM_RF;
            request.addOperand(0, 0, PrecisionT::INT16);
            request.addOperand(sys->getAddress(0 /*default tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT32/*Intermediate*/);
            sys->sendRequest(request);
            Request shift_req0(Request::Type::RowShift);
            shift_req0.addOperand(0, 0, PrecisionT::INT32);
            shift_req0.addOperand(0, 0, PrecisionT::INT32);
            sys->sendRequest(shift_req0);
          }
          request.addOperand(sys->getAddress(0 /*default tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT32);
          request.addOperand(sys->getAddress(0 /*default tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT32/*Store*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(0 /*default tile*/, 0/*block-id*/, ((0) * 4/*bytes*/) / 32/*row-number*/ + 32/*cram buffer*/), 0, PrecisionT::INT32); // src
        request.addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT32); // dst
        sys->sendRequest(request);
      }
    }
  }
  // freed c_global
  // freed a_global
  return 0;
}

static __attribute__((unused)) Registry::Entry &__fir_compiled__ = pimsim::registerFunc("fir_compiled", fir_compiled);
