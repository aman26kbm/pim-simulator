// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"


/////////////////////////////////////////////////////////////
// Queuing requests for tile0
/////////////////////////////////////////////////////////////

void test_gemv_tile0(System* sys) {

    std::vector<Request> requests;
    Request *request;

    //Load weight matrix values into CRAM
    //This is typically done beforehand/offline.
    //Each core is loading separate data, so no broadcasting here.
    //No need for synchronization either because different
    //cores load data that only they will need/use.
    //4 elements in each column need to be loaded.
    for (int i=0; i<4; i++) {
        request = new Request(Request::Type::RowLoad);
        request->addAddr(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT4); //dst
        request->addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT4); //src
        requests.push_back(*request);
    }

    //Now load vector values into RF.
    //Only 4 elements need to be loaded per core, so just 1 instruction is enough.
    request = new Request(Request::Type::RowLoad_RF);
    request->addAddr(sys->rf_base_addr_tile0, 4, PrecisionT::INT4); //dst
    request->addAddr(sys->DRAM_ADDR, 4, PrecisionT::INT4); //src
    requests.push_back(*request);

    //Initialize rows that'll hold the accumulator (accumulator size=16)
    request = new Request(Request::Type::RowReset);
    request->addAddr(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT16); //src
    request->addAddr(sys->cram_addr_tile0_block0_row8, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    //4 multiplications and additions into the accumulator
    for (int i=0; i<4; i++) {
        //Read an element from the RF
        request = new Request(Request::Type::RowRead_RF);
        request->addAddr(sys->rf_base_addr_tile0, 0, PrecisionT::INT4); //src
        requests.push_back(*request);

        //Multiply in this core/tile.
        request = new Request(Request::Type::RowMul_CRAM_RF);
        request->addAddr(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT4); //src
        request->addAddr(sys->cram_addr_tile0_block0_row8, 0, PrecisionT::INT4); //dst
        requests.push_back(*request);

        //Add the result of multiplication into the accumulator
        request = new Request(Request::Type::RowAdd);
        request->addAddr(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT16); //src
        request->addAddr(sys->cram_addr_tile0_block0_row8, 0, PrecisionT::INT16); //dst
        requests.push_back(*request);
    }

    //Send partial results to tile3
    request = new Request(Request::Type::TileSend);
    request->addAddr(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT16); //src
    request->addAddr(sys->cram_addr_tile3_block0_row8, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

/////////////////////////////////////////////////////////////
// Queuing requests for tile1
/////////////////////////////////////////////////////////////

void test_gemv_tile1(System* sys) {

    std::vector<Request> requests;
    Request *request;

    //Load weight matrix values into CRAM
    //This is typically done beforehand/offline.
    //Each core is loading separate data, so no broadcasting here.
    //No need for synchronization either because different
    //cores load data that only they will need/use.
    //4 elements in each column need to be loaded.
    for (int i=0; i<4; i++) {
        request = new Request(Request::Type::RowLoad);
        request->addAddr(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //dst
        request->addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT4); //src
        requests.push_back(*request);
    }

    //Now load vector values into RF.
    //Only 4 elements need to be loaded per core, so just 1 instruction is enough.
    request = new Request(Request::Type::RowLoad_RF);
    request->addAddr(sys->rf_base_addr_tile1, 4, PrecisionT::INT4); //dst
    request->addAddr(sys->DRAM_ADDR, 4, PrecisionT::INT4); //src
    requests.push_back(*request);

    //Initialize rows that'll hold the accumulator (accumulator size=16)
    request = new Request(Request::Type::RowReset);
    request->addAddr(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT16); //src
    request->addAddr(sys->cram_addr_tile1_block0_row8, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    //4 multiplications and additions into the accumulator
    for (int i=0; i<4; i++) {
        //Read an element from the RF
        request = new Request(Request::Type::RowRead_RF);
        request->addAddr(sys->rf_base_addr_tile1, 0, PrecisionT::INT4); //src
        requests.push_back(*request);

        //Multiply in this core/tile.
        request = new Request(Request::Type::RowMul_CRAM_RF);
        request->addAddr(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //src
        request->addAddr(sys->cram_addr_tile1_block0_row8, 0, PrecisionT::INT4); //dst
        requests.push_back(*request);

        //Add the result of multiplication into the accumulator
        request = new Request(Request::Type::RowAdd);
        request->addAddr(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT16); //src
        request->addAddr(sys->cram_addr_tile1_block0_row8, 0, PrecisionT::INT16); //dst
        requests.push_back(*request);
    }



    //Send partial results to tile3
    request = new Request(Request::Type::TileSend);
    request->addAddr(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT16); //src
    request->addAddr(sys->cram_addr_tile3_block0_row16, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);

}

/////////////////////////////////////////////////////////////
// Queuing requests for tile2
/////////////////////////////////////////////////////////////

void test_gemv_tile2(System* sys) {

    std::vector<Request> requests;
    Request *request;

    //Load weight matrix values into CRAM
    //This is typically done beforehand/offline.
    //Each core is loading separate data, so no broadcasting here.
    //No need for synchronization either because different
    //cores load data that only they will need/use.
    //4 elements in each column need to be loaded.
    for (int i=0; i<4; i++) {
        request = new Request(Request::Type::RowLoad);
        request->addAddr(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT4); //dst
        request->addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT4); //src
        requests.push_back(*request);
    }

    //Now load vector values into RF.
    //Only 4 elements need to be loaded per core, so just 1 instruction is enough.
    request = new Request(Request::Type::RowLoad_RF);
    request->addAddr(sys->rf_base_addr_tile2, 4, PrecisionT::INT4); //dst
    request->addAddr(sys->DRAM_ADDR, 4, PrecisionT::INT4); //src
    requests.push_back(*request);

    //Initialize rows that'll hold the accumulator (accumulator size=16)
    request = new Request(Request::Type::RowReset);
    request->addAddr(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT16); //src
    request->addAddr(sys->cram_addr_tile2_block0_row8, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    //4 multiplications and additions into the accumulator
    for (int i=0; i<4; i++) {
        //Read an element from the RF
        request = new Request(Request::Type::RowRead_RF);
        request->addAddr(sys->rf_base_addr_tile2, 0, PrecisionT::INT4); //src
        requests.push_back(*request);

        //Multiply in this core/tile.
        request = new Request(Request::Type::RowMul_CRAM_RF);
        request->addAddr(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT4); //src
        request->addAddr(sys->cram_addr_tile2_block0_row8, 0, PrecisionT::INT4); //dst
        requests.push_back(*request);

        //Add the result of multiplication into the accumulator
        request = new Request(Request::Type::RowAdd);
        request->addAddr(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT16); //src
        request->addAddr(sys->cram_addr_tile2_block0_row8, 0, PrecisionT::INT16); //dst
        requests.push_back(*request);
    }

    //Send partial results to tile3
    request = new Request(Request::Type::TileSend);
    request->addAddr(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT16); //src
    request->addAddr(sys->cram_addr_tile3_block0_row24, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);

}

/////////////////////////////////////////////////////////////
// Queuing requests for tile3
/////////////////////////////////////////////////////////////

void test_gemv_tile3(System* sys) {

    std::vector<Request> requests;
    Request *request;

    //Load weight matrix values into CRAM
    //This is typically done beforehand/offline.
    //Each core is loading separate data, so no broadcasting here.
    //No need for synchronization either because different
    //cores load data that only they will need/use.
    //4 elements in each column need to be loaded.
    for (int i=0; i<4; i++) {
        request = new Request(Request::Type::RowLoad);
        request->addAddr(sys->cram_addr_tile3_block0_row0, 0, PrecisionT::INT4); //dst
        request->addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT4); //src
        requests.push_back(*request);
    }

    //Now load vector values into RF.
    //Only 4 elements need to be loaded per core, so just 1 instruction is enough.
    request = new Request(Request::Type::RowLoad_RF);
    request->addAddr(sys->rf_base_addr_tile3, 4, PrecisionT::INT4); //dst
    request->addAddr(sys->DRAM_ADDR, 4, PrecisionT::INT4); //src
    requests.push_back(*request);

    //Initialize rows that'll hold the accumulator (accumulator size=16)
    request = new Request(Request::Type::RowReset);
    request->addAddr(sys->cram_addr_tile3_block0_row0, 0, PrecisionT::INT16); //src
    request->addAddr(sys->cram_addr_tile3_block0_row8, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    //4 multiplications and additions into the accumulator
    for (int i=0; i<4; i++) {
        //Read an element from the RF
        request = new Request(Request::Type::RowRead_RF);
        request->addAddr(sys->rf_base_addr_tile3, 0, PrecisionT::INT4); //src
        requests.push_back(*request);

        //Multiply in this core/tile.
        request = new Request(Request::Type::RowMul_CRAM_RF);
        request->addAddr(sys->cram_addr_tile3_block0_row0, 0, PrecisionT::INT4); //src
        request->addAddr(sys->cram_addr_tile3_block0_row8, 0, PrecisionT::INT4); //dst
        requests.push_back(*request);

        //Add the result of multiplication into the accumulator
        request = new Request(Request::Type::RowAdd);
        request->addAddr(sys->cram_addr_tile3_block0_row0, 0, PrecisionT::INT16); //src
        request->addAddr(sys->cram_addr_tile3_block0_row8, 0, PrecisionT::INT16); //dst
        requests.push_back(*request);
    }

    //Receive partial results from tile0
    request = new Request(Request::Type::TileReceive);
    request->addAddr(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT16); //src
    request->addAddr(sys->cram_addr_tile3_block0_row8, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    //Receive partial results from tile1
    request = new Request(Request::Type::TileReceive);
    request->addAddr(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT16); //src
    request->addAddr(sys->cram_addr_tile3_block0_row16, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    //Receive partial results from tile2
    request = new Request(Request::Type::TileReceive);
    request->addAddr(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT16); //src
    request->addAddr(sys->cram_addr_tile3_block0_row24, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    //Now add partial results from the two cores/tiles
    request = new Request(Request::Type::RowAdd);
    request->addAddr(sys->cram_addr_tile3_block0_row0, 0, PrecisionT::INT16); //src
    request->addAddr(sys->cram_addr_tile3_block0_row24, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::RowAdd);
    request->addAddr(sys->cram_addr_tile3_block0_row8, 0, PrecisionT::INT16); //src
    request->addAddr(sys->cram_addr_tile3_block0_row24, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    request = new Request(Request::Type::RowAdd);
    request->addAddr(sys->cram_addr_tile3_block0_row16, 0, PrecisionT::INT16); //src
    request->addAddr(sys->cram_addr_tile3_block0_row24, 0, PrecisionT::INT16); //dst
    requests.push_back(*request);

    //Store results into DRAM
    request = new Request(Request::Type::RowStore);
    request->addAddr(sys->cram_addr_tile3_block0_row24, 0, PrecisionT::INT16); //src
    request->addAddr(sys->DRAM_ADDR, 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);

}

/////////////////////////////////////////////////////////////
// Simple program to perform a matrix-vector mul 
/////////////////////////////////////////////////////////////

int32_t test_gemv(System* sys)
{

    //We have 2 tiles
    //Each has 4 CRAMs.
    //So total columns = 4 * 256 = 1024
    //Let's consider matrix size to be 1024 rows * 8 columns.
    //Let's consider vector size to be 8 rows * 1 column.

    //We will load 4 columns of matrix into tile0
    //and 4 columns of matrix into tile1.
    //That is, 4 elements in one column of each CRAM.

    //The vector is outside in RF.
    //4 elements in 1 core's RF, 4 in second core's RF.

    //We will perform 4 multiply-adds in each tile.
    //Then we will send data from tile0 to tile1.
    //And add them up in tile1.
    //This is the result (1024x1 vector).
    //One element of the result is in each column of tile1's CRAMs.
    //Then store results in DRAM.
    test_gemv_tile0(sys);
    test_gemv_tile1(sys);
    test_gemv_tile2(sys);
    test_gemv_tile3(sys);
    return 0;
}

static __attribute__((unused)) Registry::Entry &__test_gemv__ = pimsim::registerFunc("test_gemv", test_gemv);
