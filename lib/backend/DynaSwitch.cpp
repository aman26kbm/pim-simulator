#include "backend/DynaSwitch.h"
#include <string.h>
using namespace pimsim;

DynaSwitch::DynaSwitch(int index, Config* cfg){
    assert(index>=0 && index<cfg->_meshHeight*cfg->_meshWidth);
    this->index = index;
    this->myRow = index/cfg->_meshWidth;
    this->myCol = index%cfg->_meshWidth;
    this->bitwidth = cfg->_wordsize_tile2tile;

    this->cfg = cfg;
    //initialize receive queues
    this->receiveQueues = new FixedQueue<Request*,2>[5];
    //initialize states
    this->connectStates = new ConnectState[5];
    //initialize remaining packet numbers
    this->packetsRemaining = new int[5];

    this->next = (DynaSwitch*)malloc(sizeof(DynaSwitch));
    memcpy(next, this, sizeof(DynaSwitch));

    #ifdef _ROUTER_DEBUG_OUTPUT_
    printf("create router (%d,%d)\n", myRow, myCol);
    #endif
}

bool DynaSwitch::inject(Request* req){
    if(receiveQueues[L].is_full()) {
        #ifdef _ROUTER_DEBUG_OUTPUT_
        printf("router (%d,%d) inject fail\n", myRow, myCol);
        #endif
        return false;
    }
    next->receiveQueues[L].push(req);

    #ifdef _ROUTER_DEBUG_OUTPUT_
    printf("router (%d,%d) inject success\n", myRow, myCol);
    #endif

    return true;
}

void DynaSwitch::tick(){
    //print before update
    #ifdef _ROUTER_DEBUG_OUTPUT_
    print_my_status();
    #endif

    //phase 1: decode, setup possible connection or pop to local receive buffer
    for(Direction d=N; d<BOUND; d++){
        if(connectStates[d]==IDLE && !receiveQueues[d].empty()){
            Request* req = receiveQueues[d].front();
            Direction downstream = decode(req);
            if(packetsRemaining[downstream]==0){
                setupConnection(d,downstream, req->precision_list[0].bits());
            }
        }
    }

    //phase 2: push to neighbors' receive queues
    for(Direction d=N; d<BOUND; d++){
        if(inputShouldSend(d)){
            inputSend(d);
        }
    }
    //phase 3: update my states variables
    for(Direction d=N; d<BOUND; d++){
        if(packetsRemaining[connectStates[d]]==0){
            next->connectStates[d]=IDLE;
        }
    }
    update_current();
}

bool DynaSwitch::data_exist(Request* req){
    for(int i=0; i<localReceiveBuffer.size(); i++){
        Request* thisReq = localReceiveBuffer[i];
        if(isMatch(thisReq, req)) return true;
    }
    return false;
}

bool DynaSwitch::pop_data(Request* req){
    for(int i=0; i<localReceiveBuffer.size(); i++){
        Request* thisReq = localReceiveBuffer[i];
        if(isMatch(thisReq, req)) {
            localReceiveBuffer.erase(localReceiveBuffer.begin()+i);
            return true;
        }
    }
    return false;
}

void DynaSwitch::print_my_status(){
    printf("router (%d, %d) status: ", myRow, myCol);
    print_receive_Queues();
    print_connection();
    print_remaining_packets();
    print_local_receive_buffer();
    printf("\n");
}

void DynaSwitch::print_receive_Queues(){
    printf("receive queues: ");
    for(Direction d=N; d<BOUND; d++){
        printf("%d ", (int)receiveQueues[d].size());
    }
}
void DynaSwitch::print_connection(){
    printf("connection: ");
    for(Direction d=N; d<BOUND; d++){
        printf("(%s)%s ",toString(d).c_str(), toString(connectStates[d]).c_str());
    }
}
void DynaSwitch::print_remaining_packets(){
    printf("remaining packets: ");
    for(Direction d=N; d<BOUND; d++){
        printf("(%s)%d ",toString(d).c_str(), packetsRemaining[d]);
    }
}
void DynaSwitch::print_local_receive_buffer(){
    printf("local receive buffer: ");
    printf("%d", (int)localReceiveBuffer.size());
}

void DynaSwitch::update_current(){
    memcpy(this->receiveQueues, next->receiveQueues, sizeof(*this->receiveQueues)*5);
    memcpy(this->connectStates, next->connectStates, sizeof(*this->connectStates)*5);
    memcpy(this->packetsRemaining, next->packetsRemaining, sizeof(*this->packetsRemaining)*5);
    this->localReceiveBuffer = next->localReceiveBuffer;
}


//////////////////////////////////Utils///////////////////////////////

