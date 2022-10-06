#ifndef _REQSTATSENTRY_H_
#define _REQSTATSENTRY_H_

#include "Request.h"
#include "Status.h"
#include <array>

namespace pimsim {

class ReqStatsEntry{
public:
    int reqNo = -1;
    Request::Type reqType = Request::Type::NOP;
    int srcTile = -1;
    int destTile = -1;
    int bits = -1;
    int startTime=-1;
    int endTime=-1;
    std::array<long unsigned int, (int)status_t::MAX> cyclesInStates;
    ReqStatsEntry(Request req, int startTime);
    ReqStatsEntry();
};

}
#endif