#ifndef _DYNA_SWITCH_H_
#define _DYNA_SWITCH_H_

#define _ROUTER_DEBUG_OUTPUT_

#include <assert.h>
#include "Request.h"
#include "Config.h"

namespace pimsim {

template <typename T, int MaxLen>
class FixedQueue : public std::queue<T, std::deque<T>> {
public:
    void push(const T& value);
    bool is_full();

};

//N=North, S=South, W=West, E=East, L=Local
enum Direction {N,S,W,E,L,BOUND};
Direction operator ++ (Direction& d, int);
std::string toString(Direction d);

enum ConnectState {
    toN,
    toS,
    toW,
    toE,
    toL,
    IDLE
};
std::string toString(ConnectState d);

class DynaSwitch{
public:
    int index;
    int myRow;
    int myCol;
    Config* cfg;

    int bitwidth;
    //receive queues
    // receiveQueues[0]: FixedQueue<Request*,2> queueN;
    // receiveQueues[1]: FixedQueue<Request*,2> queueS;
    // receiveQueues[2]: FixedQueue<Request*,2> queueW;
    // receiveQueues[3]: FixedQueue<Request*,2> queueE;
    // receiveQueues[4]: FixedQueue<Request*,2> queueL;
    FixedQueue<Request*,2>* receiveQueues;

    //output port that an input port is connected to by crossbar
    // ConnectStates[0]: connectStateN;
    // ConnectStates[1]: connectStateS;
    // ConnectStates[2]: connectStateW;
    // ConnectStates[3]: connectStateE;
    // ConnectStates[4]: connectStateL;
    ConnectState* connectStates;

    //number of packets left to send to output port in the current request
    // packetsRemaining[0]: int packets2N;
    // packetsRemaining[1]: int packets2S;
    // packetsRemaining[2]: int packets2W;
    // packetsRemaining[3]: int packets2E;
    // packetsRemaining[4]: int packets2L;
    int* packetsRemaining;

    //assume each router has a receive buffer that is *infinitely large*
    std::vector<Request*> localReceiveBuffer;

    DynaSwitch* neighborN;
    DynaSwitch* neighborS;
    DynaSwitch* neighborW;
    DynaSwitch* neighborE;

    DynaSwitch(int index, Config* cfg);
    
    bool inject(Request* req);
    void tick();
    //given a receive/load request, find if the requested data is present in local receive buffer
    bool data_exist(Request* req);
    bool pop_data(Request* req);

    void print_my_status();
    void print_receive_Queues();
    void print_connection();
    void print_remaining_packets();
    void print_local_receive_buffer();

    //utils
private:
    int get_addr0_index(Request* req);
    int get_addr1_index(Request* req);
    int get_source_index(Request* req);
    int get_dest_index(Request* req);
    Direction decode(Request* req);
    void setupConnection(Direction in, Direction out, int packets);
    bool neighborIsFull(Direction direction);
    void push2Neighbor(Request* req, Direction direction);
    bool inputShouldSend(Direction in);
    void inputSend(Direction in);
    bool isMatch(Request* bufferedReq, Request* ReceiveReq);
    

};

}

#endif