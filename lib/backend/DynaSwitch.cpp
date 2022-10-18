#include "backend/DynaSwitch.h"
//workaround
#include "DataStructs.cpp"
#include <string.h>
using namespace pimsim;
DynaSwitch::DynaSwitch(){}
DynaSwitch::DynaSwitch(int index, Config* cfg){
    assert(index>=0 && index<cfg->_meshHeight*cfg->_meshWidth);
    this->index = index;
    this->myRow = index/cfg->_meshWidth;
    this->myCol = index%cfg->_meshWidth;
    this->bitwidth = cfg->_wordsize_tile2tile;

    this->cfg = cfg;
    this->channelNumber = cfg->_router_channel_number;
    this->isSent =  std::vector<bool>(IDLE,false);
    this->currentChannel = std::vector<int>(BOUND, 0);
    //initialize receive queues
    // this->receiveQueues = new FixedQueue<Request>[Direction::BOUND];
    //initialize states
    // this->connectStates = new ConnectState[Direction::BOUND];
    //initialize remaining packet numbers
    // this->packetsRemaining = new int[Direction::BOUND];

    for(Direction d=(Direction)0; d<BOUND; d++){
        std::vector<FixedQueue<Request>> myReceiveQueues;
        std::vector< ConnectState > myConnectStates;
        std::vector< int > myPacketsRemaining;
        std::vector<bool> myConnected;
        for(int c=0; c<channelNumber; c++){
            myReceiveQueues.push_back(FixedQueue<Request>(2));
            myConnectStates.push_back(ConnectState(IDLE,0));
            myPacketsRemaining.push_back(0);
            myConnected.push_back(false);
        }
        this->receiveQueues.push_back(myReceiveQueues);
        this->connectStates.push_back(myConnectStates);
        this->packetsRemaining.push_back(myPacketsRemaining);
        this->connected.push_back(myConnected);
    }
    //dram receive queue is infinitely large
    //this->receiveQueues[D] = FixedQueue<Request>(INT32_MAX);

    this->dramReceiveBuffer = FixedQueue<Request>(2);

    // this->next = (DynaSwitch*)malloc(sizeof(DynaSwitch));
    // memcpy(next, this, sizeof(DynaSwitch));
    this->next = new DynaSwitch();
    copy_content(this, next);
    

    //create dram
    this->dram = new Dram_sendback(cfg);

    #ifdef _ROUTER_DEBUG_OUTPUT_
    printf("create router (%d,%d)\n", myRow, myCol);
    #endif
}

//everything except next pointer is copied
void DynaSwitch::copy_content(const DynaSwitch* src, DynaSwitch* tgt){
    //tgt->dram = src->dram;
    tgt->index = src->index;
    tgt->myRow = src->myRow;
    tgt->myCol = src->myCol;
    tgt->cfg = src->cfg;
    tgt->bitwidth = src->bitwidth;
    tgt->receiveQueues = src->receiveQueues;
    tgt->connectStates = src->connectStates;
    tgt->packetsRemaining = src->packetsRemaining;
    //tgt->localReceiveBuffer = src->localReceiveBuffer;
    //tgt->dramReceiveBuffer = std::vector<Request>(src->dramReceiveBuffer);
    tgt->neighborN = src->neighborN;
    tgt->neighborS = src->neighborS;
    tgt->neighborW = src->neighborW;
    tgt->neighborE = src->neighborE;
    tgt->isSent = src->isSent;
}

