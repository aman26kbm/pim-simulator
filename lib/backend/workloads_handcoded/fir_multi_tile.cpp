// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"
/////////////////////////////////////////////////////////////
// FIR filter micro benchmark
/////////////////////////////////////////////////////////////

int32_t fir_multi_tile(System* sys)
{
    std::vector<Request> requests;
    Request *request;
    Config* cfg = sys->_config;

    int size_input = 256*128*128;
    int size_filter = 257;

    PrecisionT::Precision precision_input = PrecisionT::INT16;
    PrecisionT::Precision precision_multiply = PrecisionT::INT32;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT32;

    int use_tiles = cfg->_ntiles_used;
    int dram_tile = 0; //This specifies the location of the DRAM controller (0 implies tile 0 is connected to DRAM controller)

    
    //We have 256*128*128 elements in the input
    //We have a 128*128 mesh. Each array will store 256 elements. So, each core will store 256*128 elements.
    //But we have to waste the last `size_filter` worth of elements in the right most array in each tile.
    //Core 0 will load elements 0 to 256*128-1. It will evaluate answers 0 to 1*256*128-1-257 (where 257 is the filter_size)
    //Core 1 will load elements 1*256*128-257 to 2*256*128-1-257. It will evaluate answers 1*256*128-257 to 2*256*128-1-2*257
    //Core 2 will load elements  2*256*128-2*257 to 3*256*128-1-2*257. It will evaluate answers 2*256*128-2*257 to 3*256*128-1-3*257

    //Core N will load elements  (N)*256*128-(N)*257 to (N+1)*256*128-1-(N)*257. It will evaluate answers (N)*256*128-(N)*257 to (N+1)*256*128-1-(N+1)*257

    //When N=127:
    //Core 127 will load elements  (127)*256*128-(127)*257 to (127+1)*256*128-1-(127)*257. It will evaluate answers (127)*256*128-(127)*257 to (127+1)*256*128-1-(127+1)*257

    //Total elements were 256*128*128. We have loaded upto 128*256*128-1-127*257.
    //Elements left to load = 127*257+1 = 32640
    //Space in one core = 128*256 = 32768
    //So, all the elements we have can fit in one core.

    //But this one core (core "128") will load elements  (128)*256*128-(128)*257 to (128+1)*256*128-1-(128)*257. It will evaluate answers (128)*256*128-(128)*257 to (128+1)*256*128-1-(128+1)*257
    //Now, 32768-32640 = 128
    //This core will load the last 32760 elements (and 128 zeroes, assuming the data is padded in DRAM)
    //It will calculate the answers for 32768-257 = 32511 elements.

    //So, we still have 32640-32511 = 129 answers left. We will have to load last 129 values (and zeroes) in core 129.
    //Core 129 will finish it all.

    //Core 128 and Core 129 are basically core 0 and core 1, but in the second time slot.

    //Let's express spatial calculation in cores 0 through 127 first
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

    //Now for core 128 and 129, which are basically core 0 and core 1
    for(int tile=0; tile<2; tile++) {
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

