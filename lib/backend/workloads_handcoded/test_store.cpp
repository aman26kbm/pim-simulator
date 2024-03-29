// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"


int32_t test_store_simple(System* sys){
    std::vector<Request> requests;
    Request *request;

    PrecisionT::Precision precision = PrecisionT::INT8;
    int use_tiles = sys->_config->_ntiles_used;

    
    int myTile = use_tiles-1;
    request = new Request(Request::Type::RowStore);
    request->addOperand(sys->getAddress(myTile,0,0), 0, precision);//src
    request->addOperand(sys->DRAM_ADDR, 0, precision);//DST
    requests.push_back(*request);

    // for(int i=0; i<5; i++){
    //     request = new Request(Request::Type::RowAdd);
    //     request->addOperand(sys->getAddress(0,0,0), 0, precision);//src
    //     request->addOperand(sys->getAddress(0,0,0), 0, precision);//src
    //     request->addOperand(sys->getAddress(0,0,0), 0, precision);//src
    //     requests.push_back(*request);
    // }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}

int32_t test_store_all(System* sys){
    std::vector<Request> requests;
    Request *request;

    PrecisionT::Precision precision = PrecisionT::INT8;
    int use_tiles = sys->_config->_ntiles_used;

    for(int tile=0; tile < use_tiles; tile++){
        int myTile = (tile) % use_tiles;
        
        request = new Request(Request::Type::RowStore);
        request->addOperand(sys->getAddress(myTile,0,0), 0, precision);//src
        request->addOperand(sys->DRAM_ADDR, 0, precision);//DST
        requests.push_back(*request);
        
    }


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);

    return 0;
}

int32_t test_store_concurrent(System* sys){
    int round = 128;
    for(int i=0; i<round; i++){
        test_store_simple(sys);
    }
    return 0;
}

int32_t test_store_load(System* sys){
    std::vector<Request> requests;
    Request *request;

    PrecisionT::Precision precision = PrecisionT::INT8;
    int use_tiles = sys->_config->_ntiles_used;

    
    int myTile = 0;
    request = new Request(Request::Type::RowStore);
    request->addOperand(sys->getAddress(myTile,0,0), 0, precision);//src
    request->addOperand(sys->DRAM_ADDR, 0, precision);//DST
    requests.push_back(*request);

    request = new Request(Request::Type::RowLoad);
    request->addOperand(sys->getAddress(myTile,0,0), 0, precision);//src
    request->addOperand(sys->DRAM_ADDR, 0, precision);//DST
    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}


static __attribute__((unused)) Registry::Entry &__test_store_simple__ = pimsim::registerFunc("test_store_simple", test_store_simple);
static __attribute__((unused)) Registry::Entry &__test_store_all__ = pimsim::registerFunc("test_store_all", test_store_all);
static __attribute__((unused)) Registry::Entry &__test_store_concurrent__ = pimsim::registerFunc("test_store_concurrent", test_store_concurrent);
static __attribute__((unused)) Registry::Entry &__test_store_load__ = pimsim::registerFunc("test_store_load", test_store_load);