bool DynaSwitch::inject(Request req){
    // int availChannel = 0;
    // bool found=false;
    // for(int c=0; c<channelNumber; c++){
    //     if(!receiveQueues[L][c].is_full()) {
    //         availChannel = c;
    //         found=true;
    //         break;
    //     }
       
    // } 
    // if(!found){
    //     #ifdef _ROUTER_DEBUG_OUTPUT_
    //     printf("router (%d,%d) inject fail\n", myRow, myCol);
    //     #endif

    //     return false;
    // }
    // else{
    //     next->receiveQueues[L][availChannel].push(req);

    //     #ifdef _ROUTER_DEBUG_OUTPUT_
    //     printf("router (%d,%d) inject success\n", myRow, myCol);
    //     #endif

    //     return true;
    // }
    if(!receiveQueues[L][0].is_full()){
        next->receiveQueues[L][0].push(req);

        #ifdef _ROUTER_DEBUG_OUTPUT_
        printf("router (%d,%d) inject success\n", myRow, myCol);
        #endif

        return true;
    }
    else{
        #ifdef _ROUTER_DEBUG_OUTPUT_
        printf("router (%d,%d) inject fail\n", myRow, myCol);
        #endif

        return false;        
    }
}


void DynaSwitch::tick(){
    //print before update
    #ifdef _ROUTER_DEBUG_OUTPUT_
    print_my_status();
    #endif

    
    tick_dram_phase();


    //phase 1: decode, setup possible connection or pop to local receive buffer
    for(Direction d=(Direction)0; d<BOUND; d++){
        for(int c_in=0; c_in<channelNumber; c_in++){
            if(connectStates[d][c_in].first==IDLE && !receiveQueues[d][c_in].empty()){
                Request req = receiveQueues[d][c_in].front();
                Direction downstream = decode(req);
                if(downstream==D){
                    if(!connected[D][0]){
                        setupConnection(d,c_in,D,0, req.packets2Mesh);
                    }
                }
                else if(downstream!=L){
                    setupConnectionForInputChannel(d,c_in,downstream,req.packets2Mesh);
                }
            }
        }
    }

    //phase 2: push to neighbors' receive queues
    for(Direction d=(Direction)0; d<BOUND; d++){
        inputSendFromDirection(d);
    }
    //phase 3: update my states variables
    // for(Direction d=(Direction)0; d<BOUND; d++){
    //     if(packetsRemaining[connectStates[d]]==0){
    //         next->connectStates[d]=IDLE;
    //         connected[connectStates[d]]=false;
    //     }
    // }

    
}
bool DynaSwitch::data_exist_in_d(Request req, Direction d){
    for(int c=0; c<channelNumber; c++){
        if(receiveQueues[d][c].empty()) continue;
        Request thisReq = receiveQueues[d][c].front();
        if(isMatch(thisReq, req)) return true;
    } 
    return false;
}

// bool DynaSwitch::data_exist(Request req){
//     for(int i=0; i<localReceiveBuffer.size(); i++){
//         Request thisReq = localReceiveBuffer[i];
//         if(isMatch(thisReq, req)) return true;
//     }
//     return false;
// }

bool DynaSwitch::data_exist(Request req){
    for(Direction d:{N,S,W,E,D}){
        if(data_exist_in_d(req, d)){
            return true;
        }
    }
    return false;
}

bool DynaSwitch::pop_data_in_d(Request req, Direction d){
    for(int c=0; c<channelNumber; c++){
        if(receiveQueues[d][c].empty()) continue;
        Request thisReq = receiveQueues[d][c].front();
        if(isMatch(thisReq, req)) {
            this->receiveQueues[d][c].pop();
            next->receiveQueues[d][c].pop();
            return true;
        }
    }
    return false;
}
bool DynaSwitch::pop_data(Request req){
    for(Direction d:{N,S,W,E,D}){
        if(pop_data_in_d(req,d)) return true;
    }
    return false;
}

void DynaSwitch::print_my_status(){
    printf("router (%d, %d) status: ", myRow, myCol);
    print_receive_Queues();
    print_connection();
    print_remaining_packets();
    //print_local_receive_buffer();
    print_dram_receive_buffer();
    dram->print_dram_finished_reqs();
    printf("\n");
}

