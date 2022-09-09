// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"
/////////////////////////////////////////////////////////////
// Ignore this benchmark
/////////////////////////////////////////////////////////////

int32_t fir_new(System* sys)
{
    std::vector<Request> requests;
    Request *request;

    int input_size = 256*128*4;
    int filter_size = 257;


    //Load filter coefficients into the RF.
    //This step is typically done beforehand/offline.
    //Also, RFs in all cores will have the same coefficients.
    //So, we can brodcast.
    //When we are broadcasting though, other tiles shouldn't do anything.
    //So they will wait for a semaphore.
    //Assume the number of weights is 32

    //tile0
    request = new Request(Request::Type::RowLoad_RF, Request::BroadcastType::ALL);
    request->addOperand(0, sys->_num_regs_per_rf, PrecisionT::INT4); //dst
    request->addOperand(sys->DRAM_ADDR, sys->_num_regs_per_rf, PrecisionT::INT4); //src
    requests.push_back(*request);
    
    //Signal that the load of filter coefficients from DRAM
    //to RFs of all tiles is complete.
    request = new Request(Request::Type::Signal, sys->m1);
    request->addOperand(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    //tile1,2,3,...
    for(int tile=1; tile<sys->_ntiles; tile++){
        //Wait until the broadcast of filter coefficients
        request = new Request(Request::Type::Wait, sys->m1);
        request->addOperand(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //src
        requests.push_back(*request);
    }

    for(int tile=0; tile<sys->_ntiles; tile++){
        //Now we load inputs from DRAM
        //one load for each tile is enough.
        request = new Request(Request::Type::RowLoad);
        request->addOperand(sys->getAddress(tile,0,0), 0, PrecisionT::INT4); //dst
        request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //src
        requests.push_back(*request); 
        //Initialize rows that'll hold the accumulator (accumulator size=16)
        request = new Request(Request::Type::RowReset);
        request->addOperand(sys->getAddress(tile,0,12), 0, PrecisionT::INT16); //src
        requests.push_back(*request);

        //Loop over the following set of instructions N times,
        //where N is the number of filter coefficients
        for (int i=0; i< sys->_num_regs_per_rf; i++) {

            //Read the coefficient we want to multiply with from the RF.
            //This is not required if we make the RF out of flops.
            //Well, we can handle that internally (if we implement using flops, we
            //can make the cycles consumed by this Request to be 0)
            request = new Request(Request::Type::RowRead_RF);
            request->addOperand(sys->getAddress(tile,0,0), 0, PrecisionT::INT4); //src
            requests.push_back(*request);

            //Multiply filter coefficient (in RF) with the inputs stored in CRAM.
            //Product will be stored in some rows
            request = new Request(Request::Type::RowMul_CRAM_RF);
            request->addOperand(sys->getAddress(tile,0,0), 0, PrecisionT::INT4); //src
            request->addOperand(sys->getAddress(tile,0,4), 0, PrecisionT::INT8); //dst
            requests.push_back(*request);

            //Now, add the product generated above into the accumulator rows
            request = new Request(Request::Type::RowAdd);
            request->addOperand(sys->getAddress(tile,0,4), 0, PrecisionT::INT16); //src
            request->addOperand(sys->getAddress(tile,0,12), 0, PrecisionT::INT16); //dst
            requests.push_back(*request);

            //Shift the inputs by 1 column to the left.
            //No need to loop for each bit. That is a part of the RowShift instruction's semantics.
            request = new Request(Request::Type::RowShift);
            request->addOperand(sys->getAddress(tile,0,0), 0, PrecisionT::INT4);
            requests.push_back(*request);

            //Now store the results back into DRAM
            //Only 1 result is present per column.
            //So, just 1 RowStore instruction is enough.
            request = new Request(Request::Type::RowStore);
            request->addOperand(sys->getAddress(tile,0,12), 0, PrecisionT::INT16); //src
            request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); //dst
            requests.push_back(*request);
        }
    }

   

    

    

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);

    return 0;
}

static __attribute__((unused)) Registry::Entry &__fir_new__ = pimsim::registerFunc("fir_new", fir_new);

