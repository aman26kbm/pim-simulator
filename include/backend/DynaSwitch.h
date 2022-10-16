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

enum OutDirection {
    toD,
    toL,
    toN,
    toS,
    toW,
    toE,
    IDLE
};
typedef std::pair<OutDirection, int> ConnectState ;

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
    int channelNumber;

    int remainingStore=0;
    //receive queues
    // receiveQueues[N][0]~ receiveQueues[N][7]
    // receiveQueues[S][0]~ receiveQueues[S][7]...
    std::vector< std::vector< FixedQueue<Request> > > receiveQueues;


    //output port that an input port is connected to by crossbar
    // ConnectStates[N][0]~ConnectStates[N][7]
    // ConnectStates[S][0]~ConnectStates[S][7]...
    std::vector< std::vector< ConnectState > > connectStates;

    //number of packets left to send to output port in the current request
    // packetsRemaining[N][0]: int packets2N_0;
    //...
    std::vector< std::vector< int >> packetsRemaining;
    std::vector< std::vector<bool>> connected;
    std::vector<bool> isSent;

    //assume each router has a receive buffer that is *infinitely large*
    // std::vector<Request> localReceiveBuffer;
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
    bool data_exist_in_d(Request req, Direction d);
    bool pop_data(Request req);
    bool pop_data_in_d(Request req, Direction d);

    void print_my_status();
    void print_receive_Queues();
    void print_connection();
    void print_remaining_packets();
    //void print_local_receive_buffer();
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
    void setupConnection(Direction in,int channelIn, Direction out, int channelOut, int packets);
    bool neighborIsFull(Direction direction, int channel);
    void push2Neighbor(Request req, Direction direction, int channel);
    bool inputShouldSend(Direction in, int c_in);
    void inputSendFromDirection(Direction in);
    void inputSend(Direction in, int c_in);
    bool isMatch(Request bufferedReq, Request ReceiveReq);
    void tick_dram_phase();
    

};

}

#endif