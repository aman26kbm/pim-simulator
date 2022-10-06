#ifndef _REQSTATSENTRY_H_
#define _REQSTATSENTRY_H_

#include "Request.h"
#include "Status.h"
#include <array>
#define ENTRY_LENGTH  (int)status_t::MAX+10
namespace pimsim {

class ReqStatsEntry{
public:
    
    int reqNo = 0;
    Request::Type reqType = Request::Type::NOP;
    int op0 = 0;
    int op1 = 0;
    int op2 = 0;
    int bits = 0;
    int startTime=0;
    int endTime=0;
    std::array<long unsigned int, (int)status_t::MAX> cyclesInStates{};
    ReqStatsEntry(Request req, int startTime);
    static std::array<std::string, ENTRY_LENGTH> reqStatsHeader();
    std::array<long unsigned int, ENTRY_LENGTH> reqStatsValue();
};

}
#endif