void DynaSwitch::print_receive_Queues(){
    printf("receive queues: ");
    for(Direction d=(Direction)0; d<BOUND; d++){
        printf("%s: ",toString(d).c_str());
        for(int c=0; c<channelNumber; c++){
            printf("%d ", (int)receiveQueues[d][c].size());
        }
    }
    printf("\n");
}
void DynaSwitch::print_connection(){
    printf("connection: ");
    for(Direction d=(Direction)0; d<BOUND; d++){
        for(int c=0; c<channelNumber; c++){
            printf("(%s,%d)%s ",toString(d).c_str(), c, toString(connectStates[d][c]).c_str());
        }
    }
}
void DynaSwitch::print_remaining_packets(){
    printf("remaining packets: ");
    for(Direction d=(Direction)0; d<BOUND; d++){
        for(int c=0; c<channelNumber; c++){
            printf("(%s,%d)%d ",toString(d).c_str(), c, packetsRemaining[d][c]);
        }
    }
}

void DynaSwitch::print_dram_receive_buffer(){
    printf("dram receive buffer: ");
    printf("%d ", (int)dramReceiveBuffer.size());
}

void DynaSwitch::update_current(){
    // memcpy(this->receiveQueues, next->receiveQueues, sizeof(*this->receiveQueues)*5);
    // memcpy(this->connectStates, next->connectStates, sizeof(*this->connectStates)*5);
    // memcpy(this->packetsRemaining, next->packetsRemaining, sizeof(*this->packetsRemaining)*5);
    // this->localReceiveBuffer = next->localReceiveBuffer;
    // this->dramReceiveBuffer = next->dramReceiveBuffer;
    copy_content(this->next, this);
}

bool DynaSwitch::is_finished(){
    bool receiveQueueEmpty = true;
    for(Direction d=(Direction)0; d<BOUND; d++){
        for(int c=0; c<channelNumber; c++){
            if(!receiveQueues[d][c].empty()){
                receiveQueueEmpty = false;
                break;
            }
        }
    }
    bool noConnection = true;
    for(Direction d=(Direction)0; d<BOUND; d++){
        for(int c=0; c<channelNumber; c++){
            if(packetsRemaining[d][c]!=0){
                noConnection = false;
                break;
            }
        }
    }
    bool finished = receiveQueueEmpty 
    && noConnection
    && dramReceiveBuffer.empty()
    && dram->is_finished();
    return finished;
}


//////////////////////////////////Utils///////////////////////////////





/////////////////Direction//////////////////
Direction pimsim::operator ++ (Direction& d, int)
{
    if (d == Direction::BOUND) {
        throw std::out_of_range("for Direction& operator ++ (Direction&)");
    }
    Direction rVal = d; 
    d = (Direction)(d + 1);
    return rVal;
}
std::string pimsim::toString(Direction d){
    switch(d){
        case N: return "N";
        case S: return "S";
        case W: return "W";
        case E: return "E";
        case L: return "L";
        case D: return "D";
        case BOUND: return "BOUND";
        default: {
            assert(false);
            return "";
        }
    }
}
std::string pimsim::toString(ConnectState s){
    switch(s.first){
        case toN: return "toN"+std::to_string(s.second);
        case toS: return "toS"+std::to_string(s.second);
        case toW: return "toW"+std::to_string(s.second);
        case toE: return "toE"+std::to_string(s.second);
        case toL: return "toL"+std::to_string(s.second);
        case toD: return "toD"+std::to_string(s.second);
        case IDLE: return "IDLE";
        default: {
            assert(false);
            return "";
        }
    }
}

////////////////DynaSwitch////////////////////
//get addr_list[0] index
int DynaSwitch::get_addr0_index(Request req){
    if(req.type == Request::Type::RowLoad_RF || req.type == Request::Type::RowStore_RF){
        AddrT addr;
        addr = req.addr_list[0];
        int index = addr/cfg->_num_regs_per_rf;
        return index;
    }
    else{
        AddrT addr;
        //Source
        addr = req.addr_list[0];
        int index = addr/(cfg->_ncols * cfg->_nrows * cfg->_nblocks);
        return index;
    }    
}

