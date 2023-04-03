// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t backprop_25600_45(System *sys) {
  void* _1 = nullptr;
  // float32 compute_rf[1024], 0
  for (int32_t ax0 = 0; ax0 < 45; ++ax0) {
    // cram-array axis
    {
      int32_t rv_outer = 0;
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0, 0, 0), 25600, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0, 0, 32), 25600, PrecisionT::Precision{1, 23, 8} /*placeholder[ramp((rv.outer*256), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(ax0, 0, 0), 25600, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
        request.addOperand(sys->getAddress(ax0, 0, 64), 25600, PrecisionT::Precision{1, 23, 8} /*placeholder[ramp(((ax0*25600) + (rv.outer*256)), 1, 256)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul);
        request.addOperand(sys->getAddress(ax0, 0, 0), 25600, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(ax0, 0, 32), 25600, PrecisionT::Precision{1, 23, 8} /*placeholder[ramp((rv.outer*256), 1, 256)]*/);
        request.addOperand(sys->getAddress(ax0, 0, 64), 25600, PrecisionT::Precision{1, 23, 8} /*placeholder[ramp(((ax0*25600) + (rv.outer*256)), 1, 256)]*/);
        sys->sendRequest(request);
      }
    }
    void* _2 = (void*) "compute[ax0] = 0f/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t rv_v_outer = 100, rv_v_outer_cnt = 1; rv_v_outer >= 1; rv_v_outer -= max(rv_v_outer / 2, 1), ++rv_v_outer_cnt) {
    #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*compute.rf[((rv.v.outer*256) + rv.v.inner)]*/);
          request.addOperand(sys->getAddress(ax0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*compute[ax0]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd_CRAM_RF);
          request.addOperand(sys->getAddress(ax0, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(ax0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*compute.rf[((rv.v.outer*256) + rv.v.inner)]*/);
          request.addOperand(sys->getAddress(ax0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*compute[ax0]*/);
          sys->sendRequest(request);
        }
    }
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t rv_v_outer = 256, rv_v_outer_cnt = 1; rv_v_outer >= 1; rv_v_outer -= max(rv_v_outer / 2, 1), ++rv_v_outer_cnt) {
    #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(ax0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*compute.rf[((rv.v.outer*256) + rv.v.inner)]*/);
          request.addOperand(sys->getAddress(ax0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*compute[ax0]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd_CRAM_RF);
          request.addOperand(sys->getAddress(ax0, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(ax0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*compute.rf[((rv.v.outer*256) + rv.v.inner)]*/);
          request.addOperand(sys->getAddress(ax0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*compute[ax0]*/);
          sys->sendRequest(request);
        }
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(ax0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
      request.addOperand(sys->getAddress(ax0, 0, -1), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  // freed compute_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_backprop_25600_45__ = pimsim::registerFunc("backprop_25600_45", backprop_25600_45);

