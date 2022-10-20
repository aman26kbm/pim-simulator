// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <cmath>

#include "backend/System.h"
/////////////////////////////////////////////////////////////
// FIR filter micro benchmark
/////////////////////////////////////////////////////////////

int32_t fir_small_inp_parallel(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    //int size_input = 256*128*128;
    //int size_filter = 257;

    int size_input = 120*256*4;
    int size_filter = 256;

    PrecisionT::Precision precision_input = PrecisionT::INT16;
    PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;

    int use_tiles = cfg->_ntiles_used;

    //Each core will load 256*256 elements
    //The last array's data will be wasted. 

    int elements_in_1_tile = 256*256;

    //Load in two cols in top row

    int col=0; //top left most core
    request = new Request(Request::Type::RowLoad);
    request->addOperand(sys->getAddress(col,0,0), 256 * cfg->_ncols, precision_input); //dst
    request->addOperand(sys->DRAM_ADDR, 256 * cfg->_ncols, precision_input); //src
    requests.push_back(*request);

    //Broadcast from top left core to 51 others
    //First we send to other cores in the top row
    for(int col=1; col<cfg->_meshWidth/2; col++){
        request = new Request(Request::Type::TileSend);
        request->addOperand(sys->getAddress(0,0,0), 256*256, precision_input); //src
        request->addOperand(sys->getAddress(col,0,0), 256*256, precision_input); //dst
        requests.push_back(*request);

        request = new Request(Request::Type::TileReceive);
        request->addOperand(sys->getAddress(0,0,0), 256*256, precision_input); //src
        request->addOperand(sys->getAddress(col,0,0), 256*256, precision_input); //dst
        requests.push_back(*request);
    }

    //Then we send from these downward
    //Don't need to send it to last row
    for(int col=0; col<cfg->_meshWidth/2; col++){
        for(int row=1; row<cfg->_meshHeight-1; row++){
            request = new Request(Request::Type::TileSend);
            request->addOperand(sys->getAddress(col,0,0), 256*256, precision_input); //src
            request->addOperand(sys->getAddress(row * cfg->_meshWidth + col,0,0), 256*32, precision_input); //dst
            requests.push_back(*request);

            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(col,0,0), 256*256, precision_input); //src
            request->addOperand(sys->getAddress(row * cfg->_meshWidth + col,0,0), 256*32, precision_input); //dst
            requests.push_back(*request);        
        }
    }

    //Now load in top right most core
    col=cfg->_meshWidth-1; //top right most core
    request = new Request(Request::Type::RowLoad);
    request->addOperand(sys->getAddress(col,0,0), 256 * cfg->_ncols, precision_input); //dst
    request->addOperand(sys->DRAM_ADDR, 256 * cfg->_ncols, precision_input); //src
    requests.push_back(*request);

    //Broadcast from top right core to 51 others
    //First we send to other cores in the top row
    for(int col=cfg->_meshWidth/2; col<cfg->_meshWidth; col++){
        request = new Request(Request::Type::TileSend);
        request->addOperand(sys->getAddress(cfg->_meshWidth-1,0,0), 256*256, precision_input); //src
        request->addOperand(sys->getAddress(col,0,0), 256*32, precision_input); //dst
        requests.push_back(*request);

        request = new Request(Request::Type::TileReceive);
        request->addOperand(sys->getAddress(cfg->_meshWidth-1,0,0), 256*256, precision_input); //src
        request->addOperand(sys->getAddress(col,0,0), 256*32, precision_input); //dst
        requests.push_back(*request);
    }

    //Then we send from these downward
    //Don't need to send it to last row
    for(int col=cfg->_meshWidth/2; col<cfg->_meshWidth; col++){
        for(int row=1; row<cfg->_meshHeight-1; row++){
            request = new Request(Request::Type::TileSend);
            request->addOperand(sys->getAddress(col,0,0), 256*256, precision_input); //src
            request->addOperand(sys->getAddress(row * cfg->_meshWidth + col,0,0), 256*32, precision_input); //dst
            requests.push_back(*request);

            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(col,0,0), 256*256, precision_input); //src
            request->addOperand(sys->getAddress(row * cfg->_meshWidth + col,0,0), 256*32, precision_input); //dst
            requests.push_back(*request);        
        }
    }

    //Now we have inputs loaded
    //Now let's load RF and broadcast it to all cores
    //Could save here by broadcasting to all except last row

    //Total we have to do 256 steps (mul-add-shift)
    //But we will split that across cores
    int each_core_steps = 5;
    //So we only need like 5 elements in each core's RF
    //But we don't have a way to only load a subset of the RF
    //So we will just load it whole (32 elements)

    for(int tile=0; tile<use_tiles; tile++) {

        //Initialize rows that'll hold the accumulator (accumulator size=16)
        request = new Request(Request::Type::RowReset);
        request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), (256)*cfg->_ncols, precision_accumulate); //src
        requests.push_back(*request);

        //Load filter coefficients into the RF.
        //This step is typically done beforehand/offline.
        //Also, RFs in all tiles will have the same coefficients.
        //So, we can brodcast.
        //When we are broadcasting though, other tiles shouldn't do anything.
        //So they will wait for a semaphore.
        //Assume the number of weights is 8
        if (tile == 0) {
            request = new Request(Request::Type::RowLoad_RF, Request::BroadcastType::ALL);
            request->addOperand(0, 32, precision_input); //dst
            request->addOperand(sys->DRAM_ADDR,32, precision_input); //src
            requests.push_back(*request);   

            request = new Request(Request::Type::Signal, sys->m1);
            request->addOperand(sys->getAddress(0,0,0), 0, PrecisionT::INT4); //src
            requests.push_back(*request);
        } 
        else {
            request = new Request(Request::Type::Wait, sys->m1);
            request->addOperand(sys->getAddress(tile,0,0), 0, PrecisionT::INT4); //src
            requests.push_back(*request);
        }

        //Loop over the following set of instructions N times,
        //where N is the number of filter coefficients
        for (int i=0; i< each_core_steps; i++) {
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

    //Now we reduce

    //left half of the cores 
    


    //right half of the cores


    //Now store the results back into DRAM
    //Only 1 result is present per column.
    //So, just 1 RowStore instruction is enough.
    request = new Request(Request::Type::RowStore);
    request->addOperand(sys->getAddress(0,0,precision_input.bits()+precision_multiply.bits()), 240 * cfg->_ncols, precision_accumulate); //src
    request->addOperand(sys->DRAM_ADDR, 256 * cfg->_ncols, PrecisionT::INT16); //dst

    requests.push_back(*request);



    //Now we handle the leftover portion (256)

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

/////////////////////////////////////////////////////////////
// Simple program to perform an FIR filter
/////////////////////////////////////////////////////////////


static __attribute__((unused)) Registry::Entry &__fir_small_inp_parallel__ = pimsim::registerFunc("fir_small_inp_parallel", fir_small_inp_parallel);

