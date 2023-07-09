// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t gausselim_512(System *sys) {
  void* _1 = nullptr;
  // float32 0[1048576], 0
  // float32 1[1046528], 1048576
  // float32 481[63488], 2095104
  // float32 482[61440], 2158592
  for (int32_t x_outer = 0; x_outer < 2; ++x_outer) {
    // cram-array axis
    {
      int32_t x_inner = 0;
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
        request.addOperand(sys->getAddress(x_outer, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /*in[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(x_outer, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /*in[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
        request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowLoad);
        request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
        request.addOperand(sys->getAddress(x_outer, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /*in[ramp(0, 1, 512)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowMul_CRAM_RF);
        request.addOperand(sys->getAddress(x_outer, 0, 69472), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /*in[ramp(0, 1, 512)]*/);
        request.addOperand(x_outer * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x_outer, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
        request.addOperand(sys->getAddress(x_outer, 0, 69472), 0, PrecisionT::Precision{1, 23, 8} /**/);
        sys->sendRequest(request);
      }
    }
  }
  for (int32_t x_outer1 = 0; x_outer1 < 2; ++x_outer1) {
    // cram-array axis
    {
      int32_t x_inner1 = 0;
      int32_t _2 = x_outer1 * 256;
      int32_t _3 = _2 + x_inner1;
      bool _4 = _3 < 511;
      if (_4) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer1, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer1, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer1 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer1, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer1, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer1 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer1, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer1, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer1, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer2 = 0; x_outer2 < 2; ++x_outer2) {
    // cram-array axis
    {
      int32_t x_inner2 = 0;
      int32_t _5 = x_outer2 * 256;
      int32_t _6 = _5 + x_inner2;
      bool _7 = _6 < 510;
      if (_7) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer2, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer2, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer2 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer2, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer2, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer2 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer2, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer2, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer2, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer3 = 0; x_outer3 < 2; ++x_outer3) {
    // cram-array axis
    {
      int32_t x_inner3 = 0;
      int32_t _8 = x_outer3 * 256;
      int32_t _9 = _8 + x_inner3;
      bool _10 = _9 < 509;
      if (_10) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer3, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer3, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer3 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer3, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer3, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer3 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer3, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer3, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer3, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer4 = 0; x_outer4 < 2; ++x_outer4) {
    // cram-array axis
    {
      int32_t x_inner4 = 0;
      int32_t _11 = x_outer4 * 256;
      int32_t _12 = _11 + x_inner4;
      bool _13 = _12 < 508;
      if (_13) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer4, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer4, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer4 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer4, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer4, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer4 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer4, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer4, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer4, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer5 = 0; x_outer5 < 2; ++x_outer5) {
    // cram-array axis
    {
      int32_t x_inner5 = 0;
      int32_t _14 = x_outer5 * 256;
      int32_t _15 = _14 + x_inner5;
      bool _16 = _15 < 507;
      if (_16) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer5, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer5, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer5 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer5, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer5, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer5 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer5, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer5, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer5, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer6 = 0; x_outer6 < 2; ++x_outer6) {
    // cram-array axis
    {
      int32_t x_inner6 = 0;
      int32_t _17 = x_outer6 * 256;
      int32_t _18 = _17 + x_inner6;
      bool _19 = _18 < 506;
      if (_19) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer6, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer6, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer6 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer6, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer6, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer6 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer6, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer6, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer6, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer7 = 0; x_outer7 < 2; ++x_outer7) {
    // cram-array axis
    {
      int32_t x_inner7 = 0;
      int32_t _20 = x_outer7 * 256;
      int32_t _21 = _20 + x_inner7;
      bool _22 = _21 < 505;
      if (_22) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer7, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer7, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer7 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer7, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer7, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer7 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer7, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer7, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer7, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer8 = 0; x_outer8 < 2; ++x_outer8) {
    // cram-array axis
    {
      int32_t x_inner8 = 0;
      int32_t _23 = x_outer8 * 256;
      int32_t _24 = _23 + x_inner8;
      bool _25 = _24 < 504;
      if (_25) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer8, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer8, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer8 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer8, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer8, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer8 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer8, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer8, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer8, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer9 = 0; x_outer9 < 2; ++x_outer9) {
    // cram-array axis
    {
      int32_t x_inner9 = 0;
      int32_t _26 = x_outer9 * 256;
      int32_t _27 = _26 + x_inner9;
      bool _28 = _27 < 503;
      if (_28) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer9, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer9, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer9 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer9, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer9, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer9 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer9, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer9, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer9, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer10 = 0; x_outer10 < 2; ++x_outer10) {
    // cram-array axis
    {
      int32_t x_inner10 = 0;
      int32_t _29 = x_outer10 * 256;
      int32_t _30 = _29 + x_inner10;
      bool _31 = _30 < 502;
      if (_31) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer10, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer10, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer10 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer10, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer10, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer10 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer10, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer10, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer10, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer11 = 0; x_outer11 < 2; ++x_outer11) {
    // cram-array axis
    {
      int32_t x_inner11 = 0;
      int32_t _32 = x_outer11 * 256;
      int32_t _33 = _32 + x_inner11;
      bool _34 = _33 < 501;
      if (_34) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer11, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer11, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer11 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer11, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer11, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer11 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer11, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer11, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer11, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer12 = 0; x_outer12 < 2; ++x_outer12) {
    // cram-array axis
    {
      int32_t x_inner12 = 0;
      int32_t _35 = x_outer12 * 256;
      int32_t _36 = _35 + x_inner12;
      bool _37 = _36 < 500;
      if (_37) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer12, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer12, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer12 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer12, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer12, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer12 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer12, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer12, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer12, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer13 = 0; x_outer13 < 2; ++x_outer13) {
    // cram-array axis
    {
      int32_t x_inner13 = 0;
      int32_t _38 = x_outer13 * 256;
      int32_t _39 = _38 + x_inner13;
      bool _40 = _39 < 499;
      if (_40) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer13, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer13, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer13 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer13, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer13, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer13 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer13, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer13, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer13, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer14 = 0; x_outer14 < 2; ++x_outer14) {
    // cram-array axis
    {
      int32_t x_inner14 = 0;
      int32_t _41 = x_outer14 * 256;
      int32_t _42 = _41 + x_inner14;
      bool _43 = _42 < 498;
      if (_43) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer14, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer14, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer14 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer14, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer14, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer14 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer14, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer14, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer14, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer15 = 0; x_outer15 < 2; ++x_outer15) {
    // cram-array axis
    {
      int32_t x_inner15 = 0;
      int32_t _44 = x_outer15 * 256;
      int32_t _45 = _44 + x_inner15;
      bool _46 = _45 < 497;
      if (_46) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer15, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer15, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer15 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer15, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer15, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer15 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer15, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer15, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer15, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer16 = 0; x_outer16 < 2; ++x_outer16) {
    // cram-array axis
    {
      int32_t x_inner16 = 0;
      int32_t _47 = x_outer16 * 256;
      int32_t _48 = _47 + x_inner16;
      bool _49 = _48 < 496;
      if (_49) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer16, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer16, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer16 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer16, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer16, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer16 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer16, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer16, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer16, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer17 = 0; x_outer17 < 2; ++x_outer17) {
    // cram-array axis
    {
      int32_t x_inner17 = 0;
      int32_t _50 = x_outer17 * 256;
      int32_t _51 = _50 + x_inner17;
      bool _52 = _51 < 495;
      if (_52) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer17, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer17, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer17 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer17, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer17, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer17 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer17, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer17, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer17, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer18 = 0; x_outer18 < 2; ++x_outer18) {
    // cram-array axis
    {
      int32_t x_inner18 = 0;
      int32_t _53 = x_outer18 * 256;
      int32_t _54 = _53 + x_inner18;
      bool _55 = _54 < 494;
      if (_55) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer18, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer18, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer18 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer18, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer18, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer18 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer18, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer18, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer18, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer19 = 0; x_outer19 < 2; ++x_outer19) {
    // cram-array axis
    {
      int32_t x_inner19 = 0;
      int32_t _56 = x_outer19 * 256;
      int32_t _57 = _56 + x_inner19;
      bool _58 = _57 < 493;
      if (_58) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer19, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer19, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer19 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer19, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer19, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer19 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer19, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer19, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer19, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer20 = 0; x_outer20 < 2; ++x_outer20) {
    // cram-array axis
    {
      int32_t x_inner20 = 0;
      int32_t _59 = x_outer20 * 256;
      int32_t _60 = _59 + x_inner20;
      bool _61 = _60 < 492;
      if (_61) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer20, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer20, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer20 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer20, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer20, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer20 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer20, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer20, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer20, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer21 = 0; x_outer21 < 2; ++x_outer21) {
    // cram-array axis
    {
      int32_t x_inner21 = 0;
      int32_t _62 = x_outer21 * 256;
      int32_t _63 = _62 + x_inner21;
      bool _64 = _63 < 491;
      if (_64) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer21, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer21, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer21 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer21, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer21, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer21 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer21, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer21, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer21, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer22 = 0; x_outer22 < 2; ++x_outer22) {
    // cram-array axis
    {
      int32_t x_inner22 = 0;
      int32_t _65 = x_outer22 * 256;
      int32_t _66 = _65 + x_inner22;
      bool _67 = _66 < 490;
      if (_67) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer22, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer22, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer22 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer22, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer22, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer22 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer22, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer22, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer22, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer23 = 0; x_outer23 < 2; ++x_outer23) {
    // cram-array axis
    {
      int32_t x_inner23 = 0;
      int32_t _68 = x_outer23 * 256;
      int32_t _69 = _68 + x_inner23;
      bool _70 = _69 < 489;
      if (_70) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer23, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer23, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer23 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer23, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer23, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer23 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer23, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer23, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer23, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer24 = 0; x_outer24 < 2; ++x_outer24) {
    // cram-array axis
    {
      int32_t x_inner24 = 0;
      int32_t _71 = x_outer24 * 256;
      int32_t _72 = _71 + x_inner24;
      bool _73 = _72 < 488;
      if (_73) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer24, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer24, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer24 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer24, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer24, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer24 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer24, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer24, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer24, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer25 = 0; x_outer25 < 2; ++x_outer25) {
    // cram-array axis
    {
      int32_t x_inner25 = 0;
      int32_t _74 = x_outer25 * 256;
      int32_t _75 = _74 + x_inner25;
      bool _76 = _75 < 487;
      if (_76) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer25, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer25, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer25 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer25, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer25, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer25 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer25, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer25, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer25, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer26 = 0; x_outer26 < 2; ++x_outer26) {
    // cram-array axis
    {
      int32_t x_inner26 = 0;
      int32_t _77 = x_outer26 * 256;
      int32_t _78 = _77 + x_inner26;
      bool _79 = _78 < 486;
      if (_79) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer26, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer26, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer26 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer26, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer26, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer26 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer26, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer26, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer26, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer27 = 0; x_outer27 < 2; ++x_outer27) {
    // cram-array axis
    {
      int32_t x_inner27 = 0;
      int32_t _80 = x_outer27 * 256;
      int32_t _81 = _80 + x_inner27;
      bool _82 = _81 < 485;
      if (_82) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer27, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer27, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer27 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer27, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer27, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer27 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer27, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer27, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer27, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer28 = 0; x_outer28 < 2; ++x_outer28) {
    // cram-array axis
    {
      int32_t x_inner28 = 0;
      int32_t _83 = x_outer28 * 256;
      int32_t _84 = _83 + x_inner28;
      bool _85 = _84 < 484;
      if (_85) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer28, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer28, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer28 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer28, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer28, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer28 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer28, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer28, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer28, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer29 = 0; x_outer29 < 2; ++x_outer29) {
    // cram-array axis
    {
      int32_t x_inner29 = 0;
      int32_t _86 = x_outer29 * 256;
      int32_t _87 = _86 + x_inner29;
      bool _88 = _87 < 483;
      if (_88) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer29, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer29, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer29 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer29, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer29, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer29 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer29, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer29, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer29, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer30 = 0; x_outer30 < 2; ++x_outer30) {
    // cram-array axis
    {
      int32_t x_inner30 = 0;
      int32_t _89 = x_outer30 * 256;
      int32_t _90 = _89 + x_inner30;
      bool _91 = _90 < 482;
      if (_91) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer30, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer30, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer30 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer30, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer30, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer30 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer30, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer30, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer30, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer31 = 0; x_outer31 < 2; ++x_outer31) {
    // cram-array axis
    {
      int32_t x_inner31 = 0;
      int32_t _92 = x_outer31 * 256;
      int32_t _93 = _92 + x_inner31;
      bool _94 = _93 < 481;
      if (_94) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer31, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer31, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer31 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer31, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer31, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer31 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer31, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer31, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer31, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer32 = 0; x_outer32 < 2; ++x_outer32) {
    // cram-array axis
    {
      int32_t x_inner32 = 0;
      int32_t _95 = x_outer32 * 256;
      int32_t _96 = _95 + x_inner32;
      bool _97 = _96 < 480;
      if (_97) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer32, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer32, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer32 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer32, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer32, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer32 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer32, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer32, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer32, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer33 = 0; x_outer33 < 2; ++x_outer33) {
    // cram-array axis
    {
      int32_t x_inner33 = 0;
      int32_t _98 = x_outer33 * 256;
      int32_t _99 = _98 + x_inner33;
      bool _100 = _99 < 479;
      if (_100) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer33, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer33, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer33 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer33, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer33, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer33 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer33, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer33, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer33, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer34 = 0; x_outer34 < 2; ++x_outer34) {
    // cram-array axis
    {
      int32_t x_inner34 = 0;
      int32_t _101 = x_outer34 * 256;
      int32_t _102 = _101 + x_inner34;
      bool _103 = _102 < 478;
      if (_103) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer34, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer34, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer34 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer34, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer34, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer34 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer34, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer34, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer34, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer35 = 0; x_outer35 < 2; ++x_outer35) {
    // cram-array axis
    {
      int32_t x_inner35 = 0;
      int32_t _104 = x_outer35 * 256;
      int32_t _105 = _104 + x_inner35;
      bool _106 = _105 < 477;
      if (_106) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer35, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer35, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer35 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer35, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer35, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer35 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer35, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer35, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer35, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer36 = 0; x_outer36 < 2; ++x_outer36) {
    // cram-array axis
    {
      int32_t x_inner36 = 0;
      int32_t _107 = x_outer36 * 256;
      int32_t _108 = _107 + x_inner36;
      bool _109 = _108 < 476;
      if (_109) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer36, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer36, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer36 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer36, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer36, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer36 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer36, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer36, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer36, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer37 = 0; x_outer37 < 2; ++x_outer37) {
    // cram-array axis
    {
      int32_t x_inner37 = 0;
      int32_t _110 = x_outer37 * 256;
      int32_t _111 = _110 + x_inner37;
      bool _112 = _111 < 475;
      if (_112) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer37, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer37, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer37 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer37, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer37, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer37 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer37, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer37, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer37, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer38 = 0; x_outer38 < 2; ++x_outer38) {
    // cram-array axis
    {
      int32_t x_inner38 = 0;
      int32_t _113 = x_outer38 * 256;
      int32_t _114 = _113 + x_inner38;
      bool _115 = _114 < 474;
      if (_115) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer38, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer38, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer38 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer38, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer38, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer38 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer38, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer38, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer38, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer39 = 0; x_outer39 < 2; ++x_outer39) {
    // cram-array axis
    {
      int32_t x_inner39 = 0;
      int32_t _116 = x_outer39 * 256;
      int32_t _117 = _116 + x_inner39;
      bool _118 = _117 < 473;
      if (_118) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer39, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer39, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer39 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer39, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer39, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer39 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer39, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer39, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer39, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer40 = 0; x_outer40 < 2; ++x_outer40) {
    // cram-array axis
    {
      int32_t x_inner40 = 0;
      int32_t _119 = x_outer40 * 256;
      int32_t _120 = _119 + x_inner40;
      bool _121 = _120 < 472;
      if (_121) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer40, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer40, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer40 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer40, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer40, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer40 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer40, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer40, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer40, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer41 = 0; x_outer41 < 2; ++x_outer41) {
    // cram-array axis
    {
      int32_t x_inner41 = 0;
      int32_t _122 = x_outer41 * 256;
      int32_t _123 = _122 + x_inner41;
      bool _124 = _123 < 471;
      if (_124) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer41, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer41, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer41 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer41, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer41, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer41 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer41, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer41, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer41, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer42 = 0; x_outer42 < 2; ++x_outer42) {
    // cram-array axis
    {
      int32_t x_inner42 = 0;
      int32_t _125 = x_outer42 * 256;
      int32_t _126 = _125 + x_inner42;
      bool _127 = _126 < 470;
      if (_127) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer42, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer42, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer42 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer42, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer42, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer42 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer42, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer42, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer42, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer43 = 0; x_outer43 < 2; ++x_outer43) {
    // cram-array axis
    {
      int32_t x_inner43 = 0;
      int32_t _128 = x_outer43 * 256;
      int32_t _129 = _128 + x_inner43;
      bool _130 = _129 < 469;
      if (_130) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer43, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer43, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer43 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer43, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer43, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer43 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer43, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer43, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer43, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer44 = 0; x_outer44 < 2; ++x_outer44) {
    // cram-array axis
    {
      int32_t x_inner44 = 0;
      int32_t _131 = x_outer44 * 256;
      int32_t _132 = _131 + x_inner44;
      bool _133 = _132 < 468;
      if (_133) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer44, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer44, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer44 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer44, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer44, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer44 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer44, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer44, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer44, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer45 = 0; x_outer45 < 2; ++x_outer45) {
    // cram-array axis
    {
      int32_t x_inner45 = 0;
      int32_t _134 = x_outer45 * 256;
      int32_t _135 = _134 + x_inner45;
      bool _136 = _135 < 467;
      if (_136) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer45, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer45, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer45 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer45, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer45, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer45 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer45, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer45, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer45, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer46 = 0; x_outer46 < 2; ++x_outer46) {
    // cram-array axis
    {
      int32_t x_inner46 = 0;
      int32_t _137 = x_outer46 * 256;
      int32_t _138 = _137 + x_inner46;
      bool _139 = _138 < 466;
      if (_139) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer46, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer46, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer46 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer46, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer46, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer46 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer46, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer46, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer46, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer47 = 0; x_outer47 < 2; ++x_outer47) {
    // cram-array axis
    {
      int32_t x_inner47 = 0;
      int32_t _140 = x_outer47 * 256;
      int32_t _141 = _140 + x_inner47;
      bool _142 = _141 < 465;
      if (_142) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer47, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer47, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer47 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer47, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer47, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer47 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer47, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer47, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer47, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer48 = 0; x_outer48 < 2; ++x_outer48) {
    // cram-array axis
    {
      int32_t x_inner48 = 0;
      int32_t _143 = x_outer48 * 256;
      int32_t _144 = _143 + x_inner48;
      bool _145 = _144 < 464;
      if (_145) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer48, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer48, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer48 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer48, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer48, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer48 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer48, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer48, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer48, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer49 = 0; x_outer49 < 2; ++x_outer49) {
    // cram-array axis
    {
      int32_t x_inner49 = 0;
      int32_t _146 = x_outer49 * 256;
      int32_t _147 = _146 + x_inner49;
      bool _148 = _147 < 463;
      if (_148) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer49, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer49, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer49 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer49, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer49, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer49 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer49, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer49, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer49, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer50 = 0; x_outer50 < 2; ++x_outer50) {
    // cram-array axis
    {
      int32_t x_inner50 = 0;
      int32_t _149 = x_outer50 * 256;
      int32_t _150 = _149 + x_inner50;
      bool _151 = _150 < 462;
      if (_151) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer50, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer50, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer50 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer50, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer50, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer50 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer50, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer50, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer50, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer51 = 0; x_outer51 < 2; ++x_outer51) {
    // cram-array axis
    {
      int32_t x_inner51 = 0;
      int32_t _152 = x_outer51 * 256;
      int32_t _153 = _152 + x_inner51;
      bool _154 = _153 < 461;
      if (_154) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer51, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer51, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer51 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer51, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer51, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer51 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer51, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer51, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer51, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer52 = 0; x_outer52 < 2; ++x_outer52) {
    // cram-array axis
    {
      int32_t x_inner52 = 0;
      int32_t _155 = x_outer52 * 256;
      int32_t _156 = _155 + x_inner52;
      bool _157 = _156 < 460;
      if (_157) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer52, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer52, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer52 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer52, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer52, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer52 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer52, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer52, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer52, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer53 = 0; x_outer53 < 2; ++x_outer53) {
    // cram-array axis
    {
      int32_t x_inner53 = 0;
      int32_t _158 = x_outer53 * 256;
      int32_t _159 = _158 + x_inner53;
      bool _160 = _159 < 459;
      if (_160) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer53, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer53, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer53 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer53, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer53, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer53 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer53, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer53, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer53, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer54 = 0; x_outer54 < 2; ++x_outer54) {
    // cram-array axis
    {
      int32_t x_inner54 = 0;
      int32_t _161 = x_outer54 * 256;
      int32_t _162 = _161 + x_inner54;
      bool _163 = _162 < 458;
      if (_163) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer54, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer54, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer54 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer54, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer54, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer54 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer54, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer54, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer54, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer55 = 0; x_outer55 < 2; ++x_outer55) {
    // cram-array axis
    {
      int32_t x_inner55 = 0;
      int32_t _164 = x_outer55 * 256;
      int32_t _165 = _164 + x_inner55;
      bool _166 = _165 < 457;
      if (_166) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer55, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer55, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer55 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer55, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer55, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer55 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer55, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer55, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer55, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer56 = 0; x_outer56 < 2; ++x_outer56) {
    // cram-array axis
    {
      int32_t x_inner56 = 0;
      int32_t _167 = x_outer56 * 256;
      int32_t _168 = _167 + x_inner56;
      bool _169 = _168 < 456;
      if (_169) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer56, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer56, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer56 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer56, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer56, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer56 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer56, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer56, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer56, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer57 = 0; x_outer57 < 2; ++x_outer57) {
    // cram-array axis
    {
      int32_t x_inner57 = 0;
      int32_t _170 = x_outer57 * 256;
      int32_t _171 = _170 + x_inner57;
      bool _172 = _171 < 455;
      if (_172) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer57, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer57, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer57 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer57, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer57, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer57 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer57, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer57, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer57, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer58 = 0; x_outer58 < 2; ++x_outer58) {
    // cram-array axis
    {
      int32_t x_inner58 = 0;
      int32_t _173 = x_outer58 * 256;
      int32_t _174 = _173 + x_inner58;
      bool _175 = _174 < 454;
      if (_175) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer58, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer58, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer58 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer58, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer58, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer58 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer58, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer58, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer58, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer59 = 0; x_outer59 < 2; ++x_outer59) {
    // cram-array axis
    {
      int32_t x_inner59 = 0;
      int32_t _176 = x_outer59 * 256;
      int32_t _177 = _176 + x_inner59;
      bool _178 = _177 < 453;
      if (_178) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer59, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer59, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer59 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer59, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer59, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer59 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer59, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer59, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer59, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer60 = 0; x_outer60 < 2; ++x_outer60) {
    // cram-array axis
    {
      int32_t x_inner60 = 0;
      int32_t _179 = x_outer60 * 256;
      int32_t _180 = _179 + x_inner60;
      bool _181 = _180 < 452;
      if (_181) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer60, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer60, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer60 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer60, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer60, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer60 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer60, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer60, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer60, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer61 = 0; x_outer61 < 2; ++x_outer61) {
    // cram-array axis
    {
      int32_t x_inner61 = 0;
      int32_t _182 = x_outer61 * 256;
      int32_t _183 = _182 + x_inner61;
      bool _184 = _183 < 451;
      if (_184) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer61, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer61, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer61 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer61, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer61, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer61 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer61, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer61, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer61, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer62 = 0; x_outer62 < 2; ++x_outer62) {
    // cram-array axis
    {
      int32_t x_inner62 = 0;
      int32_t _185 = x_outer62 * 256;
      int32_t _186 = _185 + x_inner62;
      bool _187 = _186 < 450;
      if (_187) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer62, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer62, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer62 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer62, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer62, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer62 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer62, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer62, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer62, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer63 = 0; x_outer63 < 2; ++x_outer63) {
    // cram-array axis
    {
      int32_t x_inner63 = 0;
      int32_t _188 = x_outer63 * 256;
      int32_t _189 = _188 + x_inner63;
      bool _190 = _189 < 449;
      if (_190) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer63, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer63, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer63 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer63, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer63, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer63 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer63, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer63, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer63, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer64 = 0; x_outer64 < 2; ++x_outer64) {
    // cram-array axis
    {
      int32_t x_inner64 = 0;
      int32_t _191 = x_outer64 * 256;
      int32_t _192 = _191 + x_inner64;
      bool _193 = _192 < 448;
      if (_193) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer64, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer64, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer64 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer64, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer64, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer64 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer64, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer64, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer64, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer65 = 0; x_outer65 < 2; ++x_outer65) {
    // cram-array axis
    {
      int32_t x_inner65 = 0;
      int32_t _194 = x_outer65 * 256;
      int32_t _195 = _194 + x_inner65;
      bool _196 = _195 < 447;
      if (_196) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer65, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer65, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer65 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer65, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer65, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer65 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer65, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer65, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer65, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer66 = 0; x_outer66 < 2; ++x_outer66) {
    // cram-array axis
    {
      int32_t x_inner66 = 0;
      int32_t _197 = x_outer66 * 256;
      int32_t _198 = _197 + x_inner66;
      bool _199 = _198 < 446;
      if (_199) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer66, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer66, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer66 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer66, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer66, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer66 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer66, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer66, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer66, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer67 = 0; x_outer67 < 2; ++x_outer67) {
    // cram-array axis
    {
      int32_t x_inner67 = 0;
      int32_t _200 = x_outer67 * 256;
      int32_t _201 = _200 + x_inner67;
      bool _202 = _201 < 445;
      if (_202) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer67, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer67, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer67 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer67, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer67, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer67 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer67, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer67, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer67, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer68 = 0; x_outer68 < 2; ++x_outer68) {
    // cram-array axis
    {
      int32_t x_inner68 = 0;
      int32_t _203 = x_outer68 * 256;
      int32_t _204 = _203 + x_inner68;
      bool _205 = _204 < 444;
      if (_205) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer68, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer68, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer68 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer68, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer68, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer68 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer68, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer68, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer68, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer69 = 0; x_outer69 < 2; ++x_outer69) {
    // cram-array axis
    {
      int32_t x_inner69 = 0;
      int32_t _206 = x_outer69 * 256;
      int32_t _207 = _206 + x_inner69;
      bool _208 = _207 < 443;
      if (_208) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer69, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer69, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer69 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer69, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer69, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer69 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer69, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer69, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer69, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer70 = 0; x_outer70 < 2; ++x_outer70) {
    // cram-array axis
    {
      int32_t x_inner70 = 0;
      int32_t _209 = x_outer70 * 256;
      int32_t _210 = _209 + x_inner70;
      bool _211 = _210 < 442;
      if (_211) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer70, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer70, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer70 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer70, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer70, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer70 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer70, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer70, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer70, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer71 = 0; x_outer71 < 2; ++x_outer71) {
    // cram-array axis
    {
      int32_t x_inner71 = 0;
      int32_t _212 = x_outer71 * 256;
      int32_t _213 = _212 + x_inner71;
      bool _214 = _213 < 441;
      if (_214) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer71, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer71, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer71 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer71, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer71, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer71 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer71, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer71, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer71, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer72 = 0; x_outer72 < 2; ++x_outer72) {
    // cram-array axis
    {
      int32_t x_inner72 = 0;
      int32_t _215 = x_outer72 * 256;
      int32_t _216 = _215 + x_inner72;
      bool _217 = _216 < 440;
      if (_217) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer72, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer72, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer72 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer72, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer72, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer72 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer72, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer72, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer72, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer73 = 0; x_outer73 < 2; ++x_outer73) {
    // cram-array axis
    {
      int32_t x_inner73 = 0;
      int32_t _218 = x_outer73 * 256;
      int32_t _219 = _218 + x_inner73;
      bool _220 = _219 < 439;
      if (_220) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer73, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer73, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer73 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer73, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer73, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer73 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer73, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer73, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer73, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer74 = 0; x_outer74 < 2; ++x_outer74) {
    // cram-array axis
    {
      int32_t x_inner74 = 0;
      int32_t _221 = x_outer74 * 256;
      int32_t _222 = _221 + x_inner74;
      bool _223 = _222 < 438;
      if (_223) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer74, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer74, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer74 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer74, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer74, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer74 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer74, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer74, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer74, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer75 = 0; x_outer75 < 2; ++x_outer75) {
    // cram-array axis
    {
      int32_t x_inner75 = 0;
      int32_t _224 = x_outer75 * 256;
      int32_t _225 = _224 + x_inner75;
      bool _226 = _225 < 437;
      if (_226) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer75, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer75, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer75 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer75, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer75, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer75 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer75, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer75, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer75, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer76 = 0; x_outer76 < 2; ++x_outer76) {
    // cram-array axis
    {
      int32_t x_inner76 = 0;
      int32_t _227 = x_outer76 * 256;
      int32_t _228 = _227 + x_inner76;
      bool _229 = _228 < 436;
      if (_229) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer76, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer76, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer76 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer76, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer76, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer76 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer76, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer76, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer76, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer77 = 0; x_outer77 < 2; ++x_outer77) {
    // cram-array axis
    {
      int32_t x_inner77 = 0;
      int32_t _230 = x_outer77 * 256;
      int32_t _231 = _230 + x_inner77;
      bool _232 = _231 < 435;
      if (_232) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer77, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer77, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer77 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer77, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer77, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer77 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer77, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer77, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer77, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer78 = 0; x_outer78 < 2; ++x_outer78) {
    // cram-array axis
    {
      int32_t x_inner78 = 0;
      int32_t _233 = x_outer78 * 256;
      int32_t _234 = _233 + x_inner78;
      bool _235 = _234 < 434;
      if (_235) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer78, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer78, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer78 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer78, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer78, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer78 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer78, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer78, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer78, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer79 = 0; x_outer79 < 2; ++x_outer79) {
    // cram-array axis
    {
      int32_t x_inner79 = 0;
      int32_t _236 = x_outer79 * 256;
      int32_t _237 = _236 + x_inner79;
      bool _238 = _237 < 433;
      if (_238) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer79, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer79, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer79 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer79, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer79, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer79 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer79, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer79, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer79, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer80 = 0; x_outer80 < 2; ++x_outer80) {
    // cram-array axis
    {
      int32_t x_inner80 = 0;
      int32_t _239 = x_outer80 * 256;
      int32_t _240 = _239 + x_inner80;
      bool _241 = _240 < 432;
      if (_241) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer80, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer80, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer80 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer80, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer80, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer80 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer80, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer80, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer80, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer81 = 0; x_outer81 < 2; ++x_outer81) {
    // cram-array axis
    {
      int32_t x_inner81 = 0;
      int32_t _242 = x_outer81 * 256;
      int32_t _243 = _242 + x_inner81;
      bool _244 = _243 < 431;
      if (_244) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer81, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer81, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer81 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer81, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer81, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer81 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer81, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer81, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer81, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer82 = 0; x_outer82 < 2; ++x_outer82) {
    // cram-array axis
    {
      int32_t x_inner82 = 0;
      int32_t _245 = x_outer82 * 256;
      int32_t _246 = _245 + x_inner82;
      bool _247 = _246 < 430;
      if (_247) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer82, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer82, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer82 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer82, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer82, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer82 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer82, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer82, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer82, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer83 = 0; x_outer83 < 2; ++x_outer83) {
    // cram-array axis
    {
      int32_t x_inner83 = 0;
      int32_t _248 = x_outer83 * 256;
      int32_t _249 = _248 + x_inner83;
      bool _250 = _249 < 429;
      if (_250) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer83, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer83, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer83 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer83, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer83, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer83 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer83, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer83, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer83, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer84 = 0; x_outer84 < 2; ++x_outer84) {
    // cram-array axis
    {
      int32_t x_inner84 = 0;
      int32_t _251 = x_outer84 * 256;
      int32_t _252 = _251 + x_inner84;
      bool _253 = _252 < 428;
      if (_253) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer84, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer84, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer84 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer84, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer84, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer84 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer84, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer84, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer84, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer85 = 0; x_outer85 < 2; ++x_outer85) {
    // cram-array axis
    {
      int32_t x_inner85 = 0;
      int32_t _254 = x_outer85 * 256;
      int32_t _255 = _254 + x_inner85;
      bool _256 = _255 < 427;
      if (_256) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer85, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer85, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer85 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer85, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer85, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer85 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer85, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer85, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer85, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer86 = 0; x_outer86 < 2; ++x_outer86) {
    // cram-array axis
    {
      int32_t x_inner86 = 0;
      int32_t _257 = x_outer86 * 256;
      int32_t _258 = _257 + x_inner86;
      bool _259 = _258 < 426;
      if (_259) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer86, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer86, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer86 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer86, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer86, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer86 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer86, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer86, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer86, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer87 = 0; x_outer87 < 2; ++x_outer87) {
    // cram-array axis
    {
      int32_t x_inner87 = 0;
      int32_t _260 = x_outer87 * 256;
      int32_t _261 = _260 + x_inner87;
      bool _262 = _261 < 425;
      if (_262) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer87, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer87, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer87 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer87, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer87, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer87 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer87, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer87, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer87, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer88 = 0; x_outer88 < 2; ++x_outer88) {
    // cram-array axis
    {
      int32_t x_inner88 = 0;
      int32_t _263 = x_outer88 * 256;
      int32_t _264 = _263 + x_inner88;
      bool _265 = _264 < 424;
      if (_265) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer88, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer88, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer88 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer88, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer88, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer88 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer88, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer88, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer88, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer89 = 0; x_outer89 < 2; ++x_outer89) {
    // cram-array axis
    {
      int32_t x_inner89 = 0;
      int32_t _266 = x_outer89 * 256;
      int32_t _267 = _266 + x_inner89;
      bool _268 = _267 < 423;
      if (_268) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer89, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer89, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer89 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer89, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer89, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer89 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer89, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer89, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer89, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer90 = 0; x_outer90 < 2; ++x_outer90) {
    // cram-array axis
    {
      int32_t x_inner90 = 0;
      int32_t _269 = x_outer90 * 256;
      int32_t _270 = _269 + x_inner90;
      bool _271 = _270 < 422;
      if (_271) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer90, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer90, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer90 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer90, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer90, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer90 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer90, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer90, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer90, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer91 = 0; x_outer91 < 2; ++x_outer91) {
    // cram-array axis
    {
      int32_t x_inner91 = 0;
      int32_t _272 = x_outer91 * 256;
      int32_t _273 = _272 + x_inner91;
      bool _274 = _273 < 421;
      if (_274) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer91, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer91, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer91 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer91, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer91, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer91 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer91, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer91, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer91, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer92 = 0; x_outer92 < 2; ++x_outer92) {
    // cram-array axis
    {
      int32_t x_inner92 = 0;
      int32_t _275 = x_outer92 * 256;
      int32_t _276 = _275 + x_inner92;
      bool _277 = _276 < 420;
      if (_277) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer92, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer92, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer92 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer92, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer92, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer92 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer92, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer92, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer92, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer93 = 0; x_outer93 < 2; ++x_outer93) {
    // cram-array axis
    {
      int32_t x_inner93 = 0;
      int32_t _278 = x_outer93 * 256;
      int32_t _279 = _278 + x_inner93;
      bool _280 = _279 < 419;
      if (_280) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer93, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer93, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer93 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer93, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer93, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer93 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer93, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer93, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer93, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer94 = 0; x_outer94 < 2; ++x_outer94) {
    // cram-array axis
    {
      int32_t x_inner94 = 0;
      int32_t _281 = x_outer94 * 256;
      int32_t _282 = _281 + x_inner94;
      bool _283 = _282 < 418;
      if (_283) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer94, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer94, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer94 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer94, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer94, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer94 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer94, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer94, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer94, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer95 = 0; x_outer95 < 2; ++x_outer95) {
    // cram-array axis
    {
      int32_t x_inner95 = 0;
      int32_t _284 = x_outer95 * 256;
      int32_t _285 = _284 + x_inner95;
      bool _286 = _285 < 417;
      if (_286) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer95, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer95, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer95 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer95, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer95, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer95 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer95, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer95, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer95, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer96 = 0; x_outer96 < 2; ++x_outer96) {
    // cram-array axis
    {
      int32_t x_inner96 = 0;
      int32_t _287 = x_outer96 * 256;
      int32_t _288 = _287 + x_inner96;
      bool _289 = _288 < 416;
      if (_289) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer96, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer96, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer96 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer96, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer96, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer96 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer96, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer96, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer96, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer97 = 0; x_outer97 < 2; ++x_outer97) {
    // cram-array axis
    {
      int32_t x_inner97 = 0;
      int32_t _290 = x_outer97 * 256;
      int32_t _291 = _290 + x_inner97;
      bool _292 = _291 < 415;
      if (_292) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer97, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer97, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer97 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer97, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer97, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer97 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer97, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer97, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer97, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer98 = 0; x_outer98 < 2; ++x_outer98) {
    // cram-array axis
    {
      int32_t x_inner98 = 0;
      int32_t _293 = x_outer98 * 256;
      int32_t _294 = _293 + x_inner98;
      bool _295 = _294 < 414;
      if (_295) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer98, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer98, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer98 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer98, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer98, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer98 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer98, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer98, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer98, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer99 = 0; x_outer99 < 2; ++x_outer99) {
    // cram-array axis
    {
      int32_t x_inner99 = 0;
      int32_t _296 = x_outer99 * 256;
      int32_t _297 = _296 + x_inner99;
      bool _298 = _297 < 413;
      if (_298) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer99, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer99, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer99 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer99, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer99, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer99 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer99, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer99, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer99, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer100 = 0; x_outer100 < 2; ++x_outer100) {
    // cram-array axis
    {
      int32_t x_inner100 = 0;
      int32_t _299 = x_outer100 * 256;
      int32_t _300 = _299 + x_inner100;
      bool _301 = _300 < 412;
      if (_301) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer100, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer100, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer100 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer100, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer100, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer100 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer100, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer100, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer100, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer101 = 0; x_outer101 < 2; ++x_outer101) {
    // cram-array axis
    {
      int32_t x_inner101 = 0;
      int32_t _302 = x_outer101 * 256;
      int32_t _303 = _302 + x_inner101;
      bool _304 = _303 < 411;
      if (_304) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer101, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer101, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer101 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer101, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer101, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer101 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer101, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer101, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer101, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer102 = 0; x_outer102 < 2; ++x_outer102) {
    // cram-array axis
    {
      int32_t x_inner102 = 0;
      int32_t _305 = x_outer102 * 256;
      int32_t _306 = _305 + x_inner102;
      bool _307 = _306 < 410;
      if (_307) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer102, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer102, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer102 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer102, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer102, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer102 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer102, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer102, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer102, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer103 = 0; x_outer103 < 2; ++x_outer103) {
    // cram-array axis
    {
      int32_t x_inner103 = 0;
      int32_t _308 = x_outer103 * 256;
      int32_t _309 = _308 + x_inner103;
      bool _310 = _309 < 409;
      if (_310) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer103, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer103, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer103 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer103, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer103, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer103 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer103, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer103, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer103, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer104 = 0; x_outer104 < 2; ++x_outer104) {
    // cram-array axis
    {
      int32_t x_inner104 = 0;
      int32_t _311 = x_outer104 * 256;
      int32_t _312 = _311 + x_inner104;
      bool _313 = _312 < 408;
      if (_313) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer104, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer104, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer104 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer104, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer104, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer104 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer104, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer104, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer104, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer105 = 0; x_outer105 < 2; ++x_outer105) {
    // cram-array axis
    {
      int32_t x_inner105 = 0;
      int32_t _314 = x_outer105 * 256;
      int32_t _315 = _314 + x_inner105;
      bool _316 = _315 < 407;
      if (_316) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer105, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer105, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer105 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer105, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer105, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer105 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer105, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer105, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer105, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer106 = 0; x_outer106 < 2; ++x_outer106) {
    // cram-array axis
    {
      int32_t x_inner106 = 0;
      int32_t _317 = x_outer106 * 256;
      int32_t _318 = _317 + x_inner106;
      bool _319 = _318 < 406;
      if (_319) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer106, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer106, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer106 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer106, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer106, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer106 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer106, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer106, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer106, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer107 = 0; x_outer107 < 2; ++x_outer107) {
    // cram-array axis
    {
      int32_t x_inner107 = 0;
      int32_t _320 = x_outer107 * 256;
      int32_t _321 = _320 + x_inner107;
      bool _322 = _321 < 405;
      if (_322) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer107, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer107, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer107 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer107, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer107, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer107 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer107, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer107, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer107, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer108 = 0; x_outer108 < 2; ++x_outer108) {
    // cram-array axis
    {
      int32_t x_inner108 = 0;
      int32_t _323 = x_outer108 * 256;
      int32_t _324 = _323 + x_inner108;
      bool _325 = _324 < 404;
      if (_325) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer108, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer108, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer108 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer108, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer108, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer108 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer108, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer108, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer108, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer109 = 0; x_outer109 < 2; ++x_outer109) {
    // cram-array axis
    {
      int32_t x_inner109 = 0;
      int32_t _326 = x_outer109 * 256;
      int32_t _327 = _326 + x_inner109;
      bool _328 = _327 < 403;
      if (_328) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer109, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer109, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer109 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer109, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer109, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer109 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer109, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer109, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer109, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer110 = 0; x_outer110 < 2; ++x_outer110) {
    // cram-array axis
    {
      int32_t x_inner110 = 0;
      int32_t _329 = x_outer110 * 256;
      int32_t _330 = _329 + x_inner110;
      bool _331 = _330 < 402;
      if (_331) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer110, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer110, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer110 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer110, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer110, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer110 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer110, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer110, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer110, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer111 = 0; x_outer111 < 2; ++x_outer111) {
    // cram-array axis
    {
      int32_t x_inner111 = 0;
      int32_t _332 = x_outer111 * 256;
      int32_t _333 = _332 + x_inner111;
      bool _334 = _333 < 401;
      if (_334) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer111, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer111, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer111 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer111, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer111, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer111 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer111, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer111, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer111, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer112 = 0; x_outer112 < 2; ++x_outer112) {
    // cram-array axis
    {
      int32_t x_inner112 = 0;
      int32_t _335 = x_outer112 * 256;
      int32_t _336 = _335 + x_inner112;
      bool _337 = _336 < 400;
      if (_337) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer112, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer112, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer112 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer112, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer112, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer112 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer112, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer112, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer112, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer113 = 0; x_outer113 < 2; ++x_outer113) {
    // cram-array axis
    {
      int32_t x_inner113 = 0;
      int32_t _338 = x_outer113 * 256;
      int32_t _339 = _338 + x_inner113;
      bool _340 = _339 < 399;
      if (_340) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer113, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer113, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer113 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer113, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer113, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer113 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer113, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer113, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer113, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer114 = 0; x_outer114 < 2; ++x_outer114) {
    // cram-array axis
    {
      int32_t x_inner114 = 0;
      int32_t _341 = x_outer114 * 256;
      int32_t _342 = _341 + x_inner114;
      bool _343 = _342 < 398;
      if (_343) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer114, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer114, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer114 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer114, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer114, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer114 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer114, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer114, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer114, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer115 = 0; x_outer115 < 2; ++x_outer115) {
    // cram-array axis
    {
      int32_t x_inner115 = 0;
      int32_t _344 = x_outer115 * 256;
      int32_t _345 = _344 + x_inner115;
      bool _346 = _345 < 397;
      if (_346) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer115, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer115, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer115 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer115, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer115, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer115 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer115, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer115, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer115, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer116 = 0; x_outer116 < 2; ++x_outer116) {
    // cram-array axis
    {
      int32_t x_inner116 = 0;
      int32_t _347 = x_outer116 * 256;
      int32_t _348 = _347 + x_inner116;
      bool _349 = _348 < 396;
      if (_349) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer116, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer116, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer116 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer116, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer116, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer116 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer116, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer116, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer116, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer117 = 0; x_outer117 < 2; ++x_outer117) {
    // cram-array axis
    {
      int32_t x_inner117 = 0;
      int32_t _350 = x_outer117 * 256;
      int32_t _351 = _350 + x_inner117;
      bool _352 = _351 < 395;
      if (_352) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer117, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer117, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer117 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer117, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer117, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer117 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer117, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer117, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer117, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer118 = 0; x_outer118 < 2; ++x_outer118) {
    // cram-array axis
    {
      int32_t x_inner118 = 0;
      int32_t _353 = x_outer118 * 256;
      int32_t _354 = _353 + x_inner118;
      bool _355 = _354 < 394;
      if (_355) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer118, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer118, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer118 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer118, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer118, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer118 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer118, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer118, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer118, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer119 = 0; x_outer119 < 2; ++x_outer119) {
    // cram-array axis
    {
      int32_t x_inner119 = 0;
      int32_t _356 = x_outer119 * 256;
      int32_t _357 = _356 + x_inner119;
      bool _358 = _357 < 393;
      if (_358) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer119, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer119, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer119 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer119, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer119, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer119 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer119, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer119, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer119, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer120 = 0; x_outer120 < 2; ++x_outer120) {
    // cram-array axis
    {
      int32_t x_inner120 = 0;
      int32_t _359 = x_outer120 * 256;
      int32_t _360 = _359 + x_inner120;
      bool _361 = _360 < 392;
      if (_361) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer120, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer120, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer120 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer120, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer120, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer120 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer120, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer120, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer120, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer121 = 0; x_outer121 < 2; ++x_outer121) {
    // cram-array axis
    {
      int32_t x_inner121 = 0;
      int32_t _362 = x_outer121 * 256;
      int32_t _363 = _362 + x_inner121;
      bool _364 = _363 < 391;
      if (_364) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer121, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer121, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer121 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer121, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer121, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer121 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer121, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer121, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer121, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer122 = 0; x_outer122 < 2; ++x_outer122) {
    // cram-array axis
    {
      int32_t x_inner122 = 0;
      int32_t _365 = x_outer122 * 256;
      int32_t _366 = _365 + x_inner122;
      bool _367 = _366 < 390;
      if (_367) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer122, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer122, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer122 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer122, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer122, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer122 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer122, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer122, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer122, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer123 = 0; x_outer123 < 2; ++x_outer123) {
    // cram-array axis
    {
      int32_t x_inner123 = 0;
      int32_t _368 = x_outer123 * 256;
      int32_t _369 = _368 + x_inner123;
      bool _370 = _369 < 389;
      if (_370) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer123, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer123, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer123 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer123, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer123, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer123 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer123, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer123, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer123, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer124 = 0; x_outer124 < 2; ++x_outer124) {
    // cram-array axis
    {
      int32_t x_inner124 = 0;
      int32_t _371 = x_outer124 * 256;
      int32_t _372 = _371 + x_inner124;
      bool _373 = _372 < 388;
      if (_373) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer124, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer124, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer124 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer124, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer124, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer124 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer124, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer124, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer124, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer125 = 0; x_outer125 < 2; ++x_outer125) {
    // cram-array axis
    {
      int32_t x_inner125 = 0;
      int32_t _374 = x_outer125 * 256;
      int32_t _375 = _374 + x_inner125;
      bool _376 = _375 < 387;
      if (_376) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer125, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer125, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer125 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer125, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer125, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer125 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer125, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer125, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer125, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer126 = 0; x_outer126 < 2; ++x_outer126) {
    // cram-array axis
    {
      int32_t x_inner126 = 0;
      int32_t _377 = x_outer126 * 256;
      int32_t _378 = _377 + x_inner126;
      bool _379 = _378 < 386;
      if (_379) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer126, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer126, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer126 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer126, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer126, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer126 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer126, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer126, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer126, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer127 = 0; x_outer127 < 2; ++x_outer127) {
    // cram-array axis
    {
      int32_t x_inner127 = 0;
      int32_t _380 = x_outer127 * 256;
      int32_t _381 = _380 + x_inner127;
      bool _382 = _381 < 385;
      if (_382) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer127, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer127, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer127 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer127, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer127, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer127 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer127, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer127, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer127, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer128 = 0; x_outer128 < 2; ++x_outer128) {
    // cram-array axis
    {
      int32_t x_inner128 = 0;
      int32_t _383 = x_outer128 * 256;
      int32_t _384 = _383 + x_inner128;
      bool _385 = _384 < 384;
      if (_385) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer128, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer128, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer128 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer128, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer128, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer128 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer128, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer128, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer128, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer129 = 0; x_outer129 < 2; ++x_outer129) {
    // cram-array axis
    {
      int32_t x_inner129 = 0;
      int32_t _386 = x_outer129 * 256;
      int32_t _387 = _386 + x_inner129;
      bool _388 = _387 < 383;
      if (_388) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer129, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer129, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer129 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer129, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer129, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer129 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer129, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer129, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer129, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer130 = 0; x_outer130 < 2; ++x_outer130) {
    // cram-array axis
    {
      int32_t x_inner130 = 0;
      int32_t _389 = x_outer130 * 256;
      int32_t _390 = _389 + x_inner130;
      bool _391 = _390 < 382;
      if (_391) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer130, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer130, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer130 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer130, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer130, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer130 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer130, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer130, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer130, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer131 = 0; x_outer131 < 2; ++x_outer131) {
    // cram-array axis
    {
      int32_t x_inner131 = 0;
      int32_t _392 = x_outer131 * 256;
      int32_t _393 = _392 + x_inner131;
      bool _394 = _393 < 381;
      if (_394) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer131, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer131, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer131 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer131, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer131, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer131 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer131, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer131, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer131, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer132 = 0; x_outer132 < 2; ++x_outer132) {
    // cram-array axis
    {
      int32_t x_inner132 = 0;
      int32_t _395 = x_outer132 * 256;
      int32_t _396 = _395 + x_inner132;
      bool _397 = _396 < 380;
      if (_397) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer132, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer132, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer132 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer132, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer132, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer132 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer132, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer132, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer132, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer133 = 0; x_outer133 < 2; ++x_outer133) {
    // cram-array axis
    {
      int32_t x_inner133 = 0;
      int32_t _398 = x_outer133 * 256;
      int32_t _399 = _398 + x_inner133;
      bool _400 = _399 < 379;
      if (_400) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer133, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer133, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer133 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer133, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer133, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer133 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer133, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer133, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer133, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer134 = 0; x_outer134 < 2; ++x_outer134) {
    // cram-array axis
    {
      int32_t x_inner134 = 0;
      int32_t _401 = x_outer134 * 256;
      int32_t _402 = _401 + x_inner134;
      bool _403 = _402 < 378;
      if (_403) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer134, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer134, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer134 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer134, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer134, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer134 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer134, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer134, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer134, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer135 = 0; x_outer135 < 2; ++x_outer135) {
    // cram-array axis
    {
      int32_t x_inner135 = 0;
      int32_t _404 = x_outer135 * 256;
      int32_t _405 = _404 + x_inner135;
      bool _406 = _405 < 377;
      if (_406) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer135, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer135, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer135 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer135, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer135, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer135 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer135, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer135, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer135, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer136 = 0; x_outer136 < 2; ++x_outer136) {
    // cram-array axis
    {
      int32_t x_inner136 = 0;
      int32_t _407 = x_outer136 * 256;
      int32_t _408 = _407 + x_inner136;
      bool _409 = _408 < 376;
      if (_409) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer136, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer136, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer136 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer136, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer136, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer136 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer136, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer136, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer136, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer137 = 0; x_outer137 < 2; ++x_outer137) {
    // cram-array axis
    {
      int32_t x_inner137 = 0;
      int32_t _410 = x_outer137 * 256;
      int32_t _411 = _410 + x_inner137;
      bool _412 = _411 < 375;
      if (_412) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer137, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer137, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer137 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer137, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer137, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer137 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer137, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer137, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer137, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer138 = 0; x_outer138 < 2; ++x_outer138) {
    // cram-array axis
    {
      int32_t x_inner138 = 0;
      int32_t _413 = x_outer138 * 256;
      int32_t _414 = _413 + x_inner138;
      bool _415 = _414 < 374;
      if (_415) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer138, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer138, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer138 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer138, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer138, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer138 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer138, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer138, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer138, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer139 = 0; x_outer139 < 2; ++x_outer139) {
    // cram-array axis
    {
      int32_t x_inner139 = 0;
      int32_t _416 = x_outer139 * 256;
      int32_t _417 = _416 + x_inner139;
      bool _418 = _417 < 373;
      if (_418) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer139, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer139, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer139 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer139, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer139, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer139 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer139, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer139, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer139, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer140 = 0; x_outer140 < 2; ++x_outer140) {
    // cram-array axis
    {
      int32_t x_inner140 = 0;
      int32_t _419 = x_outer140 * 256;
      int32_t _420 = _419 + x_inner140;
      bool _421 = _420 < 372;
      if (_421) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer140, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer140, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer140 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer140, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer140, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer140 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer140, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer140, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer140, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer141 = 0; x_outer141 < 2; ++x_outer141) {
    // cram-array axis
    {
      int32_t x_inner141 = 0;
      int32_t _422 = x_outer141 * 256;
      int32_t _423 = _422 + x_inner141;
      bool _424 = _423 < 371;
      if (_424) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer141, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer141, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer141 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer141, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer141, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer141 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer141, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer141, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer141, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer142 = 0; x_outer142 < 2; ++x_outer142) {
    // cram-array axis
    {
      int32_t x_inner142 = 0;
      int32_t _425 = x_outer142 * 256;
      int32_t _426 = _425 + x_inner142;
      bool _427 = _426 < 370;
      if (_427) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer142, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer142, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer142 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer142, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer142, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer142 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer142, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer142, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer142, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer143 = 0; x_outer143 < 2; ++x_outer143) {
    // cram-array axis
    {
      int32_t x_inner143 = 0;
      int32_t _428 = x_outer143 * 256;
      int32_t _429 = _428 + x_inner143;
      bool _430 = _429 < 369;
      if (_430) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer143, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer143, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer143 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer143, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer143, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer143 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer143, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer143, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer143, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer144 = 0; x_outer144 < 2; ++x_outer144) {
    // cram-array axis
    {
      int32_t x_inner144 = 0;
      int32_t _431 = x_outer144 * 256;
      int32_t _432 = _431 + x_inner144;
      bool _433 = _432 < 368;
      if (_433) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer144, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer144, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer144 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer144, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer144, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer144 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer144, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer144, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer144, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer145 = 0; x_outer145 < 2; ++x_outer145) {
    // cram-array axis
    {
      int32_t x_inner145 = 0;
      int32_t _434 = x_outer145 * 256;
      int32_t _435 = _434 + x_inner145;
      bool _436 = _435 < 367;
      if (_436) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer145, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer145, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer145 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer145, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer145, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer145 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer145, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer145, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer145, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer146 = 0; x_outer146 < 2; ++x_outer146) {
    // cram-array axis
    {
      int32_t x_inner146 = 0;
      int32_t _437 = x_outer146 * 256;
      int32_t _438 = _437 + x_inner146;
      bool _439 = _438 < 366;
      if (_439) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer146, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer146, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer146 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer146, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer146, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer146 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer146, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer146, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer146, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer147 = 0; x_outer147 < 2; ++x_outer147) {
    // cram-array axis
    {
      int32_t x_inner147 = 0;
      int32_t _440 = x_outer147 * 256;
      int32_t _441 = _440 + x_inner147;
      bool _442 = _441 < 365;
      if (_442) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer147, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer147, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer147 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer147, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer147, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer147 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer147, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer147, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer147, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer148 = 0; x_outer148 < 2; ++x_outer148) {
    // cram-array axis
    {
      int32_t x_inner148 = 0;
      int32_t _443 = x_outer148 * 256;
      int32_t _444 = _443 + x_inner148;
      bool _445 = _444 < 364;
      if (_445) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer148, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer148, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer148 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer148, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer148, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer148 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer148, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer148, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer148, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer149 = 0; x_outer149 < 2; ++x_outer149) {
    // cram-array axis
    {
      int32_t x_inner149 = 0;
      int32_t _446 = x_outer149 * 256;
      int32_t _447 = _446 + x_inner149;
      bool _448 = _447 < 363;
      if (_448) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer149, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer149, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer149 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer149, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer149, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer149 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer149, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer149, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer149, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer150 = 0; x_outer150 < 2; ++x_outer150) {
    // cram-array axis
    {
      int32_t x_inner150 = 0;
      int32_t _449 = x_outer150 * 256;
      int32_t _450 = _449 + x_inner150;
      bool _451 = _450 < 362;
      if (_451) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer150, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer150, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer150 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer150, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer150, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer150 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer150, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer150, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer150, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer151 = 0; x_outer151 < 2; ++x_outer151) {
    // cram-array axis
    {
      int32_t x_inner151 = 0;
      int32_t _452 = x_outer151 * 256;
      int32_t _453 = _452 + x_inner151;
      bool _454 = _453 < 361;
      if (_454) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer151, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer151, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer151 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer151, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer151, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer151 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer151, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer151, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer151, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer152 = 0; x_outer152 < 2; ++x_outer152) {
    // cram-array axis
    {
      int32_t x_inner152 = 0;
      int32_t _455 = x_outer152 * 256;
      int32_t _456 = _455 + x_inner152;
      bool _457 = _456 < 360;
      if (_457) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer152, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer152, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer152 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer152, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer152, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer152 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer152, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer152, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer152, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer153 = 0; x_outer153 < 2; ++x_outer153) {
    // cram-array axis
    {
      int32_t x_inner153 = 0;
      int32_t _458 = x_outer153 * 256;
      int32_t _459 = _458 + x_inner153;
      bool _460 = _459 < 359;
      if (_460) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer153, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer153, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer153 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer153, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer153, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer153 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer153, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer153, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer153, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer154 = 0; x_outer154 < 2; ++x_outer154) {
    // cram-array axis
    {
      int32_t x_inner154 = 0;
      int32_t _461 = x_outer154 * 256;
      int32_t _462 = _461 + x_inner154;
      bool _463 = _462 < 358;
      if (_463) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer154, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer154, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer154 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer154, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer154, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer154 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer154, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer154, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer154, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer155 = 0; x_outer155 < 2; ++x_outer155) {
    // cram-array axis
    {
      int32_t x_inner155 = 0;
      int32_t _464 = x_outer155 * 256;
      int32_t _465 = _464 + x_inner155;
      bool _466 = _465 < 357;
      if (_466) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer155, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer155, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer155 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer155, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer155, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer155 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer155, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer155, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer155, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer156 = 0; x_outer156 < 2; ++x_outer156) {
    // cram-array axis
    {
      int32_t x_inner156 = 0;
      int32_t _467 = x_outer156 * 256;
      int32_t _468 = _467 + x_inner156;
      bool _469 = _468 < 356;
      if (_469) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer156, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer156, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer156 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer156, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer156, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer156 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer156, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer156, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer156, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer157 = 0; x_outer157 < 2; ++x_outer157) {
    // cram-array axis
    {
      int32_t x_inner157 = 0;
      int32_t _470 = x_outer157 * 256;
      int32_t _471 = _470 + x_inner157;
      bool _472 = _471 < 355;
      if (_472) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer157, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer157, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer157 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer157, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer157, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer157 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer157, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer157, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer157, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer158 = 0; x_outer158 < 2; ++x_outer158) {
    // cram-array axis
    {
      int32_t x_inner158 = 0;
      int32_t _473 = x_outer158 * 256;
      int32_t _474 = _473 + x_inner158;
      bool _475 = _474 < 354;
      if (_475) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer158, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer158, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer158 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer158, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer158, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer158 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer158, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer158, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer158, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer159 = 0; x_outer159 < 2; ++x_outer159) {
    // cram-array axis
    {
      int32_t x_inner159 = 0;
      int32_t _476 = x_outer159 * 256;
      int32_t _477 = _476 + x_inner159;
      bool _478 = _477 < 353;
      if (_478) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer159, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer159, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer159 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer159, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer159, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer159 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer159, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer159, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer159, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer160 = 0; x_outer160 < 2; ++x_outer160) {
    // cram-array axis
    {
      int32_t x_inner160 = 0;
      int32_t _479 = x_outer160 * 256;
      int32_t _480 = _479 + x_inner160;
      bool _481 = _480 < 352;
      if (_481) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer160, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer160, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer160 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer160, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer160, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer160 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer160, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer160, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer160, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer161 = 0; x_outer161 < 2; ++x_outer161) {
    // cram-array axis
    {
      int32_t x_inner161 = 0;
      int32_t _482 = x_outer161 * 256;
      int32_t _483 = _482 + x_inner161;
      bool _484 = _483 < 351;
      if (_484) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer161, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer161, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer161 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer161, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer161, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer161 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer161, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer161, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer161, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer162 = 0; x_outer162 < 2; ++x_outer162) {
    // cram-array axis
    {
      int32_t x_inner162 = 0;
      int32_t _485 = x_outer162 * 256;
      int32_t _486 = _485 + x_inner162;
      bool _487 = _486 < 350;
      if (_487) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer162, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer162, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer162 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer162, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer162, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer162 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer162, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer162, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer162, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer163 = 0; x_outer163 < 2; ++x_outer163) {
    // cram-array axis
    {
      int32_t x_inner163 = 0;
      int32_t _488 = x_outer163 * 256;
      int32_t _489 = _488 + x_inner163;
      bool _490 = _489 < 349;
      if (_490) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer163, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer163, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer163 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer163, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer163, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer163 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer163, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer163, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer163, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer164 = 0; x_outer164 < 2; ++x_outer164) {
    // cram-array axis
    {
      int32_t x_inner164 = 0;
      int32_t _491 = x_outer164 * 256;
      int32_t _492 = _491 + x_inner164;
      bool _493 = _492 < 348;
      if (_493) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer164, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer164, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer164 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer164, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer164, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer164 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer164, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer164, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer164, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer165 = 0; x_outer165 < 2; ++x_outer165) {
    // cram-array axis
    {
      int32_t x_inner165 = 0;
      int32_t _494 = x_outer165 * 256;
      int32_t _495 = _494 + x_inner165;
      bool _496 = _495 < 347;
      if (_496) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer165, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer165, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer165 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer165, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer165, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer165 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer165, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer165, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer165, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer166 = 0; x_outer166 < 2; ++x_outer166) {
    // cram-array axis
    {
      int32_t x_inner166 = 0;
      int32_t _497 = x_outer166 * 256;
      int32_t _498 = _497 + x_inner166;
      bool _499 = _498 < 346;
      if (_499) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer166, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer166, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer166 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer166, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer166, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer166 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer166, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer166, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer166, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer167 = 0; x_outer167 < 2; ++x_outer167) {
    // cram-array axis
    {
      int32_t x_inner167 = 0;
      int32_t _500 = x_outer167 * 256;
      int32_t _501 = _500 + x_inner167;
      bool _502 = _501 < 345;
      if (_502) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer167, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer167, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer167 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer167, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer167, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer167 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer167, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer167, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer167, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer168 = 0; x_outer168 < 2; ++x_outer168) {
    // cram-array axis
    {
      int32_t x_inner168 = 0;
      int32_t _503 = x_outer168 * 256;
      int32_t _504 = _503 + x_inner168;
      bool _505 = _504 < 344;
      if (_505) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer168, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer168, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer168 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer168, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer168, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer168 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer168, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer168, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer168, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer169 = 0; x_outer169 < 2; ++x_outer169) {
    // cram-array axis
    {
      int32_t x_inner169 = 0;
      int32_t _506 = x_outer169 * 256;
      int32_t _507 = _506 + x_inner169;
      bool _508 = _507 < 343;
      if (_508) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer169, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer169, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer169 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer169, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer169, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer169 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer169, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer169, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer169, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer170 = 0; x_outer170 < 2; ++x_outer170) {
    // cram-array axis
    {
      int32_t x_inner170 = 0;
      int32_t _509 = x_outer170 * 256;
      int32_t _510 = _509 + x_inner170;
      bool _511 = _510 < 342;
      if (_511) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer170, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer170, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer170 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer170, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer170, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer170 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer170, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer170, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer170, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer171 = 0; x_outer171 < 2; ++x_outer171) {
    // cram-array axis
    {
      int32_t x_inner171 = 0;
      int32_t _512 = x_outer171 * 256;
      int32_t _513 = _512 + x_inner171;
      bool _514 = _513 < 341;
      if (_514) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer171, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer171, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer171 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer171, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer171, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer171 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer171, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer171, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer171, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer172 = 0; x_outer172 < 2; ++x_outer172) {
    // cram-array axis
    {
      int32_t x_inner172 = 0;
      int32_t _515 = x_outer172 * 256;
      int32_t _516 = _515 + x_inner172;
      bool _517 = _516 < 340;
      if (_517) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer172, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer172, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer172 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer172, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer172, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer172 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer172, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer172, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer172, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer173 = 0; x_outer173 < 2; ++x_outer173) {
    // cram-array axis
    {
      int32_t x_inner173 = 0;
      int32_t _518 = x_outer173 * 256;
      int32_t _519 = _518 + x_inner173;
      bool _520 = _519 < 339;
      if (_520) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer173, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer173, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer173 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer173, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer173, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer173 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer173, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer173, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer173, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer174 = 0; x_outer174 < 2; ++x_outer174) {
    // cram-array axis
    {
      int32_t x_inner174 = 0;
      int32_t _521 = x_outer174 * 256;
      int32_t _522 = _521 + x_inner174;
      bool _523 = _522 < 338;
      if (_523) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer174, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer174, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer174 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer174, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer174, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer174 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer174, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer174, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer174, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer175 = 0; x_outer175 < 2; ++x_outer175) {
    // cram-array axis
    {
      int32_t x_inner175 = 0;
      int32_t _524 = x_outer175 * 256;
      int32_t _525 = _524 + x_inner175;
      bool _526 = _525 < 337;
      if (_526) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer175, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer175, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer175 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer175, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer175, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer175 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer175, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer175, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer175, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer176 = 0; x_outer176 < 2; ++x_outer176) {
    // cram-array axis
    {
      int32_t x_inner176 = 0;
      int32_t _527 = x_outer176 * 256;
      int32_t _528 = _527 + x_inner176;
      bool _529 = _528 < 336;
      if (_529) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer176, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer176, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer176 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer176, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer176, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer176 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer176, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer176, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer176, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer177 = 0; x_outer177 < 2; ++x_outer177) {
    // cram-array axis
    {
      int32_t x_inner177 = 0;
      int32_t _530 = x_outer177 * 256;
      int32_t _531 = _530 + x_inner177;
      bool _532 = _531 < 335;
      if (_532) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer177, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer177, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer177 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer177, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer177, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer177 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer177, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer177, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer177, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer178 = 0; x_outer178 < 2; ++x_outer178) {
    // cram-array axis
    {
      int32_t x_inner178 = 0;
      int32_t _533 = x_outer178 * 256;
      int32_t _534 = _533 + x_inner178;
      bool _535 = _534 < 334;
      if (_535) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer178, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer178, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer178 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer178, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer178, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer178 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer178, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer178, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer178, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer179 = 0; x_outer179 < 2; ++x_outer179) {
    // cram-array axis
    {
      int32_t x_inner179 = 0;
      int32_t _536 = x_outer179 * 256;
      int32_t _537 = _536 + x_inner179;
      bool _538 = _537 < 333;
      if (_538) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer179, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer179, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer179 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer179, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer179, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer179 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer179, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer179, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer179, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer180 = 0; x_outer180 < 2; ++x_outer180) {
    // cram-array axis
    {
      int32_t x_inner180 = 0;
      int32_t _539 = x_outer180 * 256;
      int32_t _540 = _539 + x_inner180;
      bool _541 = _540 < 332;
      if (_541) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer180, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer180, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer180 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer180, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer180, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer180 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer180, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer180, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer180, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer181 = 0; x_outer181 < 2; ++x_outer181) {
    // cram-array axis
    {
      int32_t x_inner181 = 0;
      int32_t _542 = x_outer181 * 256;
      int32_t _543 = _542 + x_inner181;
      bool _544 = _543 < 331;
      if (_544) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer181, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer181, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer181 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer181, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer181, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer181 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer181, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer181, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer181, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer182 = 0; x_outer182 < 2; ++x_outer182) {
    // cram-array axis
    {
      int32_t x_inner182 = 0;
      int32_t _545 = x_outer182 * 256;
      int32_t _546 = _545 + x_inner182;
      bool _547 = _546 < 330;
      if (_547) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer182, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer182, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer182 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer182, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer182, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer182 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer182, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer182, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer182, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer183 = 0; x_outer183 < 2; ++x_outer183) {
    // cram-array axis
    {
      int32_t x_inner183 = 0;
      int32_t _548 = x_outer183 * 256;
      int32_t _549 = _548 + x_inner183;
      bool _550 = _549 < 329;
      if (_550) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer183, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer183, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer183 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer183, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer183, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer183 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer183, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer183, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer183, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer184 = 0; x_outer184 < 2; ++x_outer184) {
    // cram-array axis
    {
      int32_t x_inner184 = 0;
      int32_t _551 = x_outer184 * 256;
      int32_t _552 = _551 + x_inner184;
      bool _553 = _552 < 328;
      if (_553) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer184, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer184, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer184 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer184, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer184, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer184 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer184, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer184, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer184, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer185 = 0; x_outer185 < 2; ++x_outer185) {
    // cram-array axis
    {
      int32_t x_inner185 = 0;
      int32_t _554 = x_outer185 * 256;
      int32_t _555 = _554 + x_inner185;
      bool _556 = _555 < 327;
      if (_556) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer185, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer185, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer185 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer185, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer185, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer185 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer185, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer185, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer185, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer186 = 0; x_outer186 < 2; ++x_outer186) {
    // cram-array axis
    {
      int32_t x_inner186 = 0;
      int32_t _557 = x_outer186 * 256;
      int32_t _558 = _557 + x_inner186;
      bool _559 = _558 < 326;
      if (_559) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer186, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer186, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer186 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer186, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer186, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer186 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer186, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer186, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer186, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer187 = 0; x_outer187 < 2; ++x_outer187) {
    // cram-array axis
    {
      int32_t x_inner187 = 0;
      int32_t _560 = x_outer187 * 256;
      int32_t _561 = _560 + x_inner187;
      bool _562 = _561 < 325;
      if (_562) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer187, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer187, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer187 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer187, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer187, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer187 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer187, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer187, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer187, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer188 = 0; x_outer188 < 2; ++x_outer188) {
    // cram-array axis
    {
      int32_t x_inner188 = 0;
      int32_t _563 = x_outer188 * 256;
      int32_t _564 = _563 + x_inner188;
      bool _565 = _564 < 324;
      if (_565) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer188, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer188, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer188 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer188, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer188, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer188 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer188, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer188, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer188, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer189 = 0; x_outer189 < 2; ++x_outer189) {
    // cram-array axis
    {
      int32_t x_inner189 = 0;
      int32_t _566 = x_outer189 * 256;
      int32_t _567 = _566 + x_inner189;
      bool _568 = _567 < 323;
      if (_568) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer189, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer189, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer189 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer189, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer189, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer189 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer189, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer189, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer189, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer190 = 0; x_outer190 < 2; ++x_outer190) {
    // cram-array axis
    {
      int32_t x_inner190 = 0;
      int32_t _569 = x_outer190 * 256;
      int32_t _570 = _569 + x_inner190;
      bool _571 = _570 < 322;
      if (_571) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer190, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer190, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer190 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer190, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer190, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer190 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer190, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer190, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer190, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer191 = 0; x_outer191 < 2; ++x_outer191) {
    // cram-array axis
    {
      int32_t x_inner191 = 0;
      int32_t _572 = x_outer191 * 256;
      int32_t _573 = _572 + x_inner191;
      bool _574 = _573 < 321;
      if (_574) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer191, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer191, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer191 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer191, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer191, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer191 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer191, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer191, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer191, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer192 = 0; x_outer192 < 2; ++x_outer192) {
    // cram-array axis
    {
      int32_t x_inner192 = 0;
      int32_t _575 = x_outer192 * 256;
      int32_t _576 = _575 + x_inner192;
      bool _577 = _576 < 320;
      if (_577) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer192, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer192, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer192 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer192, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer192, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer192 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer192, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer192, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer192, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer193 = 0; x_outer193 < 2; ++x_outer193) {
    // cram-array axis
    {
      int32_t x_inner193 = 0;
      int32_t _578 = x_outer193 * 256;
      int32_t _579 = _578 + x_inner193;
      bool _580 = _579 < 319;
      if (_580) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer193, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer193, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer193 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer193, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer193, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer193 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer193, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer193, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer193, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer194 = 0; x_outer194 < 2; ++x_outer194) {
    // cram-array axis
    {
      int32_t x_inner194 = 0;
      int32_t _581 = x_outer194 * 256;
      int32_t _582 = _581 + x_inner194;
      bool _583 = _582 < 318;
      if (_583) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer194, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer194, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer194 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer194, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer194, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer194 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer194, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer194, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer194, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer195 = 0; x_outer195 < 2; ++x_outer195) {
    // cram-array axis
    {
      int32_t x_inner195 = 0;
      int32_t _584 = x_outer195 * 256;
      int32_t _585 = _584 + x_inner195;
      bool _586 = _585 < 317;
      if (_586) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer195, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer195, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer195 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer195, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer195, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer195 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer195, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer195, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer195, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer196 = 0; x_outer196 < 2; ++x_outer196) {
    // cram-array axis
    {
      int32_t x_inner196 = 0;
      int32_t _587 = x_outer196 * 256;
      int32_t _588 = _587 + x_inner196;
      bool _589 = _588 < 316;
      if (_589) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer196, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer196, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer196 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer196, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer196, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer196 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer196, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer196, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer196, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer197 = 0; x_outer197 < 2; ++x_outer197) {
    // cram-array axis
    {
      int32_t x_inner197 = 0;
      int32_t _590 = x_outer197 * 256;
      int32_t _591 = _590 + x_inner197;
      bool _592 = _591 < 315;
      if (_592) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer197, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer197, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer197 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer197, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer197, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer197 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer197, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer197, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer197, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer198 = 0; x_outer198 < 2; ++x_outer198) {
    // cram-array axis
    {
      int32_t x_inner198 = 0;
      int32_t _593 = x_outer198 * 256;
      int32_t _594 = _593 + x_inner198;
      bool _595 = _594 < 314;
      if (_595) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer198, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer198, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer198 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer198, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer198, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer198 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer198, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer198, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer198, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer199 = 0; x_outer199 < 2; ++x_outer199) {
    // cram-array axis
    {
      int32_t x_inner199 = 0;
      int32_t _596 = x_outer199 * 256;
      int32_t _597 = _596 + x_inner199;
      bool _598 = _597 < 313;
      if (_598) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer199, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer199, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer199 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer199, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer199, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer199 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer199, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer199, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer199, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer200 = 0; x_outer200 < 2; ++x_outer200) {
    // cram-array axis
    {
      int32_t x_inner200 = 0;
      int32_t _599 = x_outer200 * 256;
      int32_t _600 = _599 + x_inner200;
      bool _601 = _600 < 312;
      if (_601) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer200, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer200, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer200 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer200, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer200, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer200 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer200, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer200, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer200, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer201 = 0; x_outer201 < 2; ++x_outer201) {
    // cram-array axis
    {
      int32_t x_inner201 = 0;
      int32_t _602 = x_outer201 * 256;
      int32_t _603 = _602 + x_inner201;
      bool _604 = _603 < 311;
      if (_604) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer201, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer201, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer201 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer201, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer201, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer201 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer201, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer201, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer201, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer202 = 0; x_outer202 < 2; ++x_outer202) {
    // cram-array axis
    {
      int32_t x_inner202 = 0;
      int32_t _605 = x_outer202 * 256;
      int32_t _606 = _605 + x_inner202;
      bool _607 = _606 < 310;
      if (_607) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer202, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer202, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer202 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer202, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer202, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer202 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer202, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer202, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer202, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer203 = 0; x_outer203 < 2; ++x_outer203) {
    // cram-array axis
    {
      int32_t x_inner203 = 0;
      int32_t _608 = x_outer203 * 256;
      int32_t _609 = _608 + x_inner203;
      bool _610 = _609 < 309;
      if (_610) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer203, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer203, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer203 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer203, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer203, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer203 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer203, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer203, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer203, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer204 = 0; x_outer204 < 2; ++x_outer204) {
    // cram-array axis
    {
      int32_t x_inner204 = 0;
      int32_t _611 = x_outer204 * 256;
      int32_t _612 = _611 + x_inner204;
      bool _613 = _612 < 308;
      if (_613) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer204, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer204, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer204 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer204, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer204, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer204 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer204, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer204, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer204, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer205 = 0; x_outer205 < 2; ++x_outer205) {
    // cram-array axis
    {
      int32_t x_inner205 = 0;
      int32_t _614 = x_outer205 * 256;
      int32_t _615 = _614 + x_inner205;
      bool _616 = _615 < 307;
      if (_616) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer205, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer205, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer205 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer205, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer205, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer205 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer205, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer205, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer205, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer206 = 0; x_outer206 < 2; ++x_outer206) {
    // cram-array axis
    {
      int32_t x_inner206 = 0;
      int32_t _617 = x_outer206 * 256;
      int32_t _618 = _617 + x_inner206;
      bool _619 = _618 < 306;
      if (_619) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer206, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer206, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer206 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer206, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer206, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer206 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer206, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer206, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer206, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer207 = 0; x_outer207 < 2; ++x_outer207) {
    // cram-array axis
    {
      int32_t x_inner207 = 0;
      int32_t _620 = x_outer207 * 256;
      int32_t _621 = _620 + x_inner207;
      bool _622 = _621 < 305;
      if (_622) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer207, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer207, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer207 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer207, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer207, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer207 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer207, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer207, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer207, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer208 = 0; x_outer208 < 2; ++x_outer208) {
    // cram-array axis
    {
      int32_t x_inner208 = 0;
      int32_t _623 = x_outer208 * 256;
      int32_t _624 = _623 + x_inner208;
      bool _625 = _624 < 304;
      if (_625) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer208, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer208, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer208 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer208, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer208, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer208 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer208, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer208, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer208, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer209 = 0; x_outer209 < 2; ++x_outer209) {
    // cram-array axis
    {
      int32_t x_inner209 = 0;
      int32_t _626 = x_outer209 * 256;
      int32_t _627 = _626 + x_inner209;
      bool _628 = _627 < 303;
      if (_628) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer209, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer209, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer209 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer209, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer209, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer209 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer209, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer209, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer209, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer210 = 0; x_outer210 < 2; ++x_outer210) {
    // cram-array axis
    {
      int32_t x_inner210 = 0;
      int32_t _629 = x_outer210 * 256;
      int32_t _630 = _629 + x_inner210;
      bool _631 = _630 < 302;
      if (_631) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer210, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer210, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer210 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer210, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer210, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer210 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer210, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer210, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer210, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer211 = 0; x_outer211 < 2; ++x_outer211) {
    // cram-array axis
    {
      int32_t x_inner211 = 0;
      int32_t _632 = x_outer211 * 256;
      int32_t _633 = _632 + x_inner211;
      bool _634 = _633 < 301;
      if (_634) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer211, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer211, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer211 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer211, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer211, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer211 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer211, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer211, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer211, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer212 = 0; x_outer212 < 2; ++x_outer212) {
    // cram-array axis
    {
      int32_t x_inner212 = 0;
      int32_t _635 = x_outer212 * 256;
      int32_t _636 = _635 + x_inner212;
      bool _637 = _636 < 300;
      if (_637) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer212, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer212, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer212 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer212, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer212, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer212 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer212, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer212, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer212, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer213 = 0; x_outer213 < 2; ++x_outer213) {
    // cram-array axis
    {
      int32_t x_inner213 = 0;
      int32_t _638 = x_outer213 * 256;
      int32_t _639 = _638 + x_inner213;
      bool _640 = _639 < 299;
      if (_640) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer213, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer213, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer213 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer213, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer213, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer213 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer213, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer213, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer213, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer214 = 0; x_outer214 < 2; ++x_outer214) {
    // cram-array axis
    {
      int32_t x_inner214 = 0;
      int32_t _641 = x_outer214 * 256;
      int32_t _642 = _641 + x_inner214;
      bool _643 = _642 < 298;
      if (_643) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer214, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer214, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer214 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer214, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer214, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer214 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer214, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer214, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer214, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer215 = 0; x_outer215 < 2; ++x_outer215) {
    // cram-array axis
    {
      int32_t x_inner215 = 0;
      int32_t _644 = x_outer215 * 256;
      int32_t _645 = _644 + x_inner215;
      bool _646 = _645 < 297;
      if (_646) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer215, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer215, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer215 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer215, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer215, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer215 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer215, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer215, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer215, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer216 = 0; x_outer216 < 2; ++x_outer216) {
    // cram-array axis
    {
      int32_t x_inner216 = 0;
      int32_t _647 = x_outer216 * 256;
      int32_t _648 = _647 + x_inner216;
      bool _649 = _648 < 296;
      if (_649) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer216, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer216, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer216 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer216, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer216, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer216 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer216, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer216, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer216, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer217 = 0; x_outer217 < 2; ++x_outer217) {
    // cram-array axis
    {
      int32_t x_inner217 = 0;
      int32_t _650 = x_outer217 * 256;
      int32_t _651 = _650 + x_inner217;
      bool _652 = _651 < 295;
      if (_652) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer217, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer217, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer217 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer217, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer217, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer217 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer217, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer217, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer217, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer218 = 0; x_outer218 < 2; ++x_outer218) {
    // cram-array axis
    {
      int32_t x_inner218 = 0;
      int32_t _653 = x_outer218 * 256;
      int32_t _654 = _653 + x_inner218;
      bool _655 = _654 < 294;
      if (_655) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer218, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer218, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer218 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer218, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer218, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer218 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer218, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer218, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer218, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer219 = 0; x_outer219 < 2; ++x_outer219) {
    // cram-array axis
    {
      int32_t x_inner219 = 0;
      int32_t _656 = x_outer219 * 256;
      int32_t _657 = _656 + x_inner219;
      bool _658 = _657 < 293;
      if (_658) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer219, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer219, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer219 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer219, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer219, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer219 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer219, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer219, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer219, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer220 = 0; x_outer220 < 2; ++x_outer220) {
    // cram-array axis
    {
      int32_t x_inner220 = 0;
      int32_t _659 = x_outer220 * 256;
      int32_t _660 = _659 + x_inner220;
      bool _661 = _660 < 292;
      if (_661) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer220, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer220, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer220 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer220, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer220, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer220 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer220, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer220, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer220, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer221 = 0; x_outer221 < 2; ++x_outer221) {
    // cram-array axis
    {
      int32_t x_inner221 = 0;
      int32_t _662 = x_outer221 * 256;
      int32_t _663 = _662 + x_inner221;
      bool _664 = _663 < 291;
      if (_664) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer221, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer221, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer221 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer221, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer221, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer221 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer221, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer221, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer221, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer222 = 0; x_outer222 < 2; ++x_outer222) {
    // cram-array axis
    {
      int32_t x_inner222 = 0;
      int32_t _665 = x_outer222 * 256;
      int32_t _666 = _665 + x_inner222;
      bool _667 = _666 < 290;
      if (_667) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer222, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer222, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer222 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer222, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer222, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer222 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer222, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer222, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer222, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer223 = 0; x_outer223 < 2; ++x_outer223) {
    // cram-array axis
    {
      int32_t x_inner223 = 0;
      int32_t _668 = x_outer223 * 256;
      int32_t _669 = _668 + x_inner223;
      bool _670 = _669 < 289;
      if (_670) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer223, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer223, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer223 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer223, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer223, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer223 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer223, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer223, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer223, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer224 = 0; x_outer224 < 2; ++x_outer224) {
    // cram-array axis
    {
      int32_t x_inner224 = 0;
      int32_t _671 = x_outer224 * 256;
      int32_t _672 = _671 + x_inner224;
      bool _673 = _672 < 288;
      if (_673) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer224, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer224, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer224 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer224, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer224, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer224 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer224, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer224, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer224, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer225 = 0; x_outer225 < 2; ++x_outer225) {
    // cram-array axis
    {
      int32_t x_inner225 = 0;
      int32_t _674 = x_outer225 * 256;
      int32_t _675 = _674 + x_inner225;
      bool _676 = _675 < 287;
      if (_676) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer225, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer225, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer225 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer225, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer225, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer225 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer225, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer225, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer225, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer226 = 0; x_outer226 < 2; ++x_outer226) {
    // cram-array axis
    {
      int32_t x_inner226 = 0;
      int32_t _677 = x_outer226 * 256;
      int32_t _678 = _677 + x_inner226;
      bool _679 = _678 < 286;
      if (_679) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer226, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer226, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer226 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer226, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer226, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer226 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer226, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer226, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer226, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer227 = 0; x_outer227 < 2; ++x_outer227) {
    // cram-array axis
    {
      int32_t x_inner227 = 0;
      int32_t _680 = x_outer227 * 256;
      int32_t _681 = _680 + x_inner227;
      bool _682 = _681 < 285;
      if (_682) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer227, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer227, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer227 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer227, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer227, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer227 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer227, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer227, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer227, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer228 = 0; x_outer228 < 2; ++x_outer228) {
    // cram-array axis
    {
      int32_t x_inner228 = 0;
      int32_t _683 = x_outer228 * 256;
      int32_t _684 = _683 + x_inner228;
      bool _685 = _684 < 284;
      if (_685) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer228, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer228, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer228 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer228, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer228, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer228 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer228, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer228, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer228, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer229 = 0; x_outer229 < 2; ++x_outer229) {
    // cram-array axis
    {
      int32_t x_inner229 = 0;
      int32_t _686 = x_outer229 * 256;
      int32_t _687 = _686 + x_inner229;
      bool _688 = _687 < 283;
      if (_688) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer229, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer229, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer229 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer229, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer229, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer229 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer229, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer229, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer229, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer230 = 0; x_outer230 < 2; ++x_outer230) {
    // cram-array axis
    {
      int32_t x_inner230 = 0;
      int32_t _689 = x_outer230 * 256;
      int32_t _690 = _689 + x_inner230;
      bool _691 = _690 < 282;
      if (_691) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer230, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer230, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer230 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer230, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer230, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer230 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer230, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer230, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer230, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer231 = 0; x_outer231 < 2; ++x_outer231) {
    // cram-array axis
    {
      int32_t x_inner231 = 0;
      int32_t _692 = x_outer231 * 256;
      int32_t _693 = _692 + x_inner231;
      bool _694 = _693 < 281;
      if (_694) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer231, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer231, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer231 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer231, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer231, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer231 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer231, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer231, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer231, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer232 = 0; x_outer232 < 2; ++x_outer232) {
    // cram-array axis
    {
      int32_t x_inner232 = 0;
      int32_t _695 = x_outer232 * 256;
      int32_t _696 = _695 + x_inner232;
      bool _697 = _696 < 280;
      if (_697) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer232, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer232, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer232 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer232, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer232, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer232 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer232, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer232, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer232, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer233 = 0; x_outer233 < 2; ++x_outer233) {
    // cram-array axis
    {
      int32_t x_inner233 = 0;
      int32_t _698 = x_outer233 * 256;
      int32_t _699 = _698 + x_inner233;
      bool _700 = _699 < 279;
      if (_700) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer233, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer233, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer233 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer233, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer233, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer233 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer233, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer233, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer233, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer234 = 0; x_outer234 < 2; ++x_outer234) {
    // cram-array axis
    {
      int32_t x_inner234 = 0;
      int32_t _701 = x_outer234 * 256;
      int32_t _702 = _701 + x_inner234;
      bool _703 = _702 < 278;
      if (_703) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer234, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer234, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer234 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer234, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer234, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer234 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer234, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer234, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer234, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer235 = 0; x_outer235 < 2; ++x_outer235) {
    // cram-array axis
    {
      int32_t x_inner235 = 0;
      int32_t _704 = x_outer235 * 256;
      int32_t _705 = _704 + x_inner235;
      bool _706 = _705 < 277;
      if (_706) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer235, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer235, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer235 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer235, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer235, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer235 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer235, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer235, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer235, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer236 = 0; x_outer236 < 2; ++x_outer236) {
    // cram-array axis
    {
      int32_t x_inner236 = 0;
      int32_t _707 = x_outer236 * 256;
      int32_t _708 = _707 + x_inner236;
      bool _709 = _708 < 276;
      if (_709) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer236, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer236, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer236 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer236, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer236, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer236 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer236, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer236, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer236, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer237 = 0; x_outer237 < 2; ++x_outer237) {
    // cram-array axis
    {
      int32_t x_inner237 = 0;
      int32_t _710 = x_outer237 * 256;
      int32_t _711 = _710 + x_inner237;
      bool _712 = _711 < 275;
      if (_712) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer237, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer237, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer237 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer237, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer237, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer237 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer237, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer237, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer237, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer238 = 0; x_outer238 < 2; ++x_outer238) {
    // cram-array axis
    {
      int32_t x_inner238 = 0;
      int32_t _713 = x_outer238 * 256;
      int32_t _714 = _713 + x_inner238;
      bool _715 = _714 < 274;
      if (_715) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer238, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer238, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer238 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer238, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer238, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer238 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer238, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer238, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer238, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer239 = 0; x_outer239 < 2; ++x_outer239) {
    // cram-array axis
    {
      int32_t x_inner239 = 0;
      int32_t _716 = x_outer239 * 256;
      int32_t _717 = _716 + x_inner239;
      bool _718 = _717 < 273;
      if (_718) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer239, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer239, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer239 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer239, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer239, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer239 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer239, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer239, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer239, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer240 = 0; x_outer240 < 2; ++x_outer240) {
    // cram-array axis
    {
      int32_t x_inner240 = 0;
      int32_t _719 = x_outer240 * 256;
      int32_t _720 = _719 + x_inner240;
      bool _721 = _720 < 272;
      if (_721) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer240, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer240, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer240 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer240, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer240, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer240 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer240, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer240, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer240, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer241 = 0; x_outer241 < 2; ++x_outer241) {
    // cram-array axis
    {
      int32_t x_inner241 = 0;
      int32_t _722 = x_outer241 * 256;
      int32_t _723 = _722 + x_inner241;
      bool _724 = _723 < 271;
      if (_724) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer241, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer241, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer241 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer241, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer241, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer241 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer241, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer241, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer241, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer242 = 0; x_outer242 < 2; ++x_outer242) {
    // cram-array axis
    {
      int32_t x_inner242 = 0;
      int32_t _725 = x_outer242 * 256;
      int32_t _726 = _725 + x_inner242;
      bool _727 = _726 < 270;
      if (_727) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer242, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer242, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer242 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer242, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer242, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer242 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer242, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer242, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer242, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer243 = 0; x_outer243 < 2; ++x_outer243) {
    // cram-array axis
    {
      int32_t x_inner243 = 0;
      int32_t _728 = x_outer243 * 256;
      int32_t _729 = _728 + x_inner243;
      bool _730 = _729 < 269;
      if (_730) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer243, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer243, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer243 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer243, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer243, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer243 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer243, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer243, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer243, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer244 = 0; x_outer244 < 2; ++x_outer244) {
    // cram-array axis
    {
      int32_t x_inner244 = 0;
      int32_t _731 = x_outer244 * 256;
      int32_t _732 = _731 + x_inner244;
      bool _733 = _732 < 268;
      if (_733) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer244, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer244, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer244 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer244, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer244, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer244 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer244, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer244, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer244, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer245 = 0; x_outer245 < 2; ++x_outer245) {
    // cram-array axis
    {
      int32_t x_inner245 = 0;
      int32_t _734 = x_outer245 * 256;
      int32_t _735 = _734 + x_inner245;
      bool _736 = _735 < 267;
      if (_736) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer245, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer245, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer245 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer245, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer245, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer245 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer245, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer245, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer245, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer246 = 0; x_outer246 < 2; ++x_outer246) {
    // cram-array axis
    {
      int32_t x_inner246 = 0;
      int32_t _737 = x_outer246 * 256;
      int32_t _738 = _737 + x_inner246;
      bool _739 = _738 < 266;
      if (_739) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer246, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer246, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer246 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer246, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer246, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer246 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer246, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer246, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer246, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer247 = 0; x_outer247 < 2; ++x_outer247) {
    // cram-array axis
    {
      int32_t x_inner247 = 0;
      int32_t _740 = x_outer247 * 256;
      int32_t _741 = _740 + x_inner247;
      bool _742 = _741 < 265;
      if (_742) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer247, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer247, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer247 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer247, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer247, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer247 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer247, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer247, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer247, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer248 = 0; x_outer248 < 2; ++x_outer248) {
    // cram-array axis
    {
      int32_t x_inner248 = 0;
      int32_t _743 = x_outer248 * 256;
      int32_t _744 = _743 + x_inner248;
      bool _745 = _744 < 264;
      if (_745) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer248, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer248, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer248 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer248, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer248, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer248 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer248, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer248, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer248, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer249 = 0; x_outer249 < 2; ++x_outer249) {
    // cram-array axis
    {
      int32_t x_inner249 = 0;
      int32_t _746 = x_outer249 * 256;
      int32_t _747 = _746 + x_inner249;
      bool _748 = _747 < 263;
      if (_748) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer249, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer249, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer249 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer249, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer249, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer249 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer249, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer249, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer249, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer250 = 0; x_outer250 < 2; ++x_outer250) {
    // cram-array axis
    {
      int32_t x_inner250 = 0;
      int32_t _749 = x_outer250 * 256;
      int32_t _750 = _749 + x_inner250;
      bool _751 = _750 < 262;
      if (_751) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer250, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer250, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer250 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer250, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer250, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer250 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer250, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer250, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer250, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer251 = 0; x_outer251 < 2; ++x_outer251) {
    // cram-array axis
    {
      int32_t x_inner251 = 0;
      int32_t _752 = x_outer251 * 256;
      int32_t _753 = _752 + x_inner251;
      bool _754 = _753 < 261;
      if (_754) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer251, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer251, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer251 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer251, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer251, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer251 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer251, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer251, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer251, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer252 = 0; x_outer252 < 2; ++x_outer252) {
    // cram-array axis
    {
      int32_t x_inner252 = 0;
      int32_t _755 = x_outer252 * 256;
      int32_t _756 = _755 + x_inner252;
      bool _757 = _756 < 260;
      if (_757) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer252, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer252, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer252 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer252, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer252, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer252 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer252, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer252, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer252, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer253 = 0; x_outer253 < 2; ++x_outer253) {
    // cram-array axis
    {
      int32_t x_inner253 = 0;
      int32_t _758 = x_outer253 * 256;
      int32_t _759 = _758 + x_inner253;
      bool _760 = _759 < 259;
      if (_760) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer253, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer253, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer253 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer253, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer253, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer253 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer253, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer253, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer253, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer254 = 0; x_outer254 < 2; ++x_outer254) {
    // cram-array axis
    {
      int32_t x_inner254 = 0;
      int32_t _761 = x_outer254 * 256;
      int32_t _762 = _761 + x_inner254;
      bool _763 = _762 < 258;
      if (_763) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer254, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer254, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer254 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer254, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer254, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer254 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer254, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer254, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer254, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  for (int32_t x_outer255 = 0; x_outer255 < 2; ++x_outer255) {
    // cram-array axis
    {
      int32_t x_inner255 = 0;
      int32_t _764 = x_outer255 * 256;
      int32_t _765 = _764 + x_inner255;
      bool _766 = _765 < 257;
      if (_766) {
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer255, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer255, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(((x.outer*131072) + (x.inner*512)), 1, 512)]*/);
          request.addOperand(x_outer255 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer255, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer255, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer255 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer255, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer255, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
          request.addOperand(sys->getAddress(x_outer255, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
          sys->sendRequest(request);
        }
      }
    }
  }
  // cram-array axis
  {
    int32_t x_inner256 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner257 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner258 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner259 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner260 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner261 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner262 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner263 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner264 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner265 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner266 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner267 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner268 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner269 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner270 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner271 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner272 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner273 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner274 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner275 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner276 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner277 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner278 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner279 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner280 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner281 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner282 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner283 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner284 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner285 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner286 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner287 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner288 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner289 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner290 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner291 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner292 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner293 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner294 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner295 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner296 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner297 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner298 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner299 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner300 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner301 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner302 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner303 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner304 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner305 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner306 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner307 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner308 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner309 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner310 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner311 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner312 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner313 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner314 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner315 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner316 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner317 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner318 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner319 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner320 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner321 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner322 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner323 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner324 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner325 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner326 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner327 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner328 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner329 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner330 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner331 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner332 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner333 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner334 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner335 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner336 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner337 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner338 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner339 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner340 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner341 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner342 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner343 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner344 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner345 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner346 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner347 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner348 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner349 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner350 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner351 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner352 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner353 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner354 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner355 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner356 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner357 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner358 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner359 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner360 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner361 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner362 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner363 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner364 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner365 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner366 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner367 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner368 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner369 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner370 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner371 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner372 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner373 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner374 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner375 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner376 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner377 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner378 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner379 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner380 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner381 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner382 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner383 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner384 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner385 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner386 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner387 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner388 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner389 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner390 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner391 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner392 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner393 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner394 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner395 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner396 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner397 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner398 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner399 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner400 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner401 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner402 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner403 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner404 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner405 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner406 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner407 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner408 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner409 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner410 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner411 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner412 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner413 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner414 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner415 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner416 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner417 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner418 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner419 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner420 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner421 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner422 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner423 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner424 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner425 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner426 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner427 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner428 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner429 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner430 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner431 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner432 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner433 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner434 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner435 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner436 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner437 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner438 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner439 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner440 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner441 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner442 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner443 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner444 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner445 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner446 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner447 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner448 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner449 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner450 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner451 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner452 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner453 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner454 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner455 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner456 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner457 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner458 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner459 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner460 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner461 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner462 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner463 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner464 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner465 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner466 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner467 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner468 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner469 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner470 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner471 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner472 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner473 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner474 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner475 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner476 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner477 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner478 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner479 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner480 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 32768), 0, PrecisionT::Precision{1, 23, 8} /*1[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner481 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*0[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner482 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner483 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner484 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner485 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner486 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner487 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner488 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner489 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner490 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner491 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner492 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner493 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner494 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner495 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner496 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner497 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner498 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner499 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner500 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner501 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner502 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner503 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner504 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner505 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner506 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner507 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner508 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner509 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  // cram-array axis
  {
    int32_t x_inner510 = 0;
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp((x.inner*512), 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowMul_CRAM_RF);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 65472), 0, PrecisionT::Precision{1, 23, 8} /*481[ramp(0, 1, 512)]*/);
      request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
      sys->sendRequest(request);
    }
    {
      Request request(Request::Type::RowAdd);
      request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
      request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
      sys->sendRequest(request);
    }
  }
  {
    Request request(Request::Type::RowMul_CRAM_RF);
    request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
    request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp(0, 1, 512)]*/);
    request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
    sys->sendRequest(request);
  }
  {
    Request request(Request::Type::RowMul_CRAM_RF);
    request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
    request.addOperand(sys->getAddress(0, 0, 67456), 0, PrecisionT::Precision{1, 23, 8} /*482[ramp(0, 1, 512)]*/);
    request.addOperand(0 * 32, 0, PrecisionT::Precision{1, 23, 8} /*ConstFloat*/);
    sys->sendRequest(request);
  }
  {
    Request request(Request::Type::RowAdd);
    request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
    request.addOperand(sys->getAddress(0, 0, 69376), 0, PrecisionT::Precision{1, 23, 8} /**/);
    request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /**/);
    sys->sendRequest(request);
  }
  {
    Request request(Request::Type::RowStore);
    request.addOperand(sys->getAddress(0, 0, 0), 0, PrecisionT::Precision{1, 23, 8} /*DRAM*/);
    request.addOperand(sys->getAddress(0, 0, 69440), 0, PrecisionT::Precision{1, 23, 8} /*CRAM*/);
    sys->sendRequest(request);
  }
  // freed 482
  // freed 481
  // freed 1
  // freed 0
  return 0;
}

static __attribute__((unused)) Registry::Entry &_gausselim_512__ = pimsim::registerFunc("gausselim_512", gausselim_512);

