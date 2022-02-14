#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <list>
#include <iostream>

#include "Request.h"

namespace pimsim {

class Request;

class Scheduler {
public:
    enum class Strategy {
        Naive,
        Read,
        MAX
    } strategy;
    int level = -1; // 0: chip, 1: tile, 2: block
    
    Scheduler() {}
    Scheduler(Strategy s) : strategy(s) {}
    std::list<Request>::iterator getNext(std::list<Request>& q);
};

}
#endif
