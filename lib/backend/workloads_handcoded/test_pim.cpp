// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

int32_t test_pim(System* sys){
    std::vector<Request> requests;
    Request *request;

    int array_size = 256*128*sys->_config->_ntiles_used;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_output = PrecisionT::INT8;
    int use_tiles = sys->_config->_ntiles_used;
    int dram_tile = sys->_config->_dramTile;

    int word_size = sys->_config->_nblocks*sys->_config->_ncols;
    for(int tile=0; word_size * tile < array_size; tile++){
        int myTile = (dram_tile + tile) % use_tiles;

        request = new Request(Request::Type::RowAdd);
        request->addOperand(sys->getAddress(myTile,0,0), 0, precision_input);//src1
        request->addOperand(sys->getAddress(myTile,0,precision_input.bits()), 0, precision_input);//src2
        request->addOperand(sys->getAddress(myTile,0,precision_input.bits()+precision_input.bits()), 0, precision_output);//dst
        requests.push_back(*request);
    }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}



static __attribute__((unused)) Registry::Entry &__test_pim__ = pimsim::registerFunc("test_pim", test_pim);