//get addr_list[1] index
int DynaSwitch::get_addr1_index(Request req){
    AddrT addr;
    //Source
    addr = req.addr_list[1];
    int index = addr/(cfg->_ncols * cfg->_nrows * cfg->_nblocks);
    return index;
}

int DynaSwitch::get_source_index(Request req){
    if(req.type == Request::Type::RowLoad_RF || req.type == Request::Type::RowLoad) return cfg->_dramTile;
    else return get_addr0_index(req);
}

int DynaSwitch::get_dest_index(Request req){
    if(req.type == Request::Type::RowStore_RF || req.type == Request::Type::RowStore) {
        if(!cfg->_dramDistributed)
            return cfg->_dramTile;
        return get_addr0_index(req)%cfg->_meshWidth;
    }
    else if(req.type == Request::Type::RowLoad_RF || req.type == Request::Type::RowLoad){
        if(req.requesting_load){
            if(!cfg->_dramDistributed)
                return cfg->_dramTile;
            return get_addr0_index(req)%cfg->_meshWidth;
        }
        else
            return get_addr0_index(req);
    }
    else{
        //send
        return get_addr1_index(req);
    } 
}

bool DynaSwitch::neighborIsFull(Direction direction, int channel){
    switch(direction){
        case N:
            return neighborN->receiveQueues[S][channel].is_full();
            break;
        case S:
            return neighborS->receiveQueues[N][channel].is_full();
            break;
        case W:
            return neighborW->receiveQueues[E][channel].is_full();
            break;
        case E:
            return neighborE->receiveQueues[W][channel].is_full();
            break;
        case L:
            //return localReceiveBuffer.is_full();
            return false;//since local receive buffer is infinitely large
            break;
        case D:
            return dramReceiveBuffer.is_full();
            //return false;//since dram receive buffer is infinitely large
            break;
        default:
            assert(false);
            return false;
    }
}


void DynaSwitch::push2Neighbor(Request req, Direction direction, int channel){
    assert(!neighborIsFull(direction, channel));
    switch(direction){
        case N:
            neighborN->next->receiveQueues[S][channel].push(req);
            req.dynaMeshHops++;
            break;
        case S:
            neighborS->next->receiveQueues[N][channel].push(req);
            req.dynaMeshHops++;
            break;
        case W:
            neighborW->next->receiveQueues[E][channel].push(req);
            req.dynaMeshHops++;
            break;
        case E:
            neighborE->next->receiveQueues[W][channel].push(req);
            req.dynaMeshHops++;
            break;
        case L:
            assert(false);
            //next->localReceiveBuffer.push_back(req);
            break;
        case D:
            this->dramReceiveBuffer.push(req);
            break;
        default:
            assert(false);
    }
}



Direction DynaSwitch::decode(Request req){
    int dest = get_dest_index(req);
    int destRow = dest/cfg->_meshWidth;
    int destCol = dest%cfg->_meshWidth;
    if(destRow>myRow) return Direction::S;
    else if(destRow<myRow) return Direction::N;
    else if(destCol>myCol) return Direction::E;
    else if(destCol<myCol) return Direction::W;
    else if(((req.type == Request::Type::RowLoad || req.type == Request::Type::RowLoad_RF) && req.requesting_load == true) 
    || req.type == Request::Type::RowStore || req.type == Request::Type::RowStore_RF) 
        return Direction::D;
    else return Direction::L;
}


void DynaSwitch::setupConnection(Direction in,int channelIn, Direction out, int channelOut, int packets){
    assert(out!=Direction::L);
    if(out==Direction::D) assert(channelOut==0);
    assert(!receiveQueues[in][channelIn].empty());
    assert(packetsRemaining[out][channelOut]==0);
    // this to avoid 2 input set up connection to same output
    //this->packetsRemaining[out]=packets;
    connected[out][channelOut] = true;
    next->packetsRemaining[out][channelOut]=packets;
    next->connectStates[in][channelIn] = ConnectState((OutDirection)out,channelOut);
}

