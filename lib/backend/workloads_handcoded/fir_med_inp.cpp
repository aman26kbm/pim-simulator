// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>
#include <cmath>
#include <numeric>

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
    int param = 255; //this is the last multiplier in the input_size
    int tiles_needed = 120; //this is the first multiplier in the input_size

    //Here is a summary of the algo we use
    //Load the RF with the filter weights (32 in number) and then broadcast it to all RFs (i.e. all cores).
    //Then each tile loads its data (256*256 elements) from DRAM.
    //Then we do the {mult, add shift} sequence 32 times, in each tile.
    //Then store results (256*255 elements) from each core to DRAM. 

    PrecisionT::Precision precision_input = PrecisionT::INT16;
    PrecisionT::Precision precision_multiply = PrecisionT::INT16;
    PrecisionT::Precision precision_accumulate = PrecisionT::INT16;

    int use_tiles = cfg->_ntiles_used;

    //Each core will load 256 arrays worth of data.
    //Ideally, we only need to load 32 (filter size) elements in the last CRAM.
    //But here we just load the 256th CRAM completely.
    //The last array's data will be wasted
    //The other 255 will compute 256*255 output elements

    int inputs_in_1_tile =    cfg->_nblocks * cfg->_ncols;
    int results_from_1_tile = (cfg->_nblocks - 1) * cfg->_ncols;

    //Load filters into RF in one core and then broadcast
    request = new Request(Request::Type::RowLoad);
    request->addOperand(sys->getAddress(0,0,0), size_filter, precision_input); 
    request->addOperand(sys->DRAM_ADDR, size_filter, precision_input);
    requests.push_back(*request);

    std::vector<int> v(sys->_config->_meshHeight*sys->_config->_meshWidth);
    std::iota (std::begin(v), std::end(v), 0); // Fill with 0, 1, ...
    sys->broadcast_p2p(sys->getAddress(0,0,0), PrecisionT::INT1, v, cfg->get_num_regs_per_rf()*cfg->get_num_bits_per_reg(), requests);

    //in our case this is ceil(120/108) = 2
    int iter_tile_count = (int)ceil((float)tiles_needed/(float)use_tiles);
    for(int t=0; t<iter_tile_count; t++) {
    for(int tile=0; tile<use_tiles; tile++) {

    //we have 108 tiles, but we have 120 tiles worth of work.
    //the first time, we do want all tiles to participate.
    //but the second/last time, only a few tiles will participate.
    //so, we add this check
    if ( (use_tiles*t + tile) < tiles_needed ) {

        //Now we load inputs from DRAM
        request = new Request(Request::Type::RowLoad);
        request->addOperand(sys->getAddress(tile,0,0), (param+1) * cfg->_ncols, precision_input); //dst
        request->addOperand(sys->DRAM_ADDR, (param+1) * cfg->_ncols, precision_input); //src
        requests.push_back(*request);

        //Initialize rows that'll hold the accumulator (accumulator size=16)
        request = new Request(Request::Type::RowReset);
        request->addOperand(sys->getAddress(tile,0,precision_input.bits()+precision_multiply.bits()), (param+1)*cfg->_ncols, precision_accumulate); //src
        requests.push_back(*request);

        //if filter size was say 64, and the number of registers we have in RF is 32, then we'd
        //need to iterate two times.
        int iter_load_rf_count = (int)ceil((float)size_filter/(float)cfg->_num_regs_per_rf);

        //Note that because we are not loading the RF multiple now, we only support 1 iteration here.
        //That is, we don't support filter size > 32 basically in this code.
        //In the old code, where we loaded the RF multiple times, this restriction wasn't there.
        if (iter_load_rf_count>1) {
            std::cout<<"We don't support filter size > 32"<<std::endl;
            assert(0);
        }

        for(int iter_load_rf=0; iter_load_rf<iter_load_rf_count; iter_load_rf++){
            //Load filter coefficients into the RF.
            //This step is typically done beforehand/offline.
            //Also, RFs in all tiles will have the same coefficients.
            //So, we can brodcast.
            //We did this earlier already.

            ////Reading the filters every time from DRAM
            //request = new Request(Request::Type::RowLoad_RF);
            //request->addOperand(cfg->_num_regs_per_rf * tile,  32, precision_input); //dst
            //request->addOperand(sys->DRAM_ADDR,32, precision_input); //src
            //requests.push_back(*request);   

            //Loop over the following set of instructions N times,
            //where N is the number of filter coefficients
            for (int i=0; i < cfg->_num_regs_per_rf; i++) {
                if(iter_load_rf*cfg->_num_regs_per_rf + i < size_filter){

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
    }
    }


    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}

/////////////////////////////////////////////////////////////
// Simple program to perform an FIR filter
/////////////////////////////////////////////////////////////


static __attribute__((unused)) Registry::Entry &__fir_med_inp__ = pimsim::registerFunc("fir_med_inp", fir_med_inp);

