// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <cmath>

#include "backend/System.h"
/////////////////////////////////////////////////////////////
// FIR filter micro benchmark
/////////////////////////////////////////////////////////////

int32_t fir_multi_tile(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    //int size_input = 256*128*128;
    //int size_filter = 257;

    int size_input = 120*256*512;
    int size_filter = 256;

    PrecisionT::Precision precision_input = PrecisionT::INT16;
    PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;

    int use_tiles = cfg->_ntiles_used;
    int dram_tile = 0; //This specifies the location of the DRAM controller (0 implies tile 0 is connected to DRAM controller)

    //Hardware config: 120 cores, each core has 256 crams, each cram has 256x256 geometry

    //We have 120*256*512 elements in the input
    //Each core will store 256*256 elements first.
    //But we have to waste the last `size_filter` worth of elements in the right most array in each tile.
    //Core 0 will load elements 0 to 256*256-1. It will evaluate answers 0 to 1*256*256-1-256 (where 256 is the filter_size)
    //Core 1 will load elements 1*256*256-256 to 2*256*256-1-256. It will evaluate answers 1*256*256-256 to 2*256*256-1-2*256
    //Core 2 will load elements  2*256*256-2*256 to 3*256*256-1-2*256. It will evaluate answers 2*256*256-2*256 to 3*256*256-1-3*256

    //Core N will load elements  (N)*256*256-(N)*256 to (N+1)*256*256-1-(N)*256. It will evaluate answers (N)*256*256-(N)*256 to (N+1)*256*256-1-(N+1)*256

    //When N=119:
    //Core 119 will load elements  (119)*256*256-(119)*256 to (119+1)*256*256-1-(119)*256. It will evaluate answers (119)*256*256-(119)*256 to (119+1)*256*256-1-(119+1)*256

    //Total elements were 120*256*512. We have loaded upto 120*256*256-1-119*256 = 7833855
    //Elements left to load = 15728640 - 7833855 = 7894785
    //We can do the same around again...
    //Then we will be left with 7894785 - 7833855 = 60930
    //One tile can take 256*256 = 65536 elements
    //So, the leftover 60930 elements can be loaded into one tile.

    int elements_in_1_tile = 256 * 256;
    int elements_loaded_in_all_tiles = cfg->_ntiles_used * cfg->_nblocks * cfg->_ncols - cfg->_ntiles_used * size_filter;
    int num_iterations = size_input / elements_loaded_in_all_tiles;
    int elements_left = size_input % elements_loaded_in_all_tiles;

    //Let's express spatial calculation in cores 0 through 119 first
    for(int iterations=0; iterations<=num_iterations; iterations++) {
    for(int tile=0; tile<use_tiles; tile++) {
        //Now we load inputs from DRAM
        //Currently, we're assuming just one load is enough.
        //Only one input in each column across all tiles.
        request = new Request(Request::Type::RowLoad);
        request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //dst
        request->addOperand(sys->DRAM_ADDR, 0, precision_input); //src
        requests.push_back(*request);

        //Initialize rows that'll hold the accumulator (accumulator size=16)
        request = new Request(Request::Type::RowReset);
        request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //src
        requests.push_back(*request);

        int iter_load_rf_count = (int)ceil(size_filter/(double)cfg->_num_regs_per_rf);

        for(int iter_load_rf=0; iter_load_rf<iter_load_rf_count; iter_load_rf++){
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
        request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //src
        request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); //dst

        requests.push_back(*request);
    }
    }

    //How many more tiles do we need
    int num_tiles_needed = ceil(elements_left / elements_in_1_tile);

    //Now for core 128 and 129, which are basically core 0 and core 1
    for(int tile=0; tile<num_tiles_needed; tile++) {
        //Now we load inputs from DRAM
        //Currently, we're assuming just one load is enough.
        //Only one input in each column across all tiles.
        request = new Request(Request::Type::RowLoad);
        request->addOperand(sys->getAddress(tile,0,0), 0, precision_input); //dst
        request->addOperand(sys->DRAM_ADDR, 0, precision_input); //src
        requests.push_back(*request);

        //Initialize rows that'll hold the accumulator (accumulator size=16)
        request = new Request(Request::Type::RowReset);
        request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //src
        requests.push_back(*request);

        int iter_load_rf_count = (int)ceil(size_filter/(double)cfg->_num_regs_per_rf);

        for(int iter_load_rf=0; iter_load_rf<iter_load_rf_count; iter_load_rf++){
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
        request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), 0, precision_accumulate); //src
        request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT16); //dst

        requests.push_back(*request);
    }


    //Now we handle the leftover portion (255)

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

/////////////////////////////////////////////////////////////
// Simple program to perform an FIR filter
/////////////////////////////////////////////////////////////


static __attribute__((unused)) Registry::Entry &__fir_multi_tile__ = pimsim::registerFunc("fir_multi_tile", fir_multi_tile);

