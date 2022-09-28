#include "backend/DynaSwitch.h"
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
    //initialize receive queues
    // this->receiveQueues = new FixedQueue<Request>[Direction::BOUND];
    //initialize states
    // this->connectStates = new ConnectState[Direction::BOUND];
    //initialize remaining packet numbers
    // this->packetsRemaining = new int[Direction::BOUND];

    for(Direction d=N; d<BOUND; d++){
        this->receiveQueues.push_back(FixedQueue<Request>(2));
        this->connectStates.push_back(IDLE);
        this->packetsRemaining.push_back(0);
        this->connected.push_back(false);
    }
    //dram receive queue is infinitely large
    this->receiveQueues[D] = FixedQueue<Request>(INT32_MAX);

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
    tgt->receiveQueues = std::vector< FixedQueue<Request> >(src->receiveQueues);
    tgt->connectStates = std::vector< ConnectState >(src->connectStates);
    tgt->packetsRemaining = std::vector<int>(src->packetsRemaining);
    tgt->localReceiveBuffer = std::vector<Request>(src->localReceiveBuffer);
    //tgt->dramReceiveBuffer = std::vector<Request>(src->dramReceiveBuffer);
    tgt->neighborN = src->neighborN;
    tgt->neighborS = src->neighborS;
    tgt->neighborW = src->neighborW;
    tgt->neighborE = src->neighborE;
}

