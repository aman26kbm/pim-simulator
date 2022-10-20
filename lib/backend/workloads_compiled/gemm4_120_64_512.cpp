// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

#include "./tvm_common.h"

int32_t gemm4_120_64_512(System *sys) {
  void* _1 = nullptr;
  // int16_t compute_global_rf[1024], 0
  // int16_t compute_global[1024], 1024
  for (int32_t xo_outer = 0; xo_outer < 120; ++xo_outer) {
    for (int32_t y_inner = 0; y_inner < 32; ++y_inner) {
      // cram-array axis
      {
        int32_t rv_outer = 0;
        void* _2 = (void*) "compute.global.rf[ramp((rv.outer*512), 1, 512)] = x512((int16)0)/*skip-init*/";
        for (int32_t rv_inner = 0; rv_inner < 32; ++rv_inner) {
          int32_t _3 = y_inner * 2048;
          int32_t _4 = 0 * 32;
          int32_t _5 = _3 + _4;
          int32_t _6 = _5 + rv_inner;
          int32_t _7 = _6 % 256;
          bool _8 = _7 == 0;
          if (_8) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _9 = 1 * 32;
          int32_t _10 = _3 + _9;
          int32_t _11 = _10 + rv_inner;
          int32_t _12 = _11 % 256;
          bool _13 = _12 == 0;
          if (_13) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _14 = 2 * 32;
          int32_t _15 = _3 + _14;
          int32_t _16 = _15 + rv_inner;
          int32_t _17 = _16 % 256;
          bool _18 = _17 == 0;
          if (_18) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _19 = 3 * 32;
          int32_t _20 = _3 + _19;
          int32_t _21 = _20 + rv_inner;
          int32_t _22 = _21 % 256;
          bool _23 = _22 == 0;
          if (_23) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _24 = 4 * 32;
          int32_t _25 = _3 + _24;
          int32_t _26 = _25 + rv_inner;
          int32_t _27 = _26 % 256;
          bool _28 = _27 == 0;
          if (_28) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _29 = 5 * 32;
          int32_t _30 = _3 + _29;
          int32_t _31 = _30 + rv_inner;
          int32_t _32 = _31 % 256;
          bool _33 = _32 == 0;
          if (_33) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _34 = 6 * 32;
          int32_t _35 = _3 + _34;
          int32_t _36 = _35 + rv_inner;
          int32_t _37 = _36 % 256;
          bool _38 = _37 == 0;
          if (_38) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _39 = 7 * 32;
          int32_t _40 = _3 + _39;
          int32_t _41 = _40 + rv_inner;
          int32_t _42 = _41 % 256;
          bool _43 = _42 == 0;
          if (_43) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _44 = 8 * 32;
          int32_t _45 = _3 + _44;
          int32_t _46 = _45 + rv_inner;
          int32_t _47 = _46 % 256;
          bool _48 = _47 == 0;
          if (_48) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _49 = 9 * 32;
          int32_t _50 = _3 + _49;
          int32_t _51 = _50 + rv_inner;
          int32_t _52 = _51 % 256;
          bool _53 = _52 == 0;
          if (_53) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _54 = 10 * 32;
          int32_t _55 = _3 + _54;
          int32_t _56 = _55 + rv_inner;
          int32_t _57 = _56 % 256;
          bool _58 = _57 == 0;
          if (_58) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _59 = 11 * 32;
          int32_t _60 = _3 + _59;
          int32_t _61 = _60 + rv_inner;
          int32_t _62 = _61 % 256;
          bool _63 = _62 == 0;
          if (_63) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _64 = 12 * 32;
          int32_t _65 = _3 + _64;
          int32_t _66 = _65 + rv_inner;
          int32_t _67 = _66 % 256;
          bool _68 = _67 == 0;
          if (_68) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _69 = 13 * 32;
          int32_t _70 = _3 + _69;
          int32_t _71 = _70 + rv_inner;
          int32_t _72 = _71 % 256;
          bool _73 = _72 == 0;
          if (_73) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _74 = 14 * 32;
          int32_t _75 = _3 + _74;
          int32_t _76 = _75 + rv_inner;
          int32_t _77 = _76 % 256;
          bool _78 = _77 == 0;
          if (_78) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _79 = 15 * 32;
          int32_t _80 = _3 + _79;
          int32_t _81 = _80 + rv_inner;
          int32_t _82 = _81 % 256;
          bool _83 = _82 == 0;
          if (_83) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _84 = 16 * 32;
          int32_t _85 = _3 + _84;
          int32_t _86 = _85 + rv_inner;
          int32_t _87 = _86 % 256;
          bool _88 = _87 == 0;
          if (_88) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _89 = 17 * 32;
          int32_t _90 = _3 + _89;
          int32_t _91 = _90 + rv_inner;
          int32_t _92 = _91 % 256;
          bool _93 = _92 == 0;
          if (_93) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _94 = 18 * 32;
          int32_t _95 = _3 + _94;
          int32_t _96 = _95 + rv_inner;
          int32_t _97 = _96 % 256;
          bool _98 = _97 == 0;
          if (_98) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _99 = 19 * 32;
          int32_t _100 = _3 + _99;
          int32_t _101 = _100 + rv_inner;
          int32_t _102 = _101 % 256;
          bool _103 = _102 == 0;
          if (_103) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _104 = 20 * 32;
          int32_t _105 = _3 + _104;
          int32_t _106 = _105 + rv_inner;
          int32_t _107 = _106 % 256;
          bool _108 = _107 == 0;
          if (_108) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _109 = 21 * 32;
          int32_t _110 = _3 + _109;
          int32_t _111 = _110 + rv_inner;
          int32_t _112 = _111 % 256;
          bool _113 = _112 == 0;
          if (_113) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _114 = 22 * 32;
          int32_t _115 = _3 + _114;
          int32_t _116 = _115 + rv_inner;
          int32_t _117 = _116 % 256;
          bool _118 = _117 == 0;
          if (_118) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _119 = 23 * 32;
          int32_t _120 = _3 + _119;
          int32_t _121 = _120 + rv_inner;
          int32_t _122 = _121 % 256;
          bool _123 = _122 == 0;
          if (_123) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _124 = 24 * 32;
          int32_t _125 = _3 + _124;
          int32_t _126 = _125 + rv_inner;
          int32_t _127 = _126 % 256;
          bool _128 = _127 == 0;
          if (_128) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _129 = 25 * 32;
          int32_t _130 = _3 + _129;
          int32_t _131 = _130 + rv_inner;
          int32_t _132 = _131 % 256;
          bool _133 = _132 == 0;
          if (_133) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _134 = 26 * 32;
          int32_t _135 = _3 + _134;
          int32_t _136 = _135 + rv_inner;
          int32_t _137 = _136 % 256;
          bool _138 = _137 == 0;
          if (_138) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _139 = 27 * 32;
          int32_t _140 = _3 + _139;
          int32_t _141 = _140 + rv_inner;
          int32_t _142 = _141 % 256;
          bool _143 = _142 == 0;
          if (_143) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _144 = 28 * 32;
          int32_t _145 = _3 + _144;
          int32_t _146 = _145 + rv_inner;
          int32_t _147 = _146 % 256;
          bool _148 = _147 == 0;
          if (_148) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _149 = 29 * 32;
          int32_t _150 = _3 + _149;
          int32_t _151 = _150 + rv_inner;
          int32_t _152 = _151 % 256;
          bool _153 = _152 == 0;
          if (_153) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _154 = 30 * 32;
          int32_t _155 = _3 + _154;
          int32_t _156 = _155 + rv_inner;
          int32_t _157 = _156 % 256;
          bool _158 = _157 == 0;
          if (_158) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _159 = 31 * 32;
          int32_t _160 = _3 + _159;
          int32_t _161 = _160 + rv_inner;
          int32_t _162 = _161 % 256;
          bool _163 = _162 == 0;
          if (_163) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _164 = 32 * 32;
          int32_t _165 = _3 + _164;
          int32_t _166 = _165 + rv_inner;
          int32_t _167 = _166 % 256;
          bool _168 = _167 == 0;
          if (_168) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _169 = 33 * 32;
          int32_t _170 = _3 + _169;
          int32_t _171 = _170 + rv_inner;
          int32_t _172 = _171 % 256;
          bool _173 = _172 == 0;
          if (_173) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _174 = 34 * 32;
          int32_t _175 = _3 + _174;
          int32_t _176 = _175 + rv_inner;
          int32_t _177 = _176 % 256;
          bool _178 = _177 == 0;
          if (_178) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _179 = 35 * 32;
          int32_t _180 = _3 + _179;
          int32_t _181 = _180 + rv_inner;
          int32_t _182 = _181 % 256;
          bool _183 = _182 == 0;
          if (_183) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _184 = 36 * 32;
          int32_t _185 = _3 + _184;
          int32_t _186 = _185 + rv_inner;
          int32_t _187 = _186 % 256;
          bool _188 = _187 == 0;
          if (_188) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _189 = 37 * 32;
          int32_t _190 = _3 + _189;
          int32_t _191 = _190 + rv_inner;
          int32_t _192 = _191 % 256;
          bool _193 = _192 == 0;
          if (_193) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _194 = 38 * 32;
          int32_t _195 = _3 + _194;
          int32_t _196 = _195 + rv_inner;
          int32_t _197 = _196 % 256;
          bool _198 = _197 == 0;
          if (_198) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _199 = 39 * 32;
          int32_t _200 = _3 + _199;
          int32_t _201 = _200 + rv_inner;
          int32_t _202 = _201 % 256;
          bool _203 = _202 == 0;
          if (_203) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _204 = 40 * 32;
          int32_t _205 = _3 + _204;
          int32_t _206 = _205 + rv_inner;
          int32_t _207 = _206 % 256;
          bool _208 = _207 == 0;
          if (_208) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _209 = 41 * 32;
          int32_t _210 = _3 + _209;
          int32_t _211 = _210 + rv_inner;
          int32_t _212 = _211 % 256;
          bool _213 = _212 == 0;
          if (_213) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _214 = 42 * 32;
          int32_t _215 = _3 + _214;
          int32_t _216 = _215 + rv_inner;
          int32_t _217 = _216 % 256;
          bool _218 = _217 == 0;
          if (_218) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _219 = 43 * 32;
          int32_t _220 = _3 + _219;
          int32_t _221 = _220 + rv_inner;
          int32_t _222 = _221 % 256;
          bool _223 = _222 == 0;
          if (_223) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _224 = 44 * 32;
          int32_t _225 = _3 + _224;
          int32_t _226 = _225 + rv_inner;
          int32_t _227 = _226 % 256;
          bool _228 = _227 == 0;
          if (_228) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _229 = 45 * 32;
          int32_t _230 = _3 + _229;
          int32_t _231 = _230 + rv_inner;
          int32_t _232 = _231 % 256;
          bool _233 = _232 == 0;
          if (_233) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _234 = 46 * 32;
          int32_t _235 = _3 + _234;
          int32_t _236 = _235 + rv_inner;
          int32_t _237 = _236 % 256;
          bool _238 = _237 == 0;
          if (_238) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _239 = 47 * 32;
          int32_t _240 = _3 + _239;
          int32_t _241 = _240 + rv_inner;
          int32_t _242 = _241 % 256;
          bool _243 = _242 == 0;
          if (_243) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _244 = 48 * 32;
          int32_t _245 = _3 + _244;
          int32_t _246 = _245 + rv_inner;
          int32_t _247 = _246 % 256;
          bool _248 = _247 == 0;
          if (_248) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _249 = 49 * 32;
          int32_t _250 = _3 + _249;
          int32_t _251 = _250 + rv_inner;
          int32_t _252 = _251 % 256;
          bool _253 = _252 == 0;
          if (_253) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _254 = 50 * 32;
          int32_t _255 = _3 + _254;
          int32_t _256 = _255 + rv_inner;
          int32_t _257 = _256 % 256;
          bool _258 = _257 == 0;
          if (_258) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _259 = 51 * 32;
          int32_t _260 = _3 + _259;
          int32_t _261 = _260 + rv_inner;
          int32_t _262 = _261 % 256;
          bool _263 = _262 == 0;
          if (_263) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _264 = 52 * 32;
          int32_t _265 = _3 + _264;
          int32_t _266 = _265 + rv_inner;
          int32_t _267 = _266 % 256;
          bool _268 = _267 == 0;
          if (_268) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _269 = 53 * 32;
          int32_t _270 = _3 + _269;
          int32_t _271 = _270 + rv_inner;
          int32_t _272 = _271 % 256;
          bool _273 = _272 == 0;
          if (_273) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _274 = 54 * 32;
          int32_t _275 = _3 + _274;
          int32_t _276 = _275 + rv_inner;
          int32_t _277 = _276 % 256;
          bool _278 = _277 == 0;
          if (_278) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _279 = 55 * 32;
          int32_t _280 = _3 + _279;
          int32_t _281 = _280 + rv_inner;
          int32_t _282 = _281 % 256;
          bool _283 = _282 == 0;
          if (_283) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _284 = 56 * 32;
          int32_t _285 = _3 + _284;
          int32_t _286 = _285 + rv_inner;
          int32_t _287 = _286 % 256;
          bool _288 = _287 == 0;
          if (_288) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _289 = 57 * 32;
          int32_t _290 = _3 + _289;
          int32_t _291 = _290 + rv_inner;
          int32_t _292 = _291 % 256;
          bool _293 = _292 == 0;
          if (_293) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _294 = 58 * 32;
          int32_t _295 = _3 + _294;
          int32_t _296 = _295 + rv_inner;
          int32_t _297 = _296 % 256;
          bool _298 = _297 == 0;
          if (_298) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _299 = 59 * 32;
          int32_t _300 = _3 + _299;
          int32_t _301 = _300 + rv_inner;
          int32_t _302 = _301 % 256;
          bool _303 = _302 == 0;
          if (_303) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _304 = 60 * 32;
          int32_t _305 = _3 + _304;
          int32_t _306 = _305 + rv_inner;
          int32_t _307 = _306 % 256;
          bool _308 = _307 == 0;
          if (_308) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _309 = 61 * 32;
          int32_t _310 = _3 + _309;
          int32_t _311 = _310 + rv_inner;
          int32_t _312 = _311 % 256;
          bool _313 = _312 == 0;
          if (_313) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _314 = 62 * 32;
          int32_t _315 = _3 + _314;
          int32_t _316 = _315 + rv_inner;
          int32_t _317 = _316 % 256;
          bool _318 = _317 == 0;
          if (_318) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          int32_t _319 = 63 * 32;
          int32_t _320 = _3 + _319;
          int32_t _321 = _320 + rv_inner;
          int32_t _322 = _321 % 256;
          bool _323 = _322 == 0;
          if (_323) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
          if (_8) {
            {
              Request request(Request::Type::RowLoad_RF);
              request.addOperand(xo_outer * 32, 1, PrecisionT::Precision{0, 4, 0} /*RegisterFile*/);
              request.addOperand(sys->getAddress(xo_outer, 0, 0), 1, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
              sys->sendRequest(request);
            }
          }
                    {
            Request request(Request::Type::RowLoad);
            request.addOperand(sys->getAddress(xo_outer, 0, 0), 32768, PrecisionT::Precision{0, 4, 0} /*DRAM*/);
            request.addOperand(sys->getAddress(xo_outer, 0, 64), 32768, PrecisionT::Precision{0, 4, 0} /*a[ramp((((xo.outer*1048576) + (rv.outer*16384)) + (rv.inner*512)), 1, 512)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowMul_CRAM_RF);
            request.addOperand(sys->getAddress(xo_outer, 0, 80), 32768, PrecisionT::Precision{0, 8, 0} /**/);
            request.addOperand(sys->getAddress(xo_outer, 0, 64), 32768, PrecisionT::Precision{0, 4, 0} /*a[ramp((((xo.outer*1048576) + (rv.outer*16384)) + (rv.inner*512)), 1, 512)]*/);
            request.addOperand(xo_outer * 32, 32768, PrecisionT::Precision{0, 4, 0} /*b[(((y.inner*2048) + (rv.outer*32)) + rv.inner)]*/);
            sys->sendRequest(request);
          }
          {
            Request request(Request::Type::RowAdd);
            request.addOperand(sys->getAddress(xo_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(xo_outer, 0, 80), 32768, PrecisionT::Precision{0, 16, 0} /**/);
            request.addOperand(sys->getAddress(xo_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer*512), 1, 512)]*/);
            sys->sendRequest(request);
          }
        }
      }
      void* _325 = (void*) "compute.global[ramp(0, 1, 512)] = x512((int16)0)/*skip-init*/";
      #define max(a,b) ((a)>(b)?(a):(b))
      for (int32_t rv_outer_v = 64, rv_outer_v_cnt = 1; rv_outer_v >= 1; rv_outer_v -= max(rv_outer_v / 2, 1), ++rv_outer_v_cnt) {
      #undef max
        {
          Request request(Request::Type::BlockSend_Receive);
          request.addOperand(sys->getAddress(xo_outer, 0, 32), 32768, PrecisionT::Precision{0, 16, 0} /*compute.global[ramp(0, 1, 512)]*/);
          request.addOperand(sys->getAddress(xo_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer.v*512), 1, 512)]*/);
          sys->sendRequest(request);
        }
        {
          Request request(Request::Type::RowAdd);
          request.addOperand(sys->getAddress(xo_outer, 0, 32), 32768, PrecisionT::Precision{0, 16, 0} /**/);
          request.addOperand(sys->getAddress(xo_outer, 0, 32), 32768, PrecisionT::Precision{0, 16, 0} /*compute.global[ramp(0, 1, 512)]*/);
          request.addOperand(sys->getAddress(xo_outer, 0, 0), 32768, PrecisionT::Precision{0, 16, 0} /*compute.global.rf[ramp((rv.outer.v*512), 1, 512)]*/);
          sys->sendRequest(request);
        }
      }
      {
        Request request(Request::Type::RowStore);
        request.addOperand(sys->getAddress(xo_outer, 0, 0), 512, PrecisionT::Precision{0, 16, 0} /*DRAM*/);
        request.addOperand(sys->getAddress(xo_outer, 0, 32), 512, PrecisionT::Precision{0, 16, 0} /*CRAM*/);
        sys->sendRequest(request);
      }
    }
  }
  // freed compute_global
  // freed compute_global_rf
  return 0;
}

static __attribute__((unused)) Registry::Entry &_gemm4_120_64_512__ = pimsim::registerFunc("gemm4_120_64_512", gemm4_120_64_512);

