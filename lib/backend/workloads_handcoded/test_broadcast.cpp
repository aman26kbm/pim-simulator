// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <numeric>

#include "backend/System.h"

int32_t test_broadcast(System* sys){
    std::vector<Request> requests;
    Request *request;

    for(int i=0; i<10; i++){         
        request = new Request(Request::Type::TileSend_BroadCast);
        request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT4); //src
        requests.push_back(*request);
        // request = new Request(Request::Type::Signal, sys->m1);
        // request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT4); //src
        // requests.push_back(*request);

        // request = new Request(Request::Type::Wait, sys->m1);
        // request->addOperand(sys->getAddress(1,0,0), 0, PrecisionT::INT4); //src
        // requests.push_back(*request);
        request = new Request(Request::Type::TileReceive_BroadCast);
        request->addOperand(sys->getAddress(1,0,0), 0, PrecisionT::INT4); //src
        requests.push_back(*request);

        // request = new Request(Request::Type::Wait, sys->m1);
        // request->addOperand(sys->getAddress(2,0,0), 0, PrecisionT::INT4); //src
        // requests.push_back(*request);
        request = new Request(Request::Type::TileReceive_BroadCast);
        request->addOperand(sys->getAddress(2,0,0), 0, PrecisionT::INT4); //src
        requests.push_back(*request);

        // request = new Request(Request::Type::Wait, sys->m1);
        // request->addOperand(sys->getAddress(3,0,0), 0, PrecisionT::INT4); //src
        // requests.push_back(*request);
        request = new Request(Request::Type::TileReceive_BroadCast);
        request->addOperand(sys->getAddress(3,0,0), 0, PrecisionT::INT4); //src
        requests.push_back(*request);
    }

    request = new Request(Request::Type::BlockBroadCast);
    request->addOperand(sys->getAddress(0,0,0), 1, PrecisionT::INT4); //src
    requests.push_back(*request);

    request = new Request(Request::Type::BlockBroadCast);
    request->addOperand(sys->getAddress(1,0,0), 2, PrecisionT::INT4); //src
    requests.push_back(*request);

    request = new Request(Request::Type::BlockBroadCast);
    request->addOperand(sys->getAddress(2,0,0), 4, PrecisionT::INT4); //src
    requests.push_back(*request);

    request = new Request(Request::Type::BlockBroadCast);
    request->addOperand(sys->getAddress(3,0,0), 8, PrecisionT::INT4); //src
    requests.push_back(*request);

                    


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

int32_t test_broadcast_dmesh(System* sys){
    PrecisionT::Precision p = PrecisionT::INT8;
    for(int i=0; i<10; i++){
        std::vector<int> v(sys->_config->_meshHeight*sys->_config->_meshWidth);
        std::iota (std::begin(v), std::end(v), 0); // Fill with 0, 1, ...
        sys->broadcast(sys->getAddress(1,0,i),PrecisionT::INT8, v, sys->_config->_nblocks*sys->_config->_ncols);
    }
}

int32_t test_broadcast_dmesh_p2p(System* sys){
    PrecisionT::Precision p = PrecisionT::INT8;
    for(int i=0; i<10; i++){
        std::vector<int> v(sys->_config->_meshHeight*sys->_config->_meshWidth);
        std::iota (std::begin(v), std::end(v), 0); // Fill with 0, 1, ...
        sys->broadcast_p2p(sys->getAddress(1,0,i),PrecisionT::INT8, v, sys->_config->_nblocks*sys->_config->_ncols);
    }
}




static __attribute__((unused)) Registry::Entry &__test_broadcast__ = pimsim::registerFunc("test_broadcast", test_broadcast);
static __attribute__((unused)) Registry::Entry &__test_broadcast_dmesh__ = pimsim::registerFunc("test_broadcast_dmesh", test_broadcast_dmesh);
static __attribute__((unused)) Registry::Entry &__test_broadcast_dmesh_p2p__ = pimsim::registerFunc("test_broadcast_dmesh_p2p", test_broadcast_dmesh_p2p);