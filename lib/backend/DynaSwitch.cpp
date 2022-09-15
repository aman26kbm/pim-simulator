#include "backend/DynaSwitch.h"

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
    this->packetsRemaining = new int[4];

    #ifdef DEBUG_OUTPUT
    printf("create switch %d with bitwidth:", index);
    print(this->bitwidth);
    printf("\n");
    #endif
}

bool DynaSwitch::inject(Request* req){
    if(receiveQueues[L].is_full()) return false;
    receiveQueues[L].push(req);
    return true;
}

void DynaSwitch::tick(){
    //phase 1: decode, setup possible connection or pop to local receive buffer
    for(Direction d=N; d<=L; d++){
        if(connectStates[d]==IDLE && !receiveQueues[d].empty()){
            Request* req = receiveQueues[d].front();
            Direction downstream = decode(req);
            if(packetsRemaining[downstream]==0){
                setupConnection(d,downstream, req->precision_list[0].bits());
            }
        }
    }

    //phase 2: push to neighbors' receive queues
    for(Direction d=N; d<=L; d++){
        if(inputShouldSend(d)){
            inputSend(d);
        }
    }
    //phase 3: update my states variables
    for(Direction d=N; d<=L; d++){
        if(packetsRemaining[connectStates[d]]==0){
            connectStates[d]=IDLE;
        }
    }
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
    if (d == Direction::L) {
        throw std::out_of_range("for Direction& operator ++ (Direction&)");
    }
    Direction rVal = d; 
    d = (Direction)(d + 1);
    return d;
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
            return true;//since local receive buffer is infinitely large
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
            neighborN->receiveQueues[S].push(req);
            break;
        case S:
            neighborS->receiveQueues[N].push(req);
            break;
        case W:
            neighborW->receiveQueues[E].push(req);
            break;
        case E:
            neighborE->receiveQueues[W].push(req);
            break;
        case L:
            localReceiveBuffer.push_back(req);
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
    assert(out!=Direction::L);
    assert(!receiveQueues[in].empty());
    assert(packetsRemaining[out]==0);
    packetsRemaining[out]=packets;
    connectStates[in] = (ConnectState)out;
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
}
//push front of input to the neighbor it is routed to
void DynaSwitch::inputSend(Direction in){
    assert(!receiveQueues[in].empty());
    assert(connectStates[in]!=IDLE);
    assert(packetsRemaining[(Direction)connectStates[in]]>0);
    assert(!neighborIsFull((Direction)connectStates[in]));
    push2Neighbor(receiveQueues[in].front(), (Direction)connectStates[in]);
    receiveQueues[in].pop();
    packetsRemaining[(Direction)connectStates[in]]--;
}

bool DynaSwitch::isMatch(Request* bufferedReq, Request* ReceiveReq){
    return get_source_index(bufferedReq)==get_source_index(ReceiveReq) && get_dest_index(bufferedReq)==get_dest_index(ReceiveReq);
}