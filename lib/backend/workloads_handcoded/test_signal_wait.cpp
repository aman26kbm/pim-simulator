// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"


int32_t test_signal_wait(System* sys){
    std::vector<Request> requests;
    Request *request;

    PrecisionT::Precision precision = PrecisionT::INT4;
    int use_tiles = sys->_config->_ntiles_used;
    int size = 256*64;
    for(int i=1; i<use_tiles; i++){
            
            request = new Request(Request::Type::Signal);
                            request->addOperand(sys->getAddress(0,0,0), 0, precision); //src
                            request->addOperand(sys->getAddress(i,0,0), 0, precision); //dst
                            requests.push_back(*request);
            request = new Request(Request::Type::Wait);
                            request->addOperand(sys->getAddress(0,0,0), 0, precision); //src
                            request->addOperand(sys->getAddress(i,0,0), 0, precision); //dst
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

    return 0;
}

int32_t test_signal_wait_phased(System* sys){
    std::vector<Request> requests;
    Request *request;

    PrecisionT::Precision precision = PrecisionT::INT4;
    int use_tiles = sys->_config->_ntiles_used;
    int size = 256*64;
    int tile=1;
    while(tile<use_tiles){
        int thisTile = tile;
        for(int i=0; i<sys->_config->_router_channel_number; i++){ 
            if(thisTile<use_tiles){
                request = new Request(Request::Type::Signal);
                                request->addOperand(sys->getAddress(0,0,0), 0, precision); //src
                                request->addOperand(sys->getAddress(thisTile,0,0), 0, precision); //dst
                                requests.push_back(*request);
                request = new Request(Request::Type::Wait);
                                request->addOperand(sys->getAddress(0,0,0), 0, precision); //src
                                request->addOperand(sys->getAddress(thisTile,0,0), 0, precision); //dst
                                requests.push_back(*request);

                thisTile++;
            }
        }
        for(int i=0; i<sys->_config->_router_channel_number; i++){
            if(tile<use_tiles){
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
    }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);

    return 0;
}


static __attribute__((unused)) Registry::Entry &__test_signal_wait__ = pimsim::registerFunc("test_signal_wait", test_signal_wait);
static __attribute__((unused)) Registry::Entry &__test_signal_wait_phased__ = pimsim::registerFunc("test_signal_wait_phased", test_signal_wait_phased);
