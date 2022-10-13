#ifndef _DYNA_SWITCH_H_
#define _DYNA_SWITCH_H_

//#define _ROUTER_DEBUG_OUTPUT_

#include <assert.h>
#include "Request.h"
#include "Config.h"
#include "Dram_sendback.h"
#include "DataStructs.h"

namespace pimsim {

//input directions: N=North, S=South, W=West, E=East, L=Local, D=Dram
enum Direction {D,L,N,S,W,E,BOUND};
Direction operator ++ (Direction& d, int);
std::string toString(Direction d);

enum ConnectState {
    toD,
    toL,
    toN,
    toS,
    toW,
    toE,
    IDLE
};
std::string toString(ConnectState d);

class DynaSwitch{
public:
    //dram attached to this router
    Dram_sendback* dram;

    int index;
    int myRow;
    int myCol;
    Config* cfg;

    int bitwidth;

    int remainingStore=0;
    //receive queues
    // receiveQueues[0]: FixedQueue<Request*,2> queueN;
    // receiveQueues[1]: FixedQueue<Request*,2> queueS;
    // receiveQueues[2]: FixedQueue<Request*,2> queueW;
    // receiveQueues[3]: FixedQueue<Request*,2> queueE;
    // receiveQueues[4]: FixedQueue<Request*,2> queueL;
    // receiveQueues[5]: FixedQueue<Request*,INT32_MAX> queueD;
    std::vector< FixedQueue<Request> > receiveQueues;


    //output port that an input port is connected to by crossbar
    // ConnectStates[0]: connectStateN;
    // ConnectStates[1]: connectStateS;
    // ConnectStates[2]: connectStateW;
    // ConnectStates[3]: connectStateE;
    // ConnectStates[4]: connectStateL;
    // ConnectStates[5]: connectStateDram;
    std::vector< ConnectState > connectStates;

    //number of packets left to send to output port in the current request
    // packetsRemaining[0]: int packets2N;
    // packetsRemaining[1]: int packets2S;
    // packetsRemaining[2]: int packets2W;
    // packetsRemaining[3]: int packets2E;
    // packetsRemaining[4]: int packets2L;
    // packetsRemaining[5]: int packets2D;
    std::vector< int > packetsRemaining;
    std::vector<bool> connected;

    //assume each router has a receive buffer that is *infinitely large*
    std::vector<Request> localReceiveBuffer;
    //buffers all dram requests
    FixedQueue<Request> dramReceiveBuffer;

    DynaSwitch* neighborN;
    DynaSwitch* neighborS;
    DynaSwitch* neighborW;
    DynaSwitch* neighborE;

    DynaSwitch();
    DynaSwitch(int index, Config* cfg);
    void copy_content(const DynaSwitch* src, DynaSwitch* tgt);
    
    bool inject(Request req);
    bool receive_from_dram(Request req);
    //bool store_to_dram(Request req);
    void tick();
    //given a receive/load request, find if the requested data is present in local receive buffer
    bool data_exist(Request req);
    Request pop_data(Request req);

    void print_my_status();
    void print_receive_Queues();
    void print_connection();
    void print_remaining_packets();
    void print_local_receive_buffer();
    void print_dram_receive_buffer();

    //the next state of this switch
    DynaSwitch* next= NULL;
    void update_current();

    bool is_finished();

    //utils
private:
    int get_addr0_index(Request req);
    int get_addr1_index(Request req);
    int get_source_index(Request req);
    int get_dest_index(Request req);
    Direction decode(Request req);
    void setupConnection(Direction in, Direction out, int packets);
    bool neighborIsFull(Direction direction);
    void push2Neighbor(Request req, Direction direction);
    bool inputShouldSend(Direction in);
    void inputSend(Direction in);
    bool isMatch(Request bufferedReq, Request ReceiveReq);
    void tick_dram_phase();
    

};

}

#endif