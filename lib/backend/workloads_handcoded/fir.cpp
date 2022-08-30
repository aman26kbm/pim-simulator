// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"
/////////////////////////////////////////////////////////////
// Queuing requests for tile0
/////////////////////////////////////////////////////////////

void fir_tile0(System* sys)
{
    std::vector<Request> requests;
    Request *request;

    //Load filter coefficients into the RF.
    //This step is typically done beforehand/offline.
    //Also, RFs in all cores will have the same coefficients.
    //So, we can brodcast.
    //When we are broadcasting though, other tiles shouldn't do anything.
    //So they will wait for a semaphore.
    //Assume the number of weights is 8
    request = new Request(Request::Type::RowLoad_RF, Request::BroadcastType::ALL);
    request->addOperand(sys->rf_base_addr_tile0, 8, PrecisionT::INT4); //dst
    request->addOperand(sys->DRAM_ADDR, 8, PrecisionT::INT4); //src
    requests.push_back(*request);

    //Signal that the load of filter coefficients from DRAM
    //to RFs of all tiles is complete.
    request = new Request(Request::Type::Signal, sys->m1);
    request->addOperand(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    //Now we load inputs from DRAM
    //Currently, we're assuming just one load is enough.
    //Only one input in each column across all cores.
    request = new Request(Request::Type::RowLoad);
    request->addOperand(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT4); //dst
    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    //Initialize rows that'll hold the accumulator (accumulator size=16)
    request = new Request(Request::Type::RowReset);
    request->addOperand(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT16); //src
    request->addOperand(sys->cram_addr_tile0_block0_row8, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    //Loop over the following set of instructions N times,
    //where N is the number of filter coefficients
    for (int i=0; i< 8; i++) {

        //Read the coefficient we want to multiply with from the RF.
        //This is not required if we make the RF out of flops.
        //Well, we can handle that internally (if we implement using flops, we
        //can make the cycles consumed by this Request to be 0)
        request = new Request(Request::Type::RowRead_RF);
        request->addOperand(sys->rf_base_addr_tile0, 0, PrecisionT::INT16); //src
        requests.push_back(*request);

        //Multiply filter coefficient (in RF) with the inputs stored in CRAM.
        //Product will be stored in some rows
        request = new Request(Request::Type::RowMul_CRAM_RF);
        request->addOperand(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT4); //src
        request->addOperand(sys->cram_addr_tile0_block0_row8, 0, PrecisionT::INT8); //dst
        requests.push_back(*request);

        //Now, add the product generated above into the accumulator rows
        request = new Request(Request::Type::RowAdd);
        request->addOperand(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT16); //src
        request->addOperand(sys->cram_addr_tile1_block0_row8, 0, PrecisionT::INT16); //dst
        requests.push_back(*request);

        //Shift the inputs by 1 column to the left.
        //No need to loop for each bit. That is a part of the RowShift instruction's semantics.
        request = new Request(Request::Type::RowShift);
        request->addOperand(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT4); //src
        request->addOperand(sys->cram_addr_tile0_block0_row8, 0, PrecisionT::INT4); //dst
        requests.push_back(*request);

    }

    //Now store the results back into DRAM
    //Only 1 result is present per column.
    //So, just 1 RowStore instruction is enough.
    request = new Request(Request::Type::RowStore);
    request->addOperand(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT16); //src
    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

/////////////////////////////////////////////////////////////
// Queuing requests for tile1
/////////////////////////////////////////////////////////////

void fir_tile1(System* sys)
{
    std::vector<Request> requests;
    Request *request;

    //Wait until the broadcast of filter coefficients
    request = new Request(Request::Type::Wait, sys->m1);
    request->addOperand(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    //Now we load inputs from DRAM
    //Currently, we're assuming just one load is enough.
    //Only one input in each column across all cores.
    request = new Request(Request::Type::RowLoad);
    request->addOperand(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //dst
    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    //Initialize rows that'll hold the accumulator (accumulator size=16)
    request = new Request(Request::Type::RowReset);
    request->addOperand(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT16); //src
    request->addOperand(sys->cram_addr_tile1_block0_row8, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    //Loop over the following set of instructions N times,
    //where N is the number of filter coefficients
    for (int i=0; i< 8; i++) {

        //Read the coefficient we want to multiply with from the RF.
        //This is not required if we make the RF out of flops.
        //Well, we can handle that internally (if we implement using flops, we
        //can make the cycles consumed by this Request to be 0)
        request = new Request(Request::Type::RowRead_RF);
        request->addOperand(sys->rf_base_addr_tile1, 0, PrecisionT::INT16); //src
        requests.push_back(*request);

        //Multiply filter coefficient (in RF) with the inputs stored in CRAM.
        //Product will be stored in some rows
        request = new Request(Request::Type::RowMul_CRAM_RF);
        request->addOperand(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //src
        request->addOperand(sys->cram_addr_tile1_block0_row8, 0, PrecisionT::INT4); //dst
        requests.push_back(*request);

        //Now, add the product generated above into the accumulator rows
        request = new Request(Request::Type::RowAdd);
        request->addOperand(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //src
        request->addOperand(sys->cram_addr_tile1_block0_row8, 0, PrecisionT::INT4); //dst
        requests.push_back(*request);

        //Shift the inputs by 1 column to the left.
        //No need to loop for each bit. That is a part of the RowShift instruction's semantics.
        request = new Request(Request::Type::RowShift);
        request->addOperand(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //src
        request->addOperand(sys->cram_addr_tile1_block0_row8, 0, PrecisionT::INT4); //dst
        requests.push_back(*request);
    }

    //Now store the results back into DRAM
    //Only 1 result is present per column.
    //So, just 1 RowStore instruction is enough.
    request = new Request(Request::Type::RowStore);
    request->addOperand(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT16); //src
    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

/////////////////////////////////////////////////////////////
// Queuing requests for tile1
/////////////////////////////////////////////////////////////

void fir_tile2(System* sys)
{
    std::vector<Request> requests;
    Request *request;

    //Wait until the broadcast of filter coefficients
    request = new Request(Request::Type::Wait, sys->m1);
    request->addOperand(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    //Now we load inputs from DRAM
    //Currently, we're assuming just one load is enough.
    //Only one input in each column across all cores.
    request = new Request(Request::Type::RowLoad);
    request->addOperand(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT4); //dst
    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    //Initialize rows that'll hold the accumulator (accumulator size=16)
    request = new Request(Request::Type::RowReset);
    request->addOperand(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT16); //src
    request->addOperand(sys->cram_addr_tile2_block0_row8, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    //Loop over the following set of instructions N times,
    //where N is the number of filter coefficients
    for (int i=0; i< 8; i++) {

        //Read the coefficient we want to multiply with from the RF.
        //This is not required if we make the RF out of flops.
        //Well, we can handle that internally (if we implement using flops, we
        //can make the cycles consumed by this Request to be 0)
        request = new Request(Request::Type::RowRead_RF);
        request->addOperand(sys->rf_base_addr_tile2, 0, PrecisionT::INT16); //src
        requests.push_back(*request);

        //Multiply filter coefficient (in RF) with the inputs stored in CRAM.
        //Product will be stored in some rows
        request = new Request(Request::Type::RowMul_CRAM_RF);
        request->addOperand(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT4); //src
        request->addOperand(sys->cram_addr_tile2_block0_row8, 0, PrecisionT::INT4); //dst
        requests.push_back(*request);

        //Now, add the product generated above into the accumulator rows
        request = new Request(Request::Type::RowAdd);
        request->addOperand(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT4); //src
        request->addOperand(sys->cram_addr_tile2_block0_row8, 0, PrecisionT::INT4); //dst
        requests.push_back(*request);

        //Shift the inputs by 1 column to the left.
        //No need to loop for each bit. That is a part of the RowShift instruction's semantics.
        request = new Request(Request::Type::RowShift);
        request->addOperand(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT4); //src
        request->addOperand(sys->cram_addr_tile2_block0_row8, 0, PrecisionT::INT4); //dst
        requests.push_back(*request);
    }

    //Now store the results back into DRAM
    //Only 1 result is present per column.
    //So, just 1 RowStore instruction is enough.
    request = new Request(Request::Type::RowStore);
    request->addOperand(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT16); //src
    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

/////////////////////////////////////////////////////////////
// Queuing requests for tile3
/////////////////////////////////////////////////////////////

void fir_tile3(System* sys)
{
    std::vector<Request> requests;
    Request *request;

    //Wait until the broadcast of filter coefficients
    request = new Request(Request::Type::Wait, sys->m1);
    request->addOperand(sys->cram_addr_tile3_block0_row0, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    //Now we load inputs from DRAM
    //Currently, we're assuming just one load is enough.
    //Only one input in each column across all cores.
    request = new Request(Request::Type::RowLoad);
    request->addOperand(sys->cram_addr_tile3_block0_row0, 0, PrecisionT::INT4); //dst
    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    //Initialize rows that'll hold the accumulator (accumulator size=16)
    request = new Request(Request::Type::RowReset);
    request->addOperand(sys->cram_addr_tile3_block0_row0, 0, PrecisionT::INT16); //src
    request->addOperand(sys->cram_addr_tile3_block0_row8, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    //Loop over the following set of instructions N times,
    //where N is the number of filter coefficients
    for (int i=0; i< 8; i++) {

        //Read the coefficient we want to multiply with from the RF.
        //This is not required if we make the RF out of flops.
        //Well, we can handle that internally (if we implement using flops, we
        //can make the cycles consumed by this Request to be 0)
        request = new Request(Request::Type::RowRead_RF);
        request->addOperand(sys->rf_base_addr_tile3, 0, PrecisionT::INT16); //src
        requests.push_back(*request);

        //Multiply filter coefficient (in RF) with the inputs stored in CRAM.
        //Product will be stored in some rows
        request = new Request(Request::Type::RowMul_CRAM_RF);
        request->addOperand(sys->cram_addr_tile3_block0_row0, 0, PrecisionT::INT4); //src
        request->addOperand(sys->cram_addr_tile3_block0_row8, 0, PrecisionT::INT4); //dst
        requests.push_back(*request);

        //Now, add the product generated above into the accumulator rows
        request = new Request(Request::Type::RowAdd);
        request->addOperand(sys->cram_addr_tile3_block0_row0, 0, PrecisionT::INT4); //src
        request->addOperand(sys->cram_addr_tile3_block0_row8, 0, PrecisionT::INT4); //dst
        requests.push_back(*request);

        //Shift the inputs by 1 column to the left.
        //No need to loop for each bit. That is a part of the RowShift instruction's semantics.
        request = new Request(Request::Type::RowShift);
        request->addOperand(sys->cram_addr_tile3_block0_row0, 0, PrecisionT::INT4); //src
        request->addOperand(sys->cram_addr_tile3_block0_row8, 0, PrecisionT::INT4); //dst
        requests.push_back(*request);
    }

    //Now store the results back into DRAM
    //Only 1 result is present per column.
    //So, just 1 RowStore instruction is enough.
    request = new Request(Request::Type::RowStore);
    request->addOperand(sys->cram_addr_tile3_block0_row0, 0, PrecisionT::INT16); //src
    request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

void fir_tile0_255(System* sys)
{
    std::vector<Request> requests;
    Request *request;

    int size_input = 256*128*4;
    int size_filter = 257;

    PrecisionT::Precision precision_input = PrecisionT::INT16;
    PrecisionT::Precision precision_multiply = PrecisionT::INT32;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT32;


    for(int iter_row_load=0; iter_row_load<(int)ceil(size_input/(double)(sys->_nblocks * sys->_ncols)); iter_row_load++){
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

        for(int iter_load_rf=0; iter_load_rf<(int)ceil(size_filter/(double)sys->_num_regs_per_rf); iter_load_rf++){
            //Load filter coefficients into the RF.
            //This step is typically done beforehand/offline.
            //Also, RFs in all cores will have the same coefficients.
            //So, we can brodcast.
            //When we are broadcasting though, other tiles shouldn't do anything.
            //So they will wait for a semaphore.
            //Assume the number of weights is 8
            request = new Request(Request::Type::RowLoad_RF, Request::BroadcastType::ALL);
            request->addOperand(sys->rf_base_addr_tile0, 32, precision_input); //dst
            request->addOperand(sys->DRAM_ADDR,32, precision_input); //src
            requests.push_back(*request);

            //Loop over the following set of instructions N times,
            //where N is the number of filter coefficients
            for (int i=0; i< 32; i++) {

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
                request->addOperand(sys->getAddress(0,0,precision_input.bits()+precision_multiply.bits()), 0, precision_multiply); //src
                request->addOperand(sys->getAddress(0,0,precision_input.bits()+precision_multiply.bits()), 0, precision_multiply); //dst
                requests.push_back(*request);

                //Shift the inputs by 1 column to the left.
                //No need to loop for each bit. That is a part of the RowShift instruction's semantics.
                request = new Request(Request::Type::RowShift);
                request->addOperand(sys->getAddress(0,0,0), 0, precision_input); //src
                request->addOperand(sys->getAddress(0,0,0), 0, precision_input); //dst
                requests.push_back(*request);
            }
        }
        //Now store the results back into DRAM
        //Only 1 result is present per column.
        //So, just 1 RowStore instruction is enough.
        request = new Request(Request::Type::RowStore);
        request->addOperand(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT16); //src
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
    fir_tile0_255(sys);
    return 0;
}

static __attribute__((unused)) Registry::Entry &__fir__ = pimsim::registerFunc("fir", fir);

