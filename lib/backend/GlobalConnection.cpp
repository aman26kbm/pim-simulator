#include "backend/GlobalConnection.h"
#include <cstdlib>

using namespace pimsim;
using namespace std;

GlobalConnection::GlobalConnection(System<float >* sys, Type type)
{
    _sys = sys;
    _type = type;
    for (int i = 0; i < NCHIPS; i++) {
        send_bandwidth[i] = 0;
        receive_bandwidth[i] = 0;
        wait_time[i] = 0;
        for (int j = 0; j < NCHIPS; j++) {
            n_reqs[i][j] = 0;
        }
    }
    // TODO: current only support 16 chip scheme
    if (_type == Type::Ideal) {
#ifdef NET_DEBUG_OUTPUT
        cout << "here is a Ideal network!\n";
#endif
        for (int i = 0; i < NCHIPS; i++) {
            for (int j = 0; j < NCHIPS; j++) {
                if (i != j)
                    n_hops[i][j] = 1;
                else
                    n_hops[i][j] = 0;
            }
        } 
    } else if (_type == Type::Mesh) {
#ifdef NET_DEBUG_OUTPUT
        cout << "here is a Mesh network!\n";
#endif
        for (int i = 0; i < NCHIPS; i++) {
            for (int j = 0; j < NCHIPS; j++) {
                int ir = i / 4, ic = i % 4,
                    jr = j / 4, jc = j % 4;
                n_hops[i][j] = abs(ir - jr) + abs(ic - jc);
            }
        }
    } else if (_type == Type::Dragonfly) {
#ifdef NET_DEBUG_OUTPUT
        cout << "here is a Dragonfly network!\n";
#endif
        vector<int> group1{0,1,4,5},
                    group2{2,3,6,7},
                    group3{8,9,12,13},
                    group4{10,11,14,15};
        // within a group
        for (int i : group1)
            for (int j : group1)
                if (i != j)
                    n_hops[i][j] = 1;
                else
                    n_hops[i][j] = 0;
        for (int i : group2)
            for (int j : group2)
                if (i != j)
                    n_hops[i][j] = 1;
                else
                    n_hops[i][j] = 0;
        for (int i : group3)
            for (int j : group3)
                if (i != j)
                    n_hops[i][j] = 1;
                else
                    n_hops[i][j] = 0;
        for (int i : group4)
            for (int j : group4)
                if (i != j)
                    n_hops[i][j] = 1;
                else
                    n_hops[i][j] = 0;

        // Cross-group
        for (int i : group1)
            for (int j : group2) {
                n_hops[i][j] = group_hops(1, 2, i, j);
                n_hops[j][i] = group_hops(2, 1, j, i);
            }
        for (int i : group1)
            for (int j : group3) {
                n_hops[i][j] = group_hops(4, 8, i, j);
                n_hops[j][i] = group_hops(8, 4, j, i);
            }
        for (int i : group1)
            for (int j : group4) {
                n_hops[i][j] = group_hops(5, 10, i, j);
                n_hops[j][i] = group_hops(10, 5, j, i);
            }
        for (int i : group2)
            for (int j : group3) {
                n_hops[i][j] = group_hops(6, 9, i, j);
                n_hops[j][i] = group_hops(9, 6, j, i);
            }
        for (int i : group2)
            for (int j : group4) {
                n_hops[i][j] = group_hops(7, 11, i, j);
                n_hops[j][i] = group_hops(11, 7, j, i);
            }
        for (int i : group3)
            for (int j : group4) {
                n_hops[i][j] = group_hops(13, 14, i, j);
                n_hops[j][i] = group_hops(14, 13, j, i);
            }
    }
}

int
GlobalConnection::group_hops(int router1, int router2, int src, int dst) {
    int max = 3;
    if (src == router1)
        max--;
    if (dst == router2)
        max--;
    return max;
}

int
GlobalConnection::getLatency(int c1, int c2, int size)
{
    double latency = 0;
    latency = (((size - 1) / network_bandwidth) + 1) * n_hops[c1][c2]; 
    double fclk = latency / (1000. / double(_sys->_clock_rate));
    int clk = int(fclk+1);
#ifdef NET_DEBUG_OUTPUT
    printf("Src(%d) - Dst(%d) Size(%d): Hops(%d), Latency(%.4lf), Clk(%d)\n",
            c1, c2, size, n_hops[c1][c2], latency, clk);
#endif
    return clk;
}

void
GlobalConnection::issueNetReq(int c1, int c2, int size, int tick1, int tick2, int latency) {
    n_reqs[c1][c2] ++;
    send_bandwidth[c1] += size; 
    receive_bandwidth[c2] += size;
    wait_time[c1] += tick1;
    wait_time[c2] += tick2 + latency;
}

void 
GlobalConnection::outputStat(FILE* file) {
    for (int chip = 0; chip < NCHIPS; chip++) {
        uint64_t n_sends = 0, n_receives = 0;
        for (int cc = 0; cc < NCHIPS; cc++) {
            n_sends += n_reqs[chip][cc];
            n_receives += n_reqs[cc][chip];
        }
        if (n_sends != 0)
            send_bandwidth[chip] /= double(n_sends);
        if (n_receives != 0)
            receive_bandwidth[chip] /= double(n_receives);
        if (n_sends + n_receives != 0)
            wait_time[chip] /= double(n_sends + n_receives);
        fprintf(file, "NETWORK_CP#%d - #send: %lu, #receive: %lu, send_bandwidth: %.4lf, receive_bandwidth: %.4lf, wait_time: %.4lf\n",
                chip, n_sends, n_receives, 
                send_bandwidth[chip], receive_bandwidth[chip],
                wait_time[chip]);
    }
}
