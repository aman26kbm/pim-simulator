// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"
int trans_count = 100;
void test_trans_tile0(System* sys){
    std::vector<Request> requests;
    Request *request;

    for(int i =0; i<trans_count; i++){
        request = new Request(Request::Type::TileSend);
        request->addOperand(sys->getAddress(0,0,(i*4)%128), 0, PrecisionT::INT4); //src
        request->addOperand(sys->getAddress(127,0,(i*4)%128), 0, PrecisionT::INT4); //dst
        requests.push_back(*request);
    }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

void test_trans_tile128(System* sys){
    std::vector<Request> requests;
    Request *request;

    for(int i =0; i<trans_count; i++){
        request = new Request(Request::Type::TileReceive);
        request->addOperand(sys->getAddress(0,0,(i*4)%128), 0, PrecisionT::INT4); //src
        request->addOperand(sys->getAddress(127,0,(i*4)%128), 0, PrecisionT::INT4); //dst
        requests.push_back(*request);
    }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

void test_trans_tile64(System* sys){
    std::vector<Request> requests;
    Request *request;

    for(int i =0; i<trans_count; i++){
        request = new Request(Request::Type::RowLoad);
        request->addOperand(sys->getAddress(64,0,(i*4)%128), 0, PrecisionT::INT4); //dst
        request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //src
        requests.push_back(*request);
    }
    for(int i =0; i<trans_count; i++){
        request = new Request(Request::Type::RowStore);
        request->addOperand(sys->getAddress(64,0,(i*4)%128), 0, PrecisionT::INT4); //src
        request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //dst
        requests.push_back(*request);
    }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

int32_t test_trans(System* sys){
    test_trans_tile0(sys);
    test_trans_tile128(sys);
    test_trans_tile64(sys);
    return 0;
}

static __attribute__((unused)) Registry::Entry &__test_trans__ = pimsim::registerFunc("test_consecutive_transfer", test_trans);