bool DynaSwitch::inject(Request req){
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
//push 1 packet of data to receiveQueueD
bool DynaSwitch::receive_from_dram(Request req){
    assert(!receiveQueues[D].is_full());
    next->receiveQueues[D].push(req);

    #ifdef _ROUTER_DEBUG_OUTPUT_
    printf("router (%d,%d) received from dram\n", myRow, myCol);
    #endif
}

//remove 1 matching entry from dramReceiveBuffer
bool DynaSwitch::store_to_dram(Request req){
    for(int i=0; i<dramReceiveBuffer.size(); i++){
        Request thisReq = dramReceiveBuffer[i];
        if(isMatch(thisReq, req)){
            dramReceiveBuffer.erase(dramReceiveBuffer.begin()+i--);
            return true;
        }
    }
    return false;
}

void DynaSwitch::tick(){
    //print before update
    #ifdef _ROUTER_DEBUG_OUTPUT_
    print_my_status();
    #endif

    //dram phase 1: send any dram load/store that are requestiong_load=true in LRB to dram, set packets2Mesh = bits
    for(int i=0; i<dramReceiveBuffer.size(); i++){
        Request* thisReq = &dramReceiveBuffer[i];
        assert(thisReq->type == Request::Type::RowLoad || thisReq->type == Request::Type::RowLoad_RF 
        || thisReq->type == Request::Type::RowStore || thisReq->type == Request::Type::RowStore_RF);
        
        if((thisReq->type == Request::Type::RowLoad || thisReq->type == Request::Type::RowLoad_RF)
        && thisReq->requesting_load== true) {
            thisReq->requesting_load = false;
            thisReq->packets2Mesh = thisReq->bits * cfg->_ncols * cfg->_nblocks /cfg->_wordsize_tile2tile;
            dram->receive_request(*thisReq);
            dramReceiveBuffer.erase(dramReceiveBuffer.begin()+i--);
            // i--;
        }

        if((thisReq->type == Request::Type::RowStore || thisReq->type == Request::Type::RowStore_RF)
        && thisReq->requesting_store == true) {
            if(remainingStore==0){
                thisReq->requesting_store = false;
                dram->receive_request(*thisReq);
                //remainingStore = thisReq->bits-1;
                remainingStore = thisReq->packets2Mesh-1;
            }
            else{
                thisReq->requesting_store = false;
                remainingStore--;
            }
        }
    }
    //dram phase 2: if any dram load request is dram_ready, try receive from dram. deduct packets2Mesh by 1
    //if any dram store request is ready, remove 1 entry from dramReceiveBuffer
    //only process the first dram_ready request in dramFinishedReqs
    for(int i=0; i<dram->dramFinishedReqs.size(); i++){
        Request* thisReq = &dram->dramFinishedReqs[i];
        if((thisReq->type == Request::Type::RowLoad || thisReq->type == Request::Type::RowLoad_RF) && thisReq->dram_ready == true){
            receive_from_dram(*thisReq);
            thisReq->packets2Mesh--;
            break;
        }
        if((thisReq->type == Request::Type::RowStore || thisReq->type == Request::Type::RowStore_RF) && thisReq->dram_ready == true){
            assert(store_to_dram(*thisReq));
            thisReq->packets2Mesh--;
            break;
        }
    }

    //dram phase 3: if any dram load/store request has packets2Mesh==0, remove it from dramReceiveBuffer
    for(int i=0; i<dram->dramFinishedReqs.size(); i++){
        Request* thisReq = &dram->dramFinishedReqs[i];
        if(thisReq->packets2Mesh == 0){
            dram->dramFinishedReqs.erase(dram->dramFinishedReqs.begin()+i--);
        }
    }

    //dram phase 4: tick my dram
    this->dram->tick();



    //phase 1: decode, setup possible connection or pop to local receive buffer
    for(Direction d=N; d<BOUND; d++){
        if(connectStates[d]==IDLE && !receiveQueues[d].empty()){
            Request req = receiveQueues[d].front();
            Direction downstream = decode(req);
            if(!connected[downstream]){
                setupConnection(d,downstream, req.packets2Mesh);
                // if((req.type == Request::Type::RowLoad || req.type == Request::Type::RowLoad_RF)
                // && req.requesting_load == true)
                //     setupConnection(d,downstream, 1);
                // else
                //     setupConnection(d,downstream, req.bits);
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
    // for(Direction d=N; d<BOUND; d++){
    //     if(packetsRemaining[connectStates[d]]==0){
    //         next->connectStates[d]=IDLE;
    //         connected[connectStates[d]]=false;
    //     }
    // }

    
}

bool DynaSwitch::data_exist(Request req){
    for(int i=0; i<localReceiveBuffer.size(); i++){
        Request thisReq = localReceiveBuffer[i];
        if(isMatch(thisReq, req)) return true;
    }
    return false;
}

Request DynaSwitch::pop_data(Request req){
    for(int i=0; i<localReceiveBuffer.size(); i++){
        Request thisReq = localReceiveBuffer[i];
        if(isMatch(thisReq, req)) {
            next->localReceiveBuffer.erase(next->localReceiveBuffer.begin()+i);
            return thisReq;
        }
    }
    return Request(Request::Type::NOP);
}

void DynaSwitch::print_my_status(){
    printf("router (%d, %d) status: ", myRow, myCol);
    print_receive_Queues();
    print_connection();
    print_remaining_packets();
    print_local_receive_buffer();
    print_dram_receive_buffer();
    dram->print_dram_finished_reqs();
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
    printf("%d ", (int)localReceiveBuffer.size());
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
    for(Direction d=N; d<BOUND; d++){
        if(!receiveQueues[d].empty()){
            receiveQueueEmpty = false;
            break;
        }
    }
    bool noConnection = true;
    for(Direction d=N; d<BOUND; d++){
        if(packetsRemaining[d]!=0){
            noConnection = false;
            break;
        }
    }
    bool finished = receiveQueueEmpty 
    && noConnection
    && localReceiveBuffer.empty()
    && dramReceiveBuffer.empty()
    && dram->is_finished();
    return finished;
}


//////////////////////////////////Utils///////////////////////////////

/////////////////FixedQueue/////////////////
template <typename T>
FixedQueue<T>::FixedQueue(int maxLen){
    this->maxLen = maxLen;
}

template <typename T>
FixedQueue<T>::FixedQueue(){
    this->maxLen =2;
}

template <typename T>
void FixedQueue<T>::push(const T& value) {
    if (this->size() == maxLen) {
        assert(false);
    }
    std::queue<T, std::deque<T>>::push(value);
}
template <typename T>
bool FixedQueue<T>::is_full(){
    return (this->size() >= maxLen);
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
        case D: return "D";
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
        case toD: return "toD";
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
    if(req.type == Request::Type::RowStore_RF || req.type == Request::Type::RowStore) 
        //return cfg->_dramTile;
        return get_addr0_index(req)%cfg->_meshWidth;
    else if(req.type == Request::Type::RowLoad_RF || req.type == Request::Type::RowLoad){
        if(req.requesting_load)
            //return cfg->_dramTile;
            return get_addr0_index(req)%cfg->_meshWidth;
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
        case D:
            //return receiveQueueDram.is_full();
            return false;//since dram receive buffer is infinitely large
            break;
        default:
            assert(false);
            return false;
    }
}


void DynaSwitch::push2Neighbor(Request req, Direction direction){
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
        case D:
            this->dramReceiveBuffer.push_back(req);
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

void DynaSwitch::setupConnection(Direction in, Direction out, int packets){
    //assert(out!=Direction::L);
    assert(!receiveQueues[in].empty());
    assert(packetsRemaining[out]==0);
    // this to avoid 2 input set up connection to same output
    //this->packetsRemaining[out]=packets;
    connected[out] = true;
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
    if(next->packetsRemaining[connectStates[in]]==0){
        next->connectStates[in]=IDLE;
        connected[connectStates[in]]=false;
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