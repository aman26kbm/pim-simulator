#include "backend/ReqStatsEntry.h"

using namespace pimsim;

ReqStatsEntry::ReqStatsEntry(Request req, int startTime){
        this->reqNo = req.reqNo;
        this->bits = req.bits;
        this->startTime = startTime;
}

ReqStatsEntry::ReqStatsEntry(){}