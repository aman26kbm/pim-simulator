#ifndef _REQUEST_H_
#define _REQUEST_H_

#include "Util.h"
#include "Tool.h"
#include <vector>
#include <queue>
#include <list>
#include <string>
#include <cstdio>

namespace pimsim {

class Request {
public:
    enum class Type : int {
        //////////////////////////////////
        //Basic PIM instructions
        //////////////////////////////////
        RowSet,   //Sets (to 1) 1 row in a block. 
                  //"size" argument can be used to specify how many bitlines are involved across the whole tile.
                  //The address of the row is specified by the "addr" argument.
                  //Entire row is set. No way to set a few columns in the chunk of rows.
                  //It'd take the same time if either all columns or a few columns of the chunk of rows was needed to be set. 
                  //So setting a few columns of the chunk of rows is not modelled/supported.
        RowReset, //Resets (to 0) a row in a block.
        RowRead,  //Read 1 row in a block and bring its contents into the row buffer of the block.
                  //The addr argument specifies the row ID. 
                  //"size" argument can be used to specify how many bitlines are involved across the whole tile.
                  //Can't specify a few columns within the row to read. Read all columns in the row.
        RowWrite, //Write a row in a block; rest same as above 

        RowAdd,   //Add multi-bit numbers stored over multiple rows. Bit 0 in row A, bit 1 in row A+1, ...
                  //The "addr" argument specifies the ID of the first row (contains bit 0 of the number)
                  //The time taken is specified to be X cycles in MemoryCharacteristics
                  //where X includes the cycles required to fully add multi-bit numbers. 
                  //The "precision" argument specifies precision.
                  //"size" argument can be used to specify how many bitlines are involved across the whole tile.
                  //There is implicit parallellism here. This instruction involves multiple rows.
                  //Also, the add operation happens across all columns.
                  //example: 
                  //   request = new Request(Request::Type::RowAdd);
                  //     request->addOperand(sys->getAddress(tile,0,src1_row), 0, src1_precision); //src1
                  //     request->addOperand(sys->getAddress(tile,0,src2_row), 0, src2_precision);//src2
                  //     request->addOperand(sys->getAddress(tile,0,dest_row), 0, dest_precision); //dst
                  //     requests.push_back(*request);
        RowSub,   //Same as above, but for Sub instead of Add

        RowCompare, //Compare a multi-bit number stored over multiple rows with another multi-bit number stored over multiple rows.
                    //This request should really have been 2 separate requests - Max and Min, but since they require the same
                    //number of cycles, I've just provided one.
                    //The operands have the same meaning as the RowAdd request.
                    //addr, precision
                    //The time taken by this request is specified in the MemoryCharacteristics.
                    //Happens across all columns in a cram

        RowMul,   //Multiply multi-bit numbers stored over multiple rows. Bit 0 in row A, bit 1 in row A+1, ...
                  //The "addr" argument specifies the ID of the first row (contains bit 0 of the number)
                  //The time taken is specified to be X cycles in MemoryCharacteristics
                  //where X includes the cycles required to fully multiply multi-bit numbers.
                  //The "precision" argument specifies precision.
                  //"size" argument can be used to specify how many bitlines are involved across the whole tile.
                  //There is implicit parallellism here. This instruction involves multiple rows.
                  //Also, the multiply operation happens across all columns.
                  //example:
                  //   request = new Request(Request::Type::RowMul);
                  //     request->addOperand(sys->getAddress(tile,0,src1_row), 0, src1_precision); //src1
                  //     request->addOperand(sys->getAddress(tile,0,src2_row), 0, src2_precision);//src2
                  //     request->addOperand(sys->getAddress(tile,0,dest_row), 0, dest_precision); //dst
                  //     requests.push_back(*request);

        RowBitwise,//Perform a bitwise operation between multi-bit values stored over multiple rows.
                   //The entire row participates. Can't specify a few columns that will participate.
                   //The "addr" argument specifies the row ID.
                   //The "precision" argument specifies precision.
                   //The time taken is specified to be X cycles in MemoryCharacteristics
                   //where X includes the cycles required to fully add multi-bit numbers.
                  //"size" argument can be used to specify how many bitlines are involved across the whole tile.
        ColBitwise,//UNUSED

