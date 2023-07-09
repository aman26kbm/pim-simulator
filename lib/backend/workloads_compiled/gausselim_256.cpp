// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t gausselim_256(System *sys) {
  void* _1 = nullptr;
  // float32 0[262144], 0
  // float32 1[261120], 262144
  // float32 241[15360], 523264
  // float32 242[14336], 538624
  // cram-array axis
  {
    int32_t x_inner = 0;
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /*in[ramp((x.inner*256), 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /*in[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowLoad);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /*in[ramp(0, 1, 256)]*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17328), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /*in[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17328), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner1 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner2 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner3 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner4 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner5 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner6 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner7 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner8 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner9 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner10 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner11 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner12 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner13 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner14 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner15 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner16 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner17 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner18 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner19 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner20 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner21 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner22 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner23 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner24 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner25 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner26 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner27 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner28 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner29 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner30 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner31 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner32 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner33 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner34 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner35 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner36 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner37 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner38 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner39 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner40 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner41 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner42 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner43 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner44 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner45 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner46 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner47 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner48 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner49 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner50 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner51 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner52 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner53 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner54 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner55 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner56 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner57 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner58 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner59 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner60 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner61 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner62 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner63 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner64 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner65 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner66 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner67 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner68 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner69 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner70 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner71 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner72 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner73 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner74 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner75 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner76 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner77 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner78 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner79 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner80 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner81 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner82 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner83 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner84 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner85 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner86 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner87 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner88 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner89 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner90 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner91 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner92 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner93 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner94 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner95 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner96 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner97 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner98 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner99 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner100 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner101 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner102 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner103 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner104 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner105 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner106 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner107 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner108 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner109 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner110 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner111 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner112 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner113 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner114 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner115 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner116 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner117 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner118 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner119 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner120 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner121 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner122 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner123 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner124 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner125 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner126 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner127 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner128 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner129 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner130 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner131 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner132 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner133 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner134 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner135 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner136 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner137 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner138 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner139 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner140 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner141 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner142 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner143 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner144 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner145 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner146 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner147 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner148 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner149 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner150 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner151 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner152 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner153 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner154 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner155 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner156 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner157 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner158 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner159 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner160 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner161 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner162 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner163 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner164 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner165 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner166 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner167 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner168 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner169 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner170 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner171 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner172 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner173 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner174 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner175 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner176 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner177 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner178 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner179 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner180 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner181 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner182 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner183 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner184 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner185 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner186 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner187 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner188 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner189 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner190 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner191 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner192 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner193 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner194 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner195 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner196 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner197 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner198 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner199 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner200 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner201 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner202 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner203 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner204 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner205 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner206 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner207 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner208 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner209 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner210 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner211 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner212 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner213 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner214 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner215 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner216 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner217 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner218 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner219 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner220 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner221 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner222 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner223 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner224 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner225 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner226 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner227 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner228 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner229 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner230 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner231 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner232 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner233 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner234 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner235 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner236 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner237 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner238 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner239 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner240 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 8192), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner241 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner242 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /*241[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /*241[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner243 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /*242[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /*242[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner244 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /*241[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /*241[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner245 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /*242[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /*242[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner246 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /*241[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /*241[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner247 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /*242[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /*242[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner248 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /*241[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /*241[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner249 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /*242[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /*242[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner250 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /*241[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /*241[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner251 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /*242[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /*242[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner252 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /*241[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /*241[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner253 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /*242[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /*242[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner254 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /*241[ramp((x.inner*256), 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 16352), 0, PrecisionT::Precision{1, 23, 8} /*241[ramp(0, 1, 256)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  {
    Request request(Request::Type::RowMul_CRAM_RF);
    request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
    request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /*242[ramp(0, 1, 256)]*/);
    request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
    sys->sendRequest(request);
  }
  {
    Request request(Request::Type::RowMul_CRAM_RF);
    request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
    request.addOperand(sys->getAddress(0, 0, 16832), 0, PrecisionT::Precision{1, 23, 8} /*242[ramp(0, 1, 256)]*/);
    request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
    sys->sendRequest(request);
  }
  {
    Request request(Request::Type::RowAdd);
    request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
    request.addOperand(sys->getAddress(0, 0, 17280), 0, PrecisionT::Precision{1, 23, 8} /**/);
    request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /**/);
    sys->sendRequest(request);
  }
  {
    Request request(Request::Type::RowStore);
    request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
    request.addOperand(sys->getAddress(0, 0, 17312), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
    sys->sendRequest(request);
  }
  // freed 242
  // freed 241
  // freed 1
  // freed 0
  return 0;
}

static __attribute__((unused)) Registry::Entry &_gausselim_256__ = pimsim::registerFunc("gausselim_256", gausselim_256);

