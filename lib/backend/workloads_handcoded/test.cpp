// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"
/////////////////////////////////////////////////////////////
// Queuing requests for tile0
/////////////////////////////////////////////////////////////

void test_tile0(System* sys)
{
    std::vector<Request> requests;
    Request *request;

    request = new Request(Request::Type::RowLoad);
    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //src
    request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::RowMul);
    request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT4); //src
    request->addOperand(sys->getAddress(0,0,4), 0, PrecisionT::INT4); //src
    request->addOperand(sys->getAddress(0,0,8), 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::RowAdd);
    request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT4); //src
    request->addOperand(sys->getAddress(0,0,4), 0, PrecisionT::INT4); //src
    request->addOperand(sys->getAddress(0,0,8), 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::Signal, sys->m1);
    request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    request = new Request(Request::Type::BlockSend_Receive);
    request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT4); //src
    request->addOperand(sys->getAddress(0,0,8), 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::Barrier, sys->m2);
    request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    request = new Request(Request::Type::RowStore);
    request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT4); //src
    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::RowMul);
    request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT4); //src
    request->addOperand(sys->getAddress(0,0,4), 0, PrecisionT::INT4); //src
    request->addOperand(sys->getAddress(0,0,8), 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::RowLoad);
    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //src
    request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::RowLoad_RF);
    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //src
    request->addOperand(sys->getRFAddress(0,0), 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::RowStore_RF);
    request->addOperand(sys->getRFAddress(0,0), 0, PrecisionT::INT4); //src
    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::RowMul_CRAM_RF);
    request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT4); //src
    request->addOperand(sys->getRFAddress(0,0), 0, PrecisionT::INT4);
    request->addOperand(sys->getAddress(0,0,8), 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::RowAdd_CRAM_RF);
    request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT4); //src
    request->addOperand(sys->getAddress(0,0,8), 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

/////////////////////////////////////////////////////////////
// Queuing requests for tile1
/////////////////////////////////////////////////////////////

void test_tile1(System* sys)
{
    std::vector<Request> requests;
    Request *request;

    request = new Request(Request::Type::RowAdd);
    request->addOperand(sys->getAddress(1,0,0), 0, PrecisionT::INT4); //src
    request->addOperand(sys->getAddress(1,0,8), 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::RowLoad);
    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT8); //src
    request->addOperand(sys->getAddress(1,0,0), 0, PrecisionT::INT8); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::RowMul);
    request->addOperand(sys->getAddress(1,0,0), 0, PrecisionT::INT4); //src
    request->addOperand(sys->getAddress(1,0,8), 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::RowShift);
    request->addOperand(sys->getAddress(1,0,0), 0, PrecisionT::INT4); //src
    request->addOperand(sys->getAddress(1,0,8), 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::Wait, sys->m1);
    request->addOperand(sys->getAddress(1,0,0), 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    request = new Request(Request::Type::RowStore);
    request->addOperand(sys->getAddress(1,0,0), 0, PrecisionT::INT4); //src
    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::Barrier, sys->m2);
    request->addOperand(sys->getAddress(1,0,0), 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    request = new Request(Request::Type::RowMul);
    request->addOperand(sys->getAddress(1,0,0), 0, PrecisionT::INT4); //src
    request->addOperand(sys->getAddress(1,0,8), 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::RowLoad);
    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //src
    request->addOperand(sys->getAddress(1,0,0), 0, PrecisionT::INT4); //dst
    requests.push_back(*request);


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

/////////////////////////////////////////////////////////////
// Simple program to perform an test filter
/////////////////////////////////////////////////////////////

int32_t test(System* sys)
{
    test_tile0(sys);
    test_tile1(sys);
    return 0;
}

static __attribute__((unused)) Registry::Entry &__test__ = pimsim::registerFunc("test", test);
