#ifndef _UTIL_H_
#define _UTIL_H_

#include <cstdio>
#include <cstdint>
#include <list>

namespace pimsim {

typedef uint64_t TimeT;
typedef uint64_t CountT;
typedef uint64_t AddrT;

// enum PrecisionT {
//     FP8_e3m4,
//     BF16_e8m7,
//     FP16_e5m10,
//     FP32_e8m23,
//     INT4,
//     INT8,
//     INT16,
//     INT32,
//     MAX
// };
namespace PrecisionT {
struct Precision {
    bool isfloat;
    int mantissa; //also specifies the width for integers
    int exponent; //0 for integers
    //constructor and other APIs
    bool operator==(Precision a)
    {
        return (isfloat == a.isfloat && mantissa == a.mantissa && exponent == a.exponent);
    }
    int bits(){
        if(isfloat) return mantissa + exponent + 1;
        else return mantissa;
    }
};
const Precision FP8_e3m4 = Precision{1, 4, 3};
const Precision BF16_e8m7 = Precision{1, 8, 7};
const Precision FP16_e5m10 = Precision{1, 10, 5};
const Precision FP32_e8m23 = Precision{1, 23, 8};
const Precision INT4 = Precision{0, 4, 0};
const Precision INT8 = Precision{0, 8, 0};
const Precision INT16 = Precision{0, 16, 0};
const Precision INT32 = Precision{0, 32, 0};
}

}

//#define BACK_DEBUG_OUTPUT
//#define FRONT_DEBUG_OUTPUT
#define APP_DEBUG_OUTPUT
//#define SCHED_DEBUG_OUTPUT
//#define NET_DEBUG_OUTPUT
#define ALLOC_DEBUG_OUTPUT

#endif
