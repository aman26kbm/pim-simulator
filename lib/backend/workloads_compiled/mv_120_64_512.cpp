// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t mv_120_64_512(System *sys) {
  void* _1 = nullptr;
  // int32_t c_global_rf[2048], 0
  // int8_t a_global[512], 2048
  // int32_t c_global[2048], 2560
  for (int32_t x_outer_outer = 0; x_outer_outer < 120; ++x_outer_outer) {
    // cram-array axis
    {
      int32_t k_outer = 0;
      void* _2 = (void*) "c.global.rf[ramp((k.outer*512), 1, 512)] = x512(0)/*skip-init*/";
      for (int32_t k_inner = 0; k_inner < 32; ++k_inner) {
        {
          Request request(Request::Type::RowLoad);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 64), 32768, PrecisionT::Precision{0, 8, 0} /*CRAM*/);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 32768, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
          sys->sendRequest(request);
        }
        int32_t _3 = 0 * 32;
        int32_t _4 = _3 + k_inner;
        int32_t _5 = _4 % 128;
        bool _6 = _5 == 0;
        if (_6) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _7 = 1 * 32;
        int32_t _8 = _7 + k_inner;
        int32_t _9 = _8 % 128;
        bool _10 = _9 == 0;
        if (_10) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _11 = 2 * 32;
        int32_t _12 = _11 + k_inner;
        int32_t _13 = _12 % 128;
        bool _14 = _13 == 0;
        if (_14) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _15 = 3 * 32;
        int32_t _16 = _15 + k_inner;
        int32_t _17 = _16 % 128;
        bool _18 = _17 == 0;
        if (_18) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _19 = 4 * 32;
        int32_t _20 = _19 + k_inner;
        int32_t _21 = _20 % 128;
        bool _22 = _21 == 0;
        if (_22) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _23 = 5 * 32;
        int32_t _24 = _23 + k_inner;
        int32_t _25 = _24 % 128;
        bool _26 = _25 == 0;
        if (_26) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _27 = 6 * 32;
        int32_t _28 = _27 + k_inner;
        int32_t _29 = _28 % 128;
        bool _30 = _29 == 0;
        if (_30) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _31 = 7 * 32;
        int32_t _32 = _31 + k_inner;
        int32_t _33 = _32 % 128;
        bool _34 = _33 == 0;
        if (_34) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _35 = 8 * 32;
        int32_t _36 = _35 + k_inner;
        int32_t _37 = _36 % 128;
        bool _38 = _37 == 0;
        if (_38) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _39 = 9 * 32;
        int32_t _40 = _39 + k_inner;
        int32_t _41 = _40 % 128;
        bool _42 = _41 == 0;
        if (_42) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _43 = 10 * 32;
        int32_t _44 = _43 + k_inner;
        int32_t _45 = _44 % 128;
        bool _46 = _45 == 0;
        if (_46) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _47 = 11 * 32;
        int32_t _48 = _47 + k_inner;
        int32_t _49 = _48 % 128;
        bool _50 = _49 == 0;
        if (_50) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _51 = 12 * 32;
        int32_t _52 = _51 + k_inner;
        int32_t _53 = _52 % 128;
        bool _54 = _53 == 0;
        if (_54) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _55 = 13 * 32;
        int32_t _56 = _55 + k_inner;
        int32_t _57 = _56 % 128;
        bool _58 = _57 == 0;
        if (_58) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _59 = 14 * 32;
        int32_t _60 = _59 + k_inner;
        int32_t _61 = _60 % 128;
        bool _62 = _61 == 0;
        if (_62) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _63 = 15 * 32;
        int32_t _64 = _63 + k_inner;
        int32_t _65 = _64 % 128;
        bool _66 = _65 == 0;
        if (_66) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _67 = 16 * 32;
        int32_t _68 = _67 + k_inner;
        int32_t _69 = _68 % 128;
        bool _70 = _69 == 0;
        if (_70) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _71 = 17 * 32;
        int32_t _72 = _71 + k_inner;
        int32_t _73 = _72 % 128;
        bool _74 = _73 == 0;
        if (_74) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _75 = 18 * 32;
        int32_t _76 = _75 + k_inner;
        int32_t _77 = _76 % 128;
        bool _78 = _77 == 0;
        if (_78) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _79 = 19 * 32;
        int32_t _80 = _79 + k_inner;
        int32_t _81 = _80 % 128;
        bool _82 = _81 == 0;
        if (_82) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _83 = 20 * 32;
        int32_t _84 = _83 + k_inner;
        int32_t _85 = _84 % 128;
        bool _86 = _85 == 0;
        if (_86) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _87 = 21 * 32;
        int32_t _88 = _87 + k_inner;
        int32_t _89 = _88 % 128;
        bool _90 = _89 == 0;
        if (_90) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _91 = 22 * 32;
        int32_t _92 = _91 + k_inner;
        int32_t _93 = _92 % 128;
        bool _94 = _93 == 0;
        if (_94) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _95 = 23 * 32;
        int32_t _96 = _95 + k_inner;
        int32_t _97 = _96 % 128;
        bool _98 = _97 == 0;
        if (_98) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _99 = 24 * 32;
        int32_t _100 = _99 + k_inner;
        int32_t _101 = _100 % 128;
        bool _102 = _101 == 0;
        if (_102) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _103 = 25 * 32;
        int32_t _104 = _103 + k_inner;
        int32_t _105 = _104 % 128;
        bool _106 = _105 == 0;
        if (_106) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _107 = 26 * 32;
        int32_t _108 = _107 + k_inner;
        int32_t _109 = _108 % 128;
        bool _110 = _109 == 0;
        if (_110) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _111 = 27 * 32;
        int32_t _112 = _111 + k_inner;
        int32_t _113 = _112 % 128;
        bool _114 = _113 == 0;
        if (_114) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _115 = 28 * 32;
        int32_t _116 = _115 + k_inner;
        int32_t _117 = _116 % 128;
        bool _118 = _117 == 0;
        if (_118) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _119 = 29 * 32;
        int32_t _120 = _119 + k_inner;
        int32_t _121 = _120 % 128;
        bool _122 = _121 == 0;
        if (_122) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _123 = 30 * 32;
        int32_t _124 = _123 + k_inner;
        int32_t _125 = _124 % 128;
        bool _126 = _125 == 0;
        if (_126) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _127 = 31 * 32;
        int32_t _128 = _127 + k_inner;
        int32_t _129 = _128 % 128;
        bool _130 = _129 == 0;
        if (_130) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _131 = 32 * 32;
        int32_t _132 = _131 + k_inner;
        int32_t _133 = _132 % 128;
        bool _134 = _133 == 0;
        if (_134) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _135 = 33 * 32;
        int32_t _136 = _135 + k_inner;
        int32_t _137 = _136 % 128;
        bool _138 = _137 == 0;
        if (_138) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _139 = 34 * 32;
        int32_t _140 = _139 + k_inner;
        int32_t _141 = _140 % 128;
        bool _142 = _141 == 0;
        if (_142) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _143 = 35 * 32;
        int32_t _144 = _143 + k_inner;
        int32_t _145 = _144 % 128;
        bool _146 = _145 == 0;
        if (_146) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _147 = 36 * 32;
        int32_t _148 = _147 + k_inner;
        int32_t _149 = _148 % 128;
        bool _150 = _149 == 0;
        if (_150) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _151 = 37 * 32;
        int32_t _152 = _151 + k_inner;
        int32_t _153 = _152 % 128;
        bool _154 = _153 == 0;
        if (_154) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _155 = 38 * 32;
        int32_t _156 = _155 + k_inner;
        int32_t _157 = _156 % 128;
        bool _158 = _157 == 0;
        if (_158) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _159 = 39 * 32;
        int32_t _160 = _159 + k_inner;
        int32_t _161 = _160 % 128;
        bool _162 = _161 == 0;
        if (_162) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _163 = 40 * 32;
        int32_t _164 = _163 + k_inner;
        int32_t _165 = _164 % 128;
        bool _166 = _165 == 0;
        if (_166) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _167 = 41 * 32;
        int32_t _168 = _167 + k_inner;
        int32_t _169 = _168 % 128;
        bool _170 = _169 == 0;
        if (_170) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _171 = 42 * 32;
        int32_t _172 = _171 + k_inner;
        int32_t _173 = _172 % 128;
        bool _174 = _173 == 0;
        if (_174) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _175 = 43 * 32;
        int32_t _176 = _175 + k_inner;
        int32_t _177 = _176 % 128;
        bool _178 = _177 == 0;
        if (_178) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _179 = 44 * 32;
        int32_t _180 = _179 + k_inner;
        int32_t _181 = _180 % 128;
        bool _182 = _181 == 0;
        if (_182) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _183 = 45 * 32;
        int32_t _184 = _183 + k_inner;
        int32_t _185 = _184 % 128;
        bool _186 = _185 == 0;
        if (_186) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _187 = 46 * 32;
        int32_t _188 = _187 + k_inner;
        int32_t _189 = _188 % 128;
        bool _190 = _189 == 0;
        if (_190) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _191 = 47 * 32;
        int32_t _192 = _191 + k_inner;
        int32_t _193 = _192 % 128;
        bool _194 = _193 == 0;
        if (_194) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _195 = 48 * 32;
        int32_t _196 = _195 + k_inner;
        int32_t _197 = _196 % 128;
        bool _198 = _197 == 0;
        if (_198) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _199 = 49 * 32;
        int32_t _200 = _199 + k_inner;
        int32_t _201 = _200 % 128;
        bool _202 = _201 == 0;
        if (_202) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _203 = 50 * 32;
        int32_t _204 = _203 + k_inner;
        int32_t _205 = _204 % 128;
        bool _206 = _205 == 0;
        if (_206) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _207 = 51 * 32;
        int32_t _208 = _207 + k_inner;
        int32_t _209 = _208 % 128;
        bool _210 = _209 == 0;
        if (_210) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _211 = 52 * 32;
        int32_t _212 = _211 + k_inner;
        int32_t _213 = _212 % 128;
        bool _214 = _213 == 0;
        if (_214) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _215 = 53 * 32;
        int32_t _216 = _215 + k_inner;
        int32_t _217 = _216 % 128;
        bool _218 = _217 == 0;
        if (_218) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _219 = 54 * 32;
        int32_t _220 = _219 + k_inner;
        int32_t _221 = _220 % 128;
        bool _222 = _221 == 0;
        if (_222) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _223 = 55 * 32;
        int32_t _224 = _223 + k_inner;
        int32_t _225 = _224 % 128;
        bool _226 = _225 == 0;
        if (_226) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _227 = 56 * 32;
        int32_t _228 = _227 + k_inner;
        int32_t _229 = _228 % 128;
        bool _230 = _229 == 0;
        if (_230) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _231 = 57 * 32;
        int32_t _232 = _231 + k_inner;
        int32_t _233 = _232 % 128;
        bool _234 = _233 == 0;
        if (_234) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _235 = 58 * 32;
        int32_t _236 = _235 + k_inner;
        int32_t _237 = _236 % 128;
        bool _238 = _237 == 0;
        if (_238) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _239 = 59 * 32;
        int32_t _240 = _239 + k_inner;
        int32_t _241 = _240 % 128;
        bool _242 = _241 == 0;
        if (_242) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _243 = 60 * 32;
        int32_t _244 = _243 + k_inner;
        int32_t _245 = _244 % 128;
        bool _246 = _245 == 0;
        if (_246) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _247 = 61 * 32;
        int32_t _248 = _247 + k_inner;
        int32_t _249 = _248 % 128;
        bool _250 = _249 == 0;
        if (_250) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _251 = 62 * 32;
        int32_t _252 = _251 + k_inner;
        int32_t _253 = _252 % 128;
        bool _254 = _253 == 0;
        if (_254) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
        int32_t _255 = 63 * 32;
        int32_t _256 = _255 + k_inner;
        int32_t _257 = _256 % 128;
        bool _258 = _257 == 0;
        if (_258) {
          {
            Request request(Request::Type::RowLoad_RF);
            request.addOperand(x_outer_outer * 32, 1, PrecisionT::Precision{0, 8, 0} /*RegisterFile*/);
            request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 1, PrecisionT::Precision{0, 8, 0} /*DRAM*/);
            sys->sendRequest(request);
          }
        }
                {
          Request request(Request::Type::RowMul_CRAM_RF);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 144), 32768, PrecisionT::Precision{0, 16, 0} /**/);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 64), 32768, PrecisionT::Precision{0, 8, 0} /*a.global[ramp(0, 1, 512)]*/);
          request.addOperand(x_outer_outer * 32, 32768, PrecisionT::Precision{0, 8, 0} /*b[((k.outer*32) + k.inner)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 32768, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 144), 32768, PrecisionT::Precision{0, 32, 0} /**/);
          request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 32768, PrecisionT::Precision{0, 32, 0} /*c.global.rf[ramp((k.outer*512), 1, 512)]*/);
          sys->sendRequest(request);
        }
      }
    }
    void* _260 = (void*) "c.global[ramp(0, 1, 512)] = x512(0)/*skip-init*/";
    #define max(a,b) ((a)>(b)?(a):(b))
    for (int32_t k_outer_v = 64, k_outer_v_cnt = 1; k_outer_v >= 1; k_outer_v -= max(k_outer_v / 2, 1), ++k_outer_v_cnt) {
    #undef max
      {
        Request request(Request::Type::BlockSend_Receive);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 80), 32768, PrecisionT::Precision{0, 32, 0} /*c.global[ramp(0, 1, 512)]*/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 32768, PrecisionT::Precision{0, 32, 0} /*c.global.rf[ramp((k.outer.v*512), 1, 512)]*/);
        sys->sendRequest(request);
      }
      {
        Request request(Request::Type::RowAdd);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 80), 32768, PrecisionT::Precision{0, 32, 0} /**/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 80), 32768, PrecisionT::Precision{0, 32, 0} /*c.global[ramp(0, 1, 512)]*/);
        request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 32768, PrecisionT::Precision{0, 32, 0} /*c.global.rf[ramp((k.outer.v*512), 1, 512)]*/);
        sys->sendRequest(request);
      }
    }
    {
      Request request(Request::Type::RowStore);
      request.addOperand(sys->getAddress(x_outer_outer, 0, 0), 512, PrecisionT::Precision{0, 32, 0} /*DRAM*/);
      request.addOperand(sys->getAddress(x_outer_outer, 0, 80), 512, PrecisionT::Precision{0, 32, 0} /*CRAM*/);
      sys->sendRequest(request);
    }
  }
  // freed c_global
  // freed a_global
  // freed c_global_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_mv_120_64_512__ = pimsim::registerFunc("mv_120_64_512", mv_120_64_512);