        RowSearch, //UNUSED
        ColSearch, //UNUSED
        RowReduce,  //Reduce internally within the array. Multi-bit elements present in multiple columns are reduced.
                    //"Precision" argument specifies the precision of the data ([0] will be for src, [1] will be for destination).
                    //The "addr" argument specifies the ID of the first row (contains bit 0 of the number)
                    //The time taken is specified to be X cycles in MemoryCharacteristics
                    //where X includes the cycles required to fully reduce multi-bit numbers.
                    //The "precision" argument specifies precision.
                    //The "size" argument specifies how many levels to stop after.
                    //There is implicit parallellism here. This instruction involves multiple rows.
                    //example:
                  //   request = new Request(Request::Type::RowReduce);
                  //     request->addOperand(sys->getAddress(tile,0,src1_row), size, src1_precision); //src
                  //     request->addOperand(sys->getAddress(tile,0,dest_row), size, dest_precision); //dst
                  //     requests.push_back(*request);
        RowReduce_WithinTile,  //Reduce within tile. Multi-bit elements present in multiple columns are reduced.
                    //"Precision" argument specifies the precision of the data ([0] will be for src, [1] will be for destination).
                    //The "addr" argument specifies the ID of the first row (contains bit 0 of the number)
                    //The time taken is specified to be X cycles in MemoryCharacteristics
                    //where X includes the cycles required to fully reduce multi-bit numbers.
                    //The "precision" argument specifies precision.
                    //The "size" argument specifies how many levels to stop after.
                    //There is implicit parallellism here. This instruction involves multiple rows.
                    //example:
                  //   request = new Request(Request::Type::RowReduce);
                  //     request->addOperand(sys->getAddress(tile,0,src1_row), size, src1_precision); //src
                  //     request->addOperand(sys->getAddress(tile,0,dest_row), size, dest_precision); //dst
                  //     requests.push_back(*request);
        RowShift,   
                    //example:
                  //   request = new Request(Request::Type::RowShift);
                  //     request->addOperand(sys->getAddress(tile,0,src1_row), size, src1_precision); //src
                  //     requests.push_back(*request);
                  //"size" argument can be used to specify how many bitlines are involved across the whole tile.

        //////////////////////////////////
        //Data transfer instructions
        //////////////////////////////////
        BlockSend_Receive, 
                    //one request for both send and receive
                    //Doesn't mean send a whole block from one place to another.
                   //Send N rows from one block to another within a tile. 
                   //Bandwidth of the block-to-block interconnect is specified in memory characteristics (_wordsize).
                   //The unit time taken is 1 cycle in memory characteristics. The total time is found by multiplying this unit time
                   //with number of transfer bursts (in the bus interconnect case) and with number of bursts + number of hops (in the htree interconnect case)
                   //The number of bursts = number of rows * number of bits in a row / interconnect bandwidth
                   //The "addr" argument will specify the row ID of row0.
                   //The "size" argument is unused.
                   //The "precision" argument will specify the number of rows to transfer. 
                   //example:
                    // request = new Request(Request::Type::BlockSend_Receive);
                    // request->addOperand(sys->cram_addr_tile0_block0_row8, 0, PrecisionT::INT4); //src
                    // request->addOperand(sys->cram_addr_tile0_block2_row8, 0, PrecisionT::INT4); //dst
                    // requests.push_back(*request);
        BlockBroadCast,//broadcast a few rows of #size arrays starting from array0 into all other arrays
        TileSend, //Doesn't mean send a whole tile from one place to another.
                  //Doesn't mean send some bits from a block in one tile to another block in another tile.
                  //This only accounts for transfer between tiles (transfer from block to tile and tile to block is not included).
                  //For the H-tree interconnect, this involves some hops. So, there are some extra cycles to be added.
                  //But for the bus interconnect, this involves the delay in the tile-to-tile interconnect (the bus structure
                  //isn't global across the entire chip; there is a hierarchy)
                  //precision and addr arguments have the same meaning as BlockSend_Receive.
                  //"size" argument can be used to specify how many bitlines are involved across the whole tile.
                  //example:
                    // request = new Request(Request::Type::TileSend);
                    // request->addOperand(sys->getAddress(src_tile,0,src_row), 0, PrecisionT::INT16); //src
                    // request->addOperand(sys->getAddress(dest_tile,0,dest_row), 0, PrecisionT::INT16); //dst
                    // requests.push_back(*request);
        TileReceive, //Same as above; just for receive
                   //example:
                    // request = new Request(Request::Type::TileReceive);
                    // request->addOperand(sys->getAddress(src_tile,0,src_row), 0, PrecisionT::INT16); //src
                    // request->addOperand(sys->getAddress(dest_tile,0,dest_row), 0, PrecisionT::INT16); //dst
                    // requests.push_back(*request);
        ChipSend_Receive,//UNUSED
                         //This does not transfer some bits from one block to another in a different chip. 
                         //This only accounts for the transfer from a chip to another chip. 
                         //So, network delay is added.
                         //The transfer time from block to tile, tile to chip boundary is not included.
                         //size, precision and addr arguments have the same meaning as BlockSend.
        TileSend_BroadCast,//broadcast a few rows of data to all other cores
                            //requirement: use signal to unblock all other cores when this request is finished
        TileReceive_BroadCast,//spend 1 cycle to fetch broadcasted data into arrays.
                                //requirement: use wait to block untill TileBroadCast_Send is finished 
        //////////////////////////////////
        //High level API for data transfer - UNUSED
        //////////////////////////////////
        //The following request types form a higher level API. They just wrap the lower level types above.
        //So, there is no "time taken" mentioned for the following in Memorycharacteristics.cpp.
        SystemRow2Row, //Invokes system_sendRow_receiveRow, which if invokes:
                       //RowRead, BlockSend, TileSend, ChipSend_Receive, TileReceive, BlockReceive, RowWrite
                       //If the chips are the same, then it invokes fewer of these.
                       //If the tiles are the same, then it invokes even fewer of these.
                       //In the case of PIMRA, we have only 1 chip. So, we will always use this API
                       //with the same src and dst chip
        SystemRow2Col, //Not supported
        SystemCol2Row, //Not supported
        SystemCol2Col, //Not supported
        SystemLookUpTable, //Not supported