/////////////////FixedQueue/////////////////
template <typename T, int MaxLen>
void FixedQueue<T,MaxLen>::push(const T& value) {
    if (this->size() == MaxLen) {
        assert(false);
    }
    std::queue<T, std::deque<T>>::push(value);
}
template <typename T, int MaxLen>
bool FixedQueue<T,MaxLen>::is_full(){
    return (this->size() >= MaxLen);
}

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
        case BOUND: return "BOUND";
        default: {
            assert(false);
            return "";
        }
    }
}
std::string pimsim::toString(ConnectState s){
    switch(s){
        case toN: return "toN";
        case toS: return "toS";
        case toW: return "toW";
        case toE: return "toE";
        case toL: return "toL";
        case IDLE: return "IDLE";
        default: {
            assert(false);
            return "";
        }
    }
}

////////////////DynaSwitch////////////////////
//get addr_list[0] index
int DynaSwitch::get_addr0_index(Request* req){
    if(req->type == Request::Type::RowLoad_RF || req->type == Request::Type::RowStore_RF){
        AddrT addr;
        addr = req->addr_list[0];
        int index = addr/cfg->_num_regs_per_rf;
        return index;
    }
    else{
        AddrT addr;
        //Source
        addr = req->addr_list[0];
        int index = addr/(cfg->_ncols * cfg->_nrows * cfg->_nblocks);
        return index;
    }    
}

//get addr_list[1] index
int DynaSwitch::get_addr1_index(Request* req){
    AddrT addr;
    //Source
    addr = req->addr_list[1];
    int index = addr/(cfg->_ncols * cfg->_nrows * cfg->_nblocks);
    return index;
}

int DynaSwitch::get_source_index(Request* req){
    if(req->type == Request::Type::RowLoad_RF || req->type == Request::Type::RowLoad) return cfg->_dramTile;
    else return get_addr0_index(req);
}

int DynaSwitch::get_dest_index(Request* req){
    if(req->type == Request::Type::RowStore_RF || req->type == Request::Type::RowStore) return cfg->_dramTile;
    else if(req->type == Request::Type::RowLoad_RF || req->type == Request::Type::RowLoad){
        if(req->requesting_load)
            return cfg->_dramTile;
        else
            return get_addr0_index(req);
    }
    else{
        //send
        return get_addr1_index(req);
    } 
}

bool DynaSwitch::neighborIsFull(Direction direction){
    switch(direction){
        case N:
            return neighborN->receiveQueues[S].is_full();
            break;
        case S:
            return neighborS->receiveQueues[N].is_full();
            break;
        case W:
            return neighborW->receiveQueues[E].is_full();
            break;
        case E:
            return neighborE->receiveQueues[W].is_full();
            break;
        case L:
            //return receiveQueues[L].is_full();
            return false;//since local receive buffer is infinitely large
            break;
        default:
            assert(false);
            return false;
    }
}


void DynaSwitch::push2Neighbor(Request* req, Direction direction){
    assert(!neighborIsFull(direction));
    switch(direction){
        case N:
            neighborN->next->receiveQueues[S].push(req);
            break;
        case S:
            neighborS->next->receiveQueues[N].push(req);
            break;
        case W:
            neighborW->next->receiveQueues[E].push(req);
            break;
        case E:
            neighborE->next->receiveQueues[W].push(req);
            break;
        case L:
            next->localReceiveBuffer.push_back(req);
            break;
        default:
            assert(false);
    }
}



Direction DynaSwitch::decode(Request* req){
    int dest = get_dest_index(req);
    int destRow = dest/cfg->_meshWidth;
    int destCol = dest%cfg->_meshWidth;
    if(destRow>myRow) return Direction::S;
    else if(destRow<myRow) return Direction::N;
    else if(destCol>myCol) return Direction::E;
    else if(destCol<myCol) return Direction::W;
    else return Direction::L;
}

void DynaSwitch::setupConnection(Direction in, Direction out, int packets){
    //assert(out!=Direction::L);
    assert(!receiveQueues[in].empty());
    assert(packetsRemaining[out]==0);
    next->packetsRemaining[out]=packets;
    next->connectStates[in] = (ConnectState)out;
}


bool DynaSwitch::inputShouldSend(Direction in){
    bool shouldSend = false;
    //check connection
    if(connectStates[in]!=IDLE && !receiveQueues[in].empty()){
        //check there are something left to send
        if(packetsRemaining[(Direction)connectStates[in]]>0){
            //check target neighbor's rceive queue is not full
            if(!neighborIsFull((Direction)connectStates[in])){
                shouldSend = true;
            }
        }
    }
    return shouldSend;
}
//push front of input to the neighbor it is routed to
void DynaSwitch::inputSend(Direction in){
    assert(!receiveQueues[in].empty());
    assert(connectStates[in]!=IDLE);
    assert(packetsRemaining[(Direction)connectStates[in]]>0);
    assert(!neighborIsFull((Direction)connectStates[in]));
    push2Neighbor(receiveQueues[in].front(), (Direction)connectStates[in]);
    next->receiveQueues[in].pop();
    next->packetsRemaining[(Direction)connectStates[in]]--;
}

bool DynaSwitch::isMatch(Request* bufferedReq, Request* ReceiveReq){
    return get_source_index(bufferedReq)==get_source_index(ReceiveReq) && get_dest_index(bufferedReq)==get_dest_index(ReceiveReq);
}