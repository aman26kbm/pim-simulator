#ifndef _UTIL_H_
#define _UTIL_H_

#include <cstdio>
#include <cstdint>
#include <list>

namespace pimsim {

typedef uint64_t TimeT;
typedef uint64_t CountT;
typedef uint64_t AddrT;

enum PrecisionT {
    FP8_e3m4,
    BF16_e8m7,
    FP16_e5m10,
    FP32_e8m23,
    INT4,
    INT8,
    INT16,
    INT32,
    MAX
};

}

//#define BACK_DEBUG_OUTPUT
//#define FRONT_DEBUG_OUTPUT
#define APP_DEBUG_OUTPUT
//#define SCHED_DEBUG_OUTPUT
//#define NET_DEBUG_OUTPUT
#define ALLOC_DEBUG_OUTPUT

#endif