void DynaSwitch::setupConnectionForInputChannel(Direction in,int channelIn, Direction out, int packets){
    for(int c_out=0; c_out<channelNumber; c_out++){
        if(!connected[out][c_out]){
            setupConnection(in,channelIn,out,c_out, packets);
            return;
            // if((req.type == Request::Type::RowLoad || req.type == Request::Type::RowLoad_RF)
            // && req.requesting_load == true)
            //     setupConnection(d,downstream, 1);
            // else
            //     setupConnection(d,downstream, req.bits);
        }
    }
}

bool DynaSwitch::inputShouldSend(Direction in, int c_in){
    bool shouldSend = false;
    //check connection
    Direction d_out = (Direction)connectStates[in][c_in].first;
    int  c_out = connectStates[in][c_in].second;
    if(connectStates[in][c_in].first!=IDLE && !receiveQueues[in][c_in].empty() && !isSent[d_out]){
        //check there are something left to send
        if(packetsRemaining[d_out][c_out]>0){
            //check target neighbor's rceive queue is not full
            if(!neighborIsFull(d_out, c_out)){
                shouldSend = true;
            }
        }
    }
    return shouldSend;
}

void DynaSwitch::inputSendFromDirection(Direction d_in){
    // for(int c_in=0; c_in<channelNumber; c_in++){
    //     if(inputShouldSend(d_in, c_in)){
    //         inputSend(d_in, c_in);
    //         return;
    //     }
    // }
    int c_in=currentChannel[d_in];
    int iter = 0;
    while(!inputShouldSend(d_in, c_in) && iter<channelNumber){
        c_in++;
        iter++;
        if(c_in==channelNumber){
            c_in = 0;
        }
    }
    if(inputShouldSend(d_in, c_in)){
        inputSend(d_in, c_in);
        c_in++;
        if(c_in==channelNumber){
            c_in = 0;
        }
        currentChannel[d_in]=c_in;
    }
    else{
        currentChannel[d_in]=c_in;
    }
    return;
}
//push front of input to the neighbor it is routed to
void DynaSwitch::inputSend(Direction in, int c_in){
    assert(!receiveQueues[in][c_in].empty());
    assert(connectStates[in][c_in].first!=IDLE);
    Direction d_out = (Direction)connectStates[in][c_in].first;
    int  c_out = connectStates[in][c_in].second;
    assert(packetsRemaining[d_out][c_out]>0);
    assert(!neighborIsFull(d_out, c_out));
    push2Neighbor(receiveQueues[in][c_in].front(), d_out, c_out);
    next->receiveQueues[in][c_in].pop();
    next->packetsRemaining[d_out][c_out]--;
    isSent[d_out]=true;
    if(next->packetsRemaining[d_out][c_out]==0){
    //if(packetsRemaining[connectStates[in]]==0){
        next->connectStates[in][c_in].first=IDLE;
        connected[d_out][c_out]=false;
    }
}

bool DynaSwitch::isMatch(Request bufferedReq, Request ReceiveReq){
    //return get_source_index(bufferedReq)==get_source_index(ReceiveReq) && get_dest_index(bufferedReq)==get_dest_index(ReceiveReq);
    bool retVal;
    if(ReceiveReq.type == Request::Type::RowLoad || ReceiveReq.type == Request::Type::RowLoad_RF)
        retVal = bufferedReq.addr_list[0]==ReceiveReq.addr_list[0];
    else
        retVal = bufferedReq.addr_list[0]==ReceiveReq.addr_list[0] && bufferedReq.addr_list[1]==ReceiveReq.addr_list[1];
    
    return retVal;
}