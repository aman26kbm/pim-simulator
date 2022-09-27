// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

void test_rf_tile0(System* sys){
    std::vector<Request> requests;
    Request *request;

    //popCount_reduce_RF tile0 row0->tile0 RF
    request = new Request(Request::Type::PopCountReduce_RF);
    request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT4); //src
    request->addOperand(sys->getRFAddress(0,0), 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

void test_rf_tile1(System* sys){
    std::vector<Request> requests;
    Request *request;

    //popCount_reduce_RF tile1 row0->tile1 RF
    request = new Request(Request::Type::PopCountReduce_RF);
    request->addOperand(sys->getAddress(1,0,0), 0, PrecisionT::INT4); //src
    request->addOperand(sys->getRFAddress(1,0), 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

int32_t test_rf(System* sys){
    test_rf_tile0(sys);
    test_rf_tile1(sys);
    return 0;
}

static __attribute__((unused)) Registry::Entry &__test_rf__ = pimsim::registerFunc("test_rf", test_rf);
