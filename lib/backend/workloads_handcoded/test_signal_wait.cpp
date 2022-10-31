// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"


int32_t test_signal_wait(System* sys){
    std::vector<Request> requests;
    Request *request;

    PrecisionT::Precision precision = PrecisionT::INT8;
    int use_tiles = sys->_config->_ntiles_used;
    int size = 128*4;
    for(int i=1; i<use_tiles; i++){
            
            request = new Request(Request::Type::Signal);
                            request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT16); //src
                            request->addOperand(sys->getAddress(i,0,0), 0, PrecisionT::INT16); //dst
                            requests.push_back(*request);
            request = new Request(Request::Type::Wait);
                            request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT16); //src
                            request->addOperand(sys->getAddress(i,0,0), 0, PrecisionT::INT16); //dst
                            requests.push_back(*request);

            request = new Request(Request::Type::TileSend);
            request->addOperand(sys->getAddress(i,0,0), size, precision);//src
            request->addOperand(sys->getAddress(0,0,0),size, precision);//DST
            requests.push_back(*request);

            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(i,0,0), size, precision);//src
            request->addOperand(sys->getAddress(0,0,0), size, precision);//DST
            requests.push_back(*request);
        
    }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

int32_t test_signal_wait_phased(System* sys){
    std::vector<Request> requests;
    Request *request;

    PrecisionT::Precision precision = PrecisionT::INT8;
    int use_tiles = sys->_config->_ntiles_used;
    int size = 128*4;
    int tile=1;
    for(int phase=1; phase<use_tiles/sys->_config->_router_channel_number; phase++){
        int thisTile = tile;
        for(int i=0; i<sys->_config->_router_channel_number; i++){ 
            request = new Request(Request::Type::Signal);
                            request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT16); //src
                            request->addOperand(sys->getAddress(thisTile,0,0), 0, PrecisionT::INT16); //dst
                            requests.push_back(*request);
            request = new Request(Request::Type::Wait);
                            request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT16); //src
                            request->addOperand(sys->getAddress(thisTile,0,0), 0, PrecisionT::INT16); //dst
                            requests.push_back(*request);

            thisTile++;
        }
        for(int i=0; i<sys->_config->_router_channel_number; i++){
            request = new Request(Request::Type::TileSend);
            request->addOperand(sys->getAddress(tile,0,0), size, precision);//src
            request->addOperand(sys->getAddress(0,0,0),size, precision);//DST
            requests.push_back(*request);

            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(tile,0,0), size, precision);//src
            request->addOperand(sys->getAddress(0,0,0), size, precision);//DST
            requests.push_back(*request);
            tile++;
        }
    }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}


static __attribute__((unused)) Registry::Entry &__test_signal_wait__ = pimsim::registerFunc("test_signal_wait", test_signal_wait);
static __attribute__((unused)) Registry::Entry &__test_signal_wait_phased__ = pimsim::registerFunc("test_signal_wait_phased", test_signal_wait_phased);
