// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <numeric>
#include "backend/System.h"

int32_t vadd_norf(System* sys){
    std::vector<Request> requests;
    Request *request;

    //int array_size = 256*128*sys->_config->_ntiles_used;
    int array_size = 120*256*512;
    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_output = PrecisionT::INT8;
    
    request = new Request(Request::Type::RowLoad);
        request->addOperand(sys->getAddress(0,0,0), 0, precision_input);//dst
        request->addOperand(sys->DRAM_ADDR,0, precision_input);//src
        requests.push_back(*request);
    request = new Request(Request::Type::RowLoad_RF);
        request->addOperand(sys->_config->_num_regs_per_rf * 0, 4,PrecisionT::INT8); //RF addr
        request->addOperand(sys->DRAM_ADDR, 4, PrecisionT::INT8); //dram addr
        requests.push_back(*request);
    request = new Request(Request::Type::RowAdd_CRAM_RF);
        request->addOperand(sys->getAddress(0,0,0), 0, precision_input); //src
        request->addOperand(sys->_config->_num_regs_per_rf * 0, 0, precision_input);//rf
        request->addOperand(sys->getAddress(0,0,0), 0, precision_output); //dst
        requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}

int32_t rowload_shuffle(System* sys){
    std::vector<Request> requests;
    Request *request;

    PrecisionT::Precision precision_input = PrecisionT::INT8;
    PrecisionT::Precision precision_output = PrecisionT::INT8;
    request = new Request(Request::Type::RowLoad);
                request->addOperand(sys->getAddress(0,0,0), sys->_config->_ncols, precision_input); //dst
                request->addOperand(sys->DRAM_ADDR, sys->_config->_ncols, precision_input); //src
                request->setShuffle(false, true, 0, 1);
                requests.push_back(*request);
                
    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}

int32_t shift(System* sys){
    std::vector<Request> requests;
    Request *request;

    PrecisionT::Precision precision_input = PrecisionT::INT8;
    request = new Request(Request::Type::RowShift);
        request->addOperand(sys->getAddress(0,0,0), 1, precision_input); //src
        requests.push_back(*request);
                
    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}


int32_t ablation_broadcast(System* sys){
    std::vector<Request> requests;
    Request *request;

    PrecisionT::Precision precision_input = PrecisionT::INT8;
    
    std::vector<int> v(sys->_config->_meshHeight*sys->_config->_meshWidth);
        std::iota (std::begin(v), std::end(v), 0); // Fill with 0, 1, ...
        //sys->broadcast_p2p(sys->getAddress(0,0,0),PrecisionT::INT8, v, sys->_config->_nblocks*sys->_config->_ncols);
        sys->broadcast_p2p(sys->getAddress(0,0,0),PrecisionT::INT8, v,1024);        

    return 0;
}


static __attribute__((unused)) Registry::Entry &__vadd_norf__ = pimsim::registerFunc("vadd_norf", vadd_norf);
static __attribute__((unused)) Registry::Entry &__rowload_shuffle___ = pimsim::registerFunc("rowload_shuffle", rowload_shuffle);
static __attribute__((unused)) Registry::Entry &__shift___ = pimsim::registerFunc("shift", shift);
static __attribute__((unused)) Registry::Entry &__ablation_broadcast___ = pimsim::registerFunc("ablation_broadcast", ablation_broadcast);