        SystemRowStore, //Stores rows from a CRAM block into DRAM. This is a higher level API.
                        //So, this includes the steps to read rows from a block, then send it via the 
                        //block to block interconnect, then via the tile level interconnect, into the DRAM.
        SystemColRead,  //Not supported
        SystemRowLoad,  //Loads rows from DRAM into a CRAM block. This is basically the same as
                        //SystemRowStore, just in the opposite direction.
        SystemColWrite, //Not supported

        //////////////////////////////////
        //DRAM loads and stores 
        //////////////////////////////////
        RowLoad,    //Load multiple rows from DRAM into CRAM (all CRAMs in a core are loaded with one instruction).
                    //The "size" argument specifies the number of columns to load. For example, if size=number of columns in a CRAM,
                    //then only that many columns are loaded. That is, only one CRAM will be loaded.
                    //The "precision" argument tells the number of rows to load.
                    //The "addr" argument will specify the row ID of row0.
                    //Example:
                    // size = 256;
                    // request = new Request(Request::Type::RowLoad);
                    // request->addOperand(sys->getAddress(tile,0,row), size, PrecisionT::INT4); //cram addr
                    // request->addOperand(sys->DRAM_ADDR, size, PrecisionT::INT4); //dram addr
                    // requests.push_back(*request);
                    //We have more arguments related to data shuffling here:
                    //1. broadcast_en
                    //2. multicast_en
                    //3. shift_amount
                    //4. bit_count
                    //If broadcast_en or multicast_en is set to 1, then the "size" has to be number of columns in a CRAM.
                    //For details, see: https://docs.google.com/document/d/1Bf0mXqqMR2pZI0vSRqwMha4yL-xd5H8vjs1tdAiPKfM/edit?usp=sharing
                    //Example with multicast:
                    // size = 256; //this needs to match num_cols in CRAMs
                    // broadcast_en = 0;
                    // multicast_en = 1;
                    // shift_amount = 2;
                    // bit_count = 2;
                    // request = new Request(Request::Type::RowLoad);
                    // request->addOperand(sys->getAddress(tile,0,row), size, PrecisionT::INT4); //cram addr
                    // request->addOperand(sys->DRAM_ADDR, size, PrecisionT::INT4); //dram addr
                    // request->setShuffle(broadcast_en, multicast_en, shift_amount, bit_count);
                    // requests.push_back(*request);
        RowStore,   //Store multiple rows from a CRAM block to DRAM
                    //Rest of the details are the same as RowLoad
                    //example:
                    // request = new Request(Request::Type::RowStore);
                    // request->addOperand(sys->getAddress(tile,0,row), 0, PrecisionT::INT4); //cram addr
                    // request->addOperand(sys->DRAM_ADDR, 0, PrecisionT::INT4); //dram addr
                    // requests.push_back(*request);
        
        //////////////////////////////////
        //Synchronization related instructions
        //////////////////////////////////
        Signal,
        //example:
                    // request = new Request(Request::Type::Signal);
                    // request->addOperand(sys->getAddress(src_tile,0,src_row), 0, PrecisionT::INT16); //src
                    // request->addOperand(sys->getAddress(dest_tile,0,dest_row), 0, PrecisionT::INT16); //dst
                    // requests.push_back(*request);
        Wait,
        //example:
                    // request = new Request(Request::Type::Wait);
                    // request->addOperand(sys->getAddress(src_tile,0,src_row), 0, PrecisionT::INT16); //src
                    // request->addOperand(sys->getAddress(dest_tile,0,dest_row), 0, PrecisionT::INT16); //dst
                    // requests.push_back(*request);
        Barrier,
        ResetSync,
        NOP,
        
        //////////////////////////////////
        //RF related instructions
        //////////////////////////////////
        RowLoad_RF,     //Load a set of data elements from DRAM into RF.
                        //The "precision" argument tells the precision of each element loaded.
                        //The "size" argument tells how many such elements will be loaded.
                        //Number of words loaded from DRAM depends on the total bytes requested
                        //and the configured rf_chunk_size.
                        //The "addr" argument specifies the location in the RF where the first
                        //element will be saved.
                        //example:
                        // request = new Request(Request::Type::RowLoad_RF);
                        // request->addOperand(sys->_num_regs_per_rf * tile, 4, PrecisionT::INT4); //RF addr
                        // request->addOperand(sys->DRAM_ADDR, 4, PrecisionT::INT4); //dram addr
                        // requests.push_back(*request);
        RowStore_RF,    //Store a set of data elements from RF into DRAM
                        //Rest of the details are the same as RowLoad_RF
                        //example:
                        // request = new Request(Request::Type::RowStore_RF);
                        // request->addOperand(sys->_num_regs_per_rf * tile, 4, PrecisionT::INT4); //RF addr
                        // request->addOperand(sys->DRAM_ADDR, 4, PrecisionT::INT4); //dram addr
                        // requests.push_back(*request);
        RowMul_CRAM_RF, //Multiplication between a value in CRAM and RF.
                        //The "addr" argument specifies the ID of the first row (contains bit 0 of the number)
                        //One operand comes from the RF (common for all columns). It's addr is not specified.
                        //The time taken is specified to be X cycles in MemoryCharacteristics
                        //where X includes the cycles required to fully multiply multi-bit numbers.
                        //The "precision" argument specifies precision.
                        //"size" argument can be used to specify how many bitlines are involved across the whole tile.
                        //There is implicit parallellism here. This instruction involves multiple rows.
                        //Also, the multiply operation happens across all columns.
        RowAdd_CRAM_RF, //Addition between a value in CRAM and RF
                        //Same as RowMul_CRAM_RF, but for addition
                        //example:
                        // request = new Request(Request::Type::RowDotProduct_CRAM_RF);
                        // request->addOperand(sys->getAddress(tile,0,src_row), 0, PrecisionT::INT4); //src
                        // request->addOperand(sys->_num_regs_per_rf * tile, 4, PrecisionT::INT4);//rf
                        // request->addOperand(sys->getAddress(tile,0,dest_row), 0, PrecisionT::INT4); //dst
                        // requests.push_back(*request);

        RowDotProduct_CRAM_RF, //Dot product between two values in CRAM (a,b) and two values in RF (x,y)
                               //Answer = ax + by, where x and y scalars (in RF), but a and b are vectors in CRAM
                               // request = new Request(Request::Type::RowDotProduct_CRAM_RF);
                               // request->addOperand(sys->getAddress(tile,0,src1_row), 0, PrecisionT::INT4); //src a
                               // request->addOperand(sys->getAddress(tile,0,src2_row), 0, PrecisionT::INT4); //src b
                               // request->addOperand(sys->_num_regs_per_rf * tile, 0, PrecisionT::INT4);//rf x 
                               // request->addOperand(sys->_num_regs_per_rf * tile, 0, PrecisionT::INT4);//rf y
                               // request->addOperand(sys->getAddress(tile,0,dest_row), 0, PrecisionT::INT4); //dst
                               // requests.push_back(*request);

