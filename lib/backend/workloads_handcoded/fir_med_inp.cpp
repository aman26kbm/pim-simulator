// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <cmath>

#include "backend/System.h"
/////////////////////////////////////////////////////////////
// FIR filter micro benchmark
/////////////////////////////////////////////////////////////

int32_t fir_med_inp(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    //int size_input = 256*128*128;
    //int size_filter = 257;

    int size_input = 120*256*255;
    int size_filter = 32;

    PrecisionT::Precision precision_input = PrecisionT::INT16;
    PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;

    int use_tiles = cfg->_ntiles_used;

    //Each core will load 256 arrays worth of data
    //The last array's data will be wasted
    //The other 255 will compute 256*255 output elements

    int param = 255; //this is the last multiplier in the input_size

    int elements_in_1_tile = 256*(param+1);

    //Let's express spatial calculation in cores 0 through 119 first
    for(int tile=0; tile<use_tiles; tile++) {
        //Now we load inputs from DRAM
        request = new Request(Request::Type::RowLoad);
        request->addOperand(sys->getAddress(tile,0,0), (param+1) * cfg->_ncols, precision_input); //dst
        request->addOperand(sys->DRAM_ADDR, (param+1) * cfg->_ncols, precision_input); //src
        requests.push_back(*request);

        //Initialize rows that'll hold the accumulator (accumulator size=16)
        request = new Request(Request::Type::RowReset);
        request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), (param+1)*cfg->_ncols, precision_accumulate); //src
        requests.push_back(*request);

        int iter_load_rf_count = (int)ceil((float)size_filter/(float)cfg->_num_regs_per_rf);

        for(int iter_load_rf=0; iter_load_rf<iter_load_rf_count; iter_load_rf++){
            //Load filter coefficients into the RF.
            //This step is typically done beforehand/offline.
            //Also, RFs in all tiles will have the same coefficients.
            //So, we can brodcast.
            //When we are broadcasting though, other tiles shouldn't do anything.
            //So they will wait for a semaphore.
            //Assume the number of weights is 8

            //For now, we don't support BroadcastType for requests in the dynaMesh.
            //So reading the filters every time from DRAM
            //if (tile == 0) {
            //    request = new Request(Request::Type::RowLoad_RF, Request::BroadcastType::ALL);
            //    request->addOperand(0, 32, precision_input); //dst
            //    request->addOperand(sys->DRAM_ADDR,32, precision_input); //src
            //    requests.push_back(*request);   

            //    request = new Request(Request::Type::Signal, sys->m1);
            //    request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT4); //src
            //    requests.push_back(*request);
            //} 
            //else {
            //    request = new Request(Request::Type::Wait, sys->m1);
            //    request->addOperand(sys->getAddress(tile,0,0), 0, PrecisionT::INT4); //src
            //    requests.push_back(*request);
            //}

            request = new Request(Request::Type::RowLoad_RF);
            request->addOperand(cfg->_num_regs_per_rf * tile,  32, precision_input); //dst
            request->addOperand(sys->DRAM_ADDR,32, precision_input); //src
            requests.push_back(*request);   

            //Loop over the following set of instructions N times,
            //where N is the number of filter coefficients
            for (int i=0; i< 32; i++) {
                if(iter_load_rf*cfg->_num_regs_per_rf+i<size_filter){

                //Read the coefficient we want to multiply with from the RF.
                //This is not required if we make the RF out of flops.
                //Well, we can handle that internally (if we implement using flops, we
                //can make the cycles consumed by this Request to be 0)
                request = new Request(Request::Type::RowRead_RF);
                request->addOperand(i, 0, precision_input); //src
                requests.push_back(*request);

                //Multiply filter coefficient (in RF) with the inputs stored in CRAM.
                //Product will be stored in some rows
                request = new Request(Request::Type::RowMul_CRAM_RF);
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                request->addOperand(i, 0, precision_input); //src
                request->addOperand(sys->getAddress(tile,0,precision_input.bits()), 0, precision_multiply); //dst
                requests.push_back(*request);

                //Now, add the product generated above into the accumulator rows
                request = new Request(Request::Type::RowAdd);
                request->addOperand(sys->getAddress(tile,0,precision_input.bits()), 0, precision_multiply); //src
                request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //src
                request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //dst
                requests.push_back(*request);

                //Shift the inputs by 1 column to the left.
                //No need to loop for each bit. That is a part of the RowShift instruction's semantics.
                request = new Request(Request::Type::RowShift);
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //src
                request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //dst
                requests.push_back(*request);
                }
            }
        }
        //Now store the results back into DRAM
        //Only 1 result is present per column.
        //So, just 1 RowStore instruction is enough.
        request = new Request(Request::Type::RowStore);
        request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), param * cfg->_ncols, precision_accumulate); //src
        request->addOperand(sys->DRAM_ADDR, param * cfg->_ncols, PrecisionT::INT16); //dst

        requests.push_back(*request);
    }


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

/////////////////////////////////////////////////////////////
// Simple program to perform an FIR filter
/////////////////////////////////////////////////////////////


static __attribute__((unused)) Registry::Entry &__fir_med_inp__ = pimsim::registerFunc("fir_med_inp", fir_med_inp);

