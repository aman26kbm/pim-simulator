// tvm target: c -keys=cpu -link-params=0
#define TVM_EXPORTS
#include <cstdint>

#include "backend/System.h"

/////////////////////////////////////////////////////////////////////
///////////////////Synchronization Test//////////////////////////////
///////////////////////////////////////////////////////////////////////

void test_sync_tile0(System *sys)
{
    std::vector<Request> requests;
    Request *request;
    
    //rowLoad_RF dram->tile0 RF
    request = new Request(Request::Type::RowLoad_RF);
    request->addAddr(sys->rf_base_addr_tile0, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    //0. rowLoad dram->tile0 row0
    request = new Request(Request::Type::RowLoad);
    request->addAddr(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    //1. tilesend tile0 row0-> tile1 row8
    request = new Request(Request::Type::TileSend);
    request->addAddr(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT4); //src
    request->addAddr(sys->cram_addr_tile1_block0_row8, 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    //2. tileReceive tile1 row0 -> tile0 row8
    //should execute after the previous request
    request = new Request(Request::Type::TileReceive);
    request->addAddr(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //src
    request->addAddr(sys->cram_addr_tile0_block0_row8, 0, PrecisionT::INT4); //dst
    requests.push_back(*request);
    
    //3. blocksend tile0 block0 row8 -> block2 row8
    //should execute after the previous request
    request = new Request(Request::Type::BlockSend_Receive);
    request->addAddr(sys->cram_addr_tile0_block0_row8, 0, PrecisionT::INT4); //src
    request->addAddr(sys->cram_addr_tile0_block2_row8, 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    //RowAdd INT8
    request = new Request(Request::Type::RowAdd);
    request->addAddr(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT8); //src
    request->addAddr(sys->cram_addr_tile0_block0_row8, 0, PrecisionT::INT8); //dst
    request->addAddr(sys->cram_addr_tile0_block2_row0, 0, PrecisionT::INT8); //src2 not used
    requests.push_back(*request);

    //4. rowStore tile0 row8
    request = new Request(Request::Type::RowStore);
    request->addAddr(sys->cram_addr_tile0_block0_row8, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}

void test_sync_tile1(System *sys)
{
    std::vector<Request> requests;
    Request *request;

    //rowLoad_RF dram->tile1 RF
    request = new Request(Request::Type::RowLoad_RF);
    request->addAddr(sys->rf_base_addr_tile1, 0, PrecisionT::INT4); //src
    requests.push_back(*request);
    
    //0. rowLoad dram -> tile1 row0 
    request = new Request(Request::Type::RowLoad);
    request->addAddr(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    //1. tileReceive tile0 row0-> tile1 row8
    request = new Request(Request::Type::TileReceive);
    request->addAddr(sys->cram_addr_tile0_block0_row0, 0, PrecisionT::INT4); //src
    request->addAddr(sys->cram_addr_tile1_block0_row8, 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    //4. blocksend tile1 block0 row8 -> block2 row8
    //should execute after the previous request
    request = new Request(Request::Type::BlockSend_Receive);
    request->addAddr(sys->cram_addr_tile1_block0_row8, 0, PrecisionT::INT4); //src
    request->addAddr(sys->cram_addr_tile1_block2_row8, 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    //2. tileSend tile1 row0 -> tile0 row8
    request = new Request(Request::Type::TileSend);
    request->addAddr(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //src
    request->addAddr(sys->cram_addr_tile0_block0_row8, 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    //RowAdd INT8
    request = new Request(Request::Type::RowAdd);
    request->addAddr(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT8); //src
    request->addAddr(sys->cram_addr_tile1_block0_row8, 0, PrecisionT::INT8); //dst
    request->addAddr(sys->cram_addr_tile1_block2_row0, 0, PrecisionT::INT8); //src2 not used
    requests.push_back(*request);

    //4. rowStore tile1 row8
    request = new Request(Request::Type::RowStore);
    request->addAddr(sys->cram_addr_tile1_block0_row8, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}


void test_sync_tile2(System *sys)
{
    std::vector<Request> requests;
    Request *request;

    //rowLoad_RF dram->tile2 RF
    request = new Request(Request::Type::RowLoad_RF);
    request->addAddr(sys->rf_base_addr_tile2, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    //0. rowLoad dram->tile2 row0
    request = new Request(Request::Type::RowLoad);
    request->addAddr(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    //1. tilesend tile2 row0-> tile3 row8
    request = new Request(Request::Type::TileSend);
    request->addAddr(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT4); //src
    request->addAddr(sys->cram_addr_tile3_block0_row8, 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    //2. tileReceive tile3 row0 -> tile2 row8
    //should execute after the previous request
    request = new Request(Request::Type::TileReceive);
    request->addAddr(sys->cram_addr_tile3_block0_row0, 0, PrecisionT::INT4); //src
    request->addAddr(sys->cram_addr_tile2_block0_row8, 0, PrecisionT::INT4); //dst
    requests.push_back(*request);
    
    //3. blocksend tile2 block0 row8 -> block2 row8
    //should execute after the previous request
    request = new Request(Request::Type::BlockSend_Receive);
    request->addAddr(sys->cram_addr_tile2_block0_row8, 0, PrecisionT::INT4); //src
    request->addAddr(sys->cram_addr_tile2_block2_row8, 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    //RowAdd INT8
    request = new Request(Request::Type::RowAdd);
    request->addAddr(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT8); //src
    request->addAddr(sys->cram_addr_tile2_block0_row8, 0, PrecisionT::INT8); //dst
    request->addAddr(sys->cram_addr_tile2_block2_row0, 0, PrecisionT::INT8); //src2 not used
    requests.push_back(*request);

    //4. rowStore tile2 row8
    request = new Request(Request::Type::RowStore);
    request->addAddr(sys->cram_addr_tile2_block0_row8, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}


void test_sync_tile3(System *sys)
{
    std::vector<Request> requests;
    Request *request;

    //rowLoad_RF dram->tile3 RF
    request = new Request(Request::Type::RowLoad_RF);
    request->addAddr(sys->rf_base_addr_tile3, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    //0. rowLoad dram -> tile3 row0
    request = new Request(Request::Type::RowLoad);
    request->addAddr(sys->cram_addr_tile3_block0_row0, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    //1. tileReceive tile2 row0-> tile3 row8
    request = new Request(Request::Type::TileReceive);
    request->addAddr(sys->cram_addr_tile2_block0_row0, 0, PrecisionT::INT4); //src
    request->addAddr(sys->cram_addr_tile3_block0_row8, 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    //4. blocksend tile3 block0 row8 -> block2 row8
    //should execute after the previous request
    request = new Request(Request::Type::BlockSend_Receive);
    request->addAddr(sys->cram_addr_tile3_block0_row8, 0, PrecisionT::INT4); //src
    request->addAddr(sys->cram_addr_tile3_block2_row8, 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    //2. tileSend tile3 row0 -> tile2 row8
    request = new Request(Request::Type::TileSend);
    request->addAddr(sys->cram_addr_tile3_block0_row0, 0, PrecisionT::INT4); //src
    request->addAddr(sys->cram_addr_tile2_block0_row8, 0, PrecisionT::INT4); //dst
    requests.push_back(*request);

    //RowAdd INT8
    request = new Request(Request::Type::RowAdd);
    request->addAddr(sys->cram_addr_tile3_block0_row0, 0, PrecisionT::INT8); //src
    request->addAddr(sys->cram_addr_tile3_block0_row8, 0, PrecisionT::INT8); //dst
    request->addAddr(sys->cram_addr_tile3_block2_row0, 0, PrecisionT::INT8); //src2 not used
    requests.push_back(*request);

    //4. rowStore tile3 row8
    request = new Request(Request::Type::RowStore);
    request->addAddr(sys->cram_addr_tile3_block0_row8, 0, PrecisionT::INT4); //src
    requests.push_back(*request);

    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
}


int32_t test_sync(System *sys)
{
    printf("adding tile0 requests:\n");
    test_sync_tile0(sys);
    printf("adding tile1 requests:\n");
    test_sync_tile1(sys);
    printf("adding tile2 requests:\n");
    test_sync_tile2(sys);
    printf("adding tile3 requests:\n");
    test_sync_tile3(sys);
}

static __attribute__((unused)) Registry::Entry &__test_sync__ = pimsim::registerFunc("test_sync", test_sync);