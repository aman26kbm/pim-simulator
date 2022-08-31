// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t vadd(System* sys){
    std::vector<Request> requests;
    Request *request;

    int array_size = 256*128;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_output = PrecisionT::INT8;
    int dram_tile=64;
    int use_tiles=1;

    for(int tile=dram_tile; tile<dram_tile+use_tiles; tile++){              
        request = new Request(Request::Type::RowLoad);
        request->addAddr(sys->getAddress(tile%sys->_ntiles,0,0), 0, precision_input);//dst
        request->addAddr(sys->DRAM_ADDR,0, precision_input);//src
        requests.push_back(*request);

        request = new Request(Request::Type::RowLoad);
        request->addAddr(sys->getAddress(tile%sys->_ntiles,0,precision_input.bits()), 0, precision_input);//dst
        request->addAddr(sys->DRAM_ADDR,0, precision_input);//src
        requests.push_back(*request);

        request = new Request(Request::Type::RowAdd);
        request->addAddr(sys->getAddress(tile%sys->_ntiles,0,0), 0, precision_input);//src1
        request->addAddr(sys->getAddress(tile%sys->_ntiles,0,precision_input.bits()), 0, precision_input);//src2
        request->addAddr(sys->getAddress(tile%sys->_ntiles,0,precision_input.bits()+precision_input.bits()), 0, precision_output);//dst
        requests.push_back(*request);

        request = new Request(Request::Type::RowStore);
        request->addAddr(sys->getAddress(tile%sys->_ntiles,0,precision_input.bits()+precision_input.bits()), 0, precision_output);//dst
        request->addAddr(sys->DRAM_ADDR,0, precision_output);//src
        requests.push_back(*request);
    }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}



static __attribute__((unused)) Registry::Entry &__vadd__ = pimsim::registerFunc("vadd", vadd);