        RowRead_RF,     //Read a row from the RF into a register (flops)
                        //The addr argument specifies the row ID. 
                        //"size" argument is not used.
                        //"precision" argument specifies the precision of the value to read.
                        //example:
                        // request = new Request(Request::Type::RowRead_RF);
                        // request->addOperand(sys->_num_regs_per_rf * tile, 0, PrecisionT::INT4); //src
                        // requests.push_back(*request);
        RowWrite_RF,    //Write a row from a register (flops) into RF
        RowAdd_RF,      //Add two values sourced from the RF.
                        //This is not used for computations, but used for control (e.g looping over other instructions, etc.)
                        //"precision" argument specifies the precision of the operands.
                        //"size" argument is not used
                        //"addr" argument specifies the location of the operand in the RF
        RowSub_RF,      //Same as RowAdd_RF, but for subtraction
        PopCountReduce_RF,//popcount reduce several rows. Result stored in RF.
                        //example:
                        // request = new Request(Request::Type::PopCountReduce_RF);
                        // request->addOperand(sys->cram_addr_tile1_block0_row0, 0, PrecisionT::INT4); //src
                        // request->addOperand(sys->rf_base_addr_tile1, 0, PrecisionT::INT4); //dst
                        // requests.push_back(*request);
        MAX
    } type;


    static std::string print_name(Type type);
    static std::string print_category(Type type);
    int reqNo;
    
    TimeT arrive_time = 0; //The time at which the request arrived in the request queue
    TimeT start_time = 0; //The time at which the request is decoded and start waiting
    TimeT process_time = 0; //The time at which the request started to be processed (ie. actually executed by the tile)
    TimeT finish_time = 0; //The time at which the request finished processing
    std::vector<AddrT> addr_list; //address list
    std::vector<int> size_list; //size list
    std::vector<PrecisionT::Precision> precision_list;
    int bits=0;
    int chip, tile, block, row, col=0;
    Mailbox* mail;
    //indicates if the htree is configured for this request. Value owned by htree.
    bool hTree_ready=false;
    bool mesh_ready=false;
    int dynaMeshHops = 0;  //num of hops in the mesh network
    int packets2Mesh = 0;  //number of flits
    bool requesting_load = false;
    bool requesting_store = false;
    //indicates if a tileSend/blockSend is finished. hTree will check this value at each cycle. When it is true, hTree will disconfigure for this request.
    int mesh_transfer_time=0;//only used for mesh. Indicates transfer time of a request
    int DynaMesh_transfer_time=0; //Not populated in the simulator
    bool send_receive_finished=false;
    bool enableTransposeUnit = true; //this bit can be used to control whether we want the transpose unit in the DRAM controller to be enabled or not.
                                     //it is enabled by default, but can be disabled when data doesn't need to be transposed on the way. For example,
                                     //when you're sending contents that spilled from CRAMs or RF to the DRAM, or when you are loading weights from the DRAM.
                                     //this is only relevant for dram related instructions. also, this is enabled by default.

    //Members related to data shuffling. These are applicable when loading data from DRAM
    bool broadcast_en = 0;
    bool multicast_en = 0;
    int shift_amount = 0;
    int bit_count = 0;

    bool dram_ready=false;

    enum class BroadcastType: int {
        NONE,
        ALL
    } broadcast;

    Request();
    Request(Type t);
    Request(Type t, AddrT addr);
    Request(Type t, Mailbox* m);
    Request(Type t, BroadcastType b);
    Request(const Request& req);

    std::string reqToStr();

    void addOperand(AddrT addr, int size, PrecisionT::Precision precision=PrecisionT::INT8);
    void swapSrcDst();
    void setLocation(int chip, int tile, int block, int row, int col);
    void disableTranspose(); //disable transpose unit for this request
    void enableTranspose(); //enable transpose unit for this request
    //a request is load or store
    bool isChipDram();
    //a request is transferation between tiles
    bool isChip();
    //a request is transferation between blocks within a tile
    bool isTile();
    //a request is only a PIM operation on its own
    bool isPIM();
    //a request is an explicit synchronization request
    bool isSync();
    //set shuffle related parameters
    void setShuffle(bool ben, bool men, int samt, int bcnt);

};

struct ReqQueue {
    std::queue<Request> *q = new std::queue<Request>();
    unsigned int max = 64;
    unsigned int size();
    void push_back(Request req);
    Request pop_front();
    void clear();
    bool is_empty();
};

}

#endif
