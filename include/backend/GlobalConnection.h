#ifndef _GLOBAL_CONNECTION_H_
#define _GLOBAL_CONNECTION_H_

#include "util.h"
#include <vector>
#include <queue>
#include <deque>
#include <set>

#include "Message.h"
#include "System.h"

#define NCHIPS 16

namespace pimsim {
    template <class T>
    class System;
 
struct conn_stat {
    std::set<int> direct, special;
    int direct_latency, special_latency, bus_latency; 
};

class GlobalConnection {
private:
    std::queue<Message> _msg_q;
    System<float> *_sys;
    int network_bandwidth = 480; // GB/s -> B/ns
    int n_hops[NCHIPS][NCHIPS];
    /* Network statistics */
    uint64_t n_reqs[NCHIPS][NCHIPS];
    double send_bandwidth[NCHIPS], receive_bandwidth[NCHIPS], wait_time[NCHIPS];
public:
    enum class Type : int {
        Ideal,
        Mesh,
        Dragonfly,
        MAX
    } _type;

    GlobalConnection(System<float > *, Type type);
    int getLatency(int c1, int c2, int size);
    int group_hops(int router1, int router2, int src, int dst);
    void issueNetReq(int c1, int c2, int size, int tick1, int tick2, int latency);
    void outputStat(FILE* file);
};

}

#endif
