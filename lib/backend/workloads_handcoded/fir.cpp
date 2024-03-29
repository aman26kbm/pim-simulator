// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"
/////////////////////////////////////////////////////////////
// FIR filter micro benchmark
/////////////////////////////////////////////////////////////

void fir_tile(System* sys)
{
    std::vector<Request> requests;
    Request *request;

    int size_input = 256*128*4;
    int size_filter = 257;

    PrecisionT::Precision precision_input = PrecisionT::INT16;
    PrecisionT::Precision precision_multiply = PrecisionT::INT32;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT32;

    Config* cfg = sys->_config;
    int use_tiles = cfg->_ntiles_used;
    int dram_tile = 0; //This specifies the location of the DRAM controller (0 implies core 0 is connected to DRAM controller)


    for(int iter_row_load=0; iter_row_load<(int)ceil(size_input/(double)(cfg->_nblocks * cfg->_ncols - size_filter)); iter_row_load++){
        //Now we load inputs from DRAM
        //Currently, we're assuming just one load is enough.
        //Only one input in each column across all cores.
        request = new Request(Request::Type::RowLoad);

        request->addOperand(sys->getAddress(0,0,0), 0, precision_input); //dst
        request->addOperand(sys->DRAM_ADDR, 0, precision_input); //src

        requests.push_back(*request);

        //Initialize rows that'll hold the accumulator (accumulator size=16)
        request = new Request(Request::Type::RowReset);

        request->addOperand(sys->getAddress(0,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //src
        requests.push_back(*request);

        int iter_load_rf_count = (int)ceil(size_filter/(double)cfg->_num_regs_per_rf);

        for(int iter_load_rf=0; iter_load_rf<iter_load_rf_count; iter_load_rf++){
            //Load filter coefficients into the RF.
            //This step is typically done beforehand/offline.
            //Also, RFs in all cores will have the same coefficients.
            //So, we can brodcast.
            //When we are broadcasting though, other tiles shouldn't do anything.
            //So they will wait for a semaphore.
            //Assume the number of weights is 8
            request = new Request(Request::Type::RowLoad_RF, Request::BroadcastType::ALL);

            request->addOperand(0, 32, precision_input); //dst
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
                request->addOperand(sys->getAddress(0,0,0), 0, precision_input); //src
                request->addOperand(i, 0, precision_input); //src
                request->addOperand(sys->getAddress(0,0,precision_input.bits()), 0, precision_multiply); //dst
                requests.push_back(*request);

                //Now, add the product generated above into the accumulator rows
                request = new Request(Request::Type::RowAdd);
                request->addOperand(sys->getAddress(0,0,precision_input.bits()), 0, precision_multiply); //src
                request->addOperand(sys->getAddress(0,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //src
                request->addOperand(sys->getAddress(0,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //dst
                requests.push_back(*request);

                //Shift the inputs by 1 column to the left.
                //No need to loop for each bit. That is a part of the RowShift instruction's semantics.
                request = new Request(Request::Type::RowShift);
                request->addOperand(sys->getAddress(0,0,0), 0, precision_input); //src
                request->addOperand(sys->getAddress(0,0,0), 0, precision_input); //dst
                requests.push_back(*request);
                }
            }
        }
        //Now store the results back into DRAM
        //Only 1 result is present per column.
        //So, just 1 RowStore instruction is enough.
        request = new Request(Request::Type::RowStore);
        request->addOperand(sys->getAddress(0,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //src
        request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); //dst

        requests.push_back(*request);
    }

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

/////////////////////////////////////////////////////////////
// Simple program to perform an FIR filter
/////////////////////////////////////////////////////////////

int32_t fir(System* sys)
{
    //We have 2 tiles
    //Each has 4 CRAMs.
    //There are 256 columns in each CRAM.
    //So, total columns is 1024.
    //Let's consider the input length to be 1024*2 = 2048 (minus some halo).
    //And filter size of say 8 .

    //The coefficients are outside in RF (same coefficients are broadcasted in all cores).
    //The input is loaded into CRAMs.
    //Just one element per column of each CRAM.

    //The result's length will be 2048 (minus halo).
    //One element in each column of each CRAM.
    //fir_tile0(sys);
    // fir_tile1(sys);
    // fir_tile2(sys);
    // fir_tile3(sys);
    fir_tile(sys);
    return 0;
}

static __attribute__((unused)) Registry::Entry &__fir__ = pimsim::registerFunc("fir", fir);

