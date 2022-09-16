// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t test_send_receive_cross(System* sys){
    std::vector<Request> requests;
    Request *request;

    PrecisionT::Precision precision = PrecisionT::INT8;
    int use_tiles = sys->_config->_ntiles_used;

    for(int tile=0; tile < use_tiles; tile++){
        int myTile = (tile) % use_tiles;
        if(myTile%2==0){
            request = new Request(Request::Type::TileSend);
            request->addOperand(sys->getAddress(myTile,0,0), 0, precision);//src
            request->addOperand(sys->getAddress(myTile+1,0,0), 0, precision);//DST
            requests.push_back(*request);
        }
        else{
            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(myTile-1,0,0), 0, precision);//src
            request->addOperand(sys->getAddress(myTile,0,0), 0, precision);//DST
            requests.push_back(*request);
        }
    }

    for(int tile=0; tile < use_tiles; tile++){
        int myTile = (tile) % use_tiles;
        if(myTile%2==0){
            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(myTile+1,0,0), 0, precision);//src
            request->addOperand(sys->getAddress(myTile,0,0), 0, precision);//DST
            requests.push_back(*request);
        }
        else{
            request = new Request(Request::Type::TileSend);
            request->addOperand(sys->getAddress(myTile,0,0), 0, precision);//src
            request->addOperand(sys->getAddress(myTile-1,0,0), 0, precision);//DST
            requests.push_back(*request);
        }
    }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

int32_t test_send_receive_concurrent(System* sys){
    std::vector<Request> requests;
    Request *request;

    PrecisionT::Precision precision = PrecisionT::INT8;
    int use_tiles = sys->_config->_ntiles_used;

    for(int tile=0; tile < use_tiles; tile++){
        int myTile = (tile) % use_tiles;
        if(myTile%2==0){
            request = new Request(Request::Type::TileSend);
            request->addOperand(sys->getAddress(myTile,0,0), 0, precision);//src
            request->addOperand(sys->getAddress(myTile+1,0,0), 0, precision);//DST
            requests.push_back(*request);

            request = new Request(Request::Type::TileSend);
            request->addOperand(sys->getAddress(myTile,0,8), 0, precision);//src
            request->addOperand(sys->getAddress(myTile+1,0,8), 0, precision);//DST
            requests.push_back(*request);

        }
        else{
            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(myTile-1,0,8), 0, precision);//src
            request->addOperand(sys->getAddress(myTile,0,8), 0, precision);//DST
            requests.push_back(*request);

            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(myTile-1,0,8), 0, precision);//src
            request->addOperand(sys->getAddress(myTile,0,8), 0, precision);//DST
            requests.push_back(*request);
        }
    }


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

int32_t test_send_receive(System* sys){
    test_send_receive_concurrent(sys);
    test_send_receive_cross(sys);
}


static __attribute__((unused)) Registry::Entry &__test_send_receive_concurrent__ = pimsim::registerFunc("test_send_receive_concurrent", test_send_receive_concurrent);
static __attribute__((unused)) Registry::Entry &__test_send_receive_cross__ = pimsim::registerFunc("test_send_receive_cross", test_send_receive_cross);
static __attribute__((unused)) Registry::Entry &__test_send_receive__ = pimsim::registerFunc("test_send_receive", test_send_receive);