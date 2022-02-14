#include "backend/Scheduler.h"

#include <list>
#include <iostream>

using namespace std;
using namespace pimsim;

list<Request>::iterator
Scheduler::getNext(list<Request>& q)
{
    auto head = q.begin();
    if (!q.size())
        return q.end();
    if (strategy == Strategy::Naive) { // just choose the earliest request
        for (auto it = next(q.begin(), 1); it != q.end(); it++) {
            if (head->arrive_time > it->arrive_time) {
                head = it;
            }
        }
        return head;
    } else if (strategy == Strategy::Read) {
    } else {
    }
    return head;
}
