#include "backend/hTreeTile.h"
#include <math.h> 

using namespace pimsim;

int hTreeTile::getCycles(Request req, Config* cfg){
    if(req.type == Request::Type::BlockSend_Receive){
        int srcAddr = req.addr_list[0];
        int destAddr = req.addr_list[1];
        int tileIdx = srcAddr/(cfg->_nrows * cfg->_ncols * cfg->_nblocks);
        assert(tileIdx == destAddr/(cfg->_nrows * cfg->_ncols * cfg->_nblocks));
        int srcBlock = (srcAddr%(cfg->_nrows * cfg->_ncols * cfg->_nblocks))/(cfg->_nrows * cfg->_ncols);
        int destBlock = (destAddr%(cfg->_nrows * cfg->_ncols * cfg->_nblocks))/(cfg->_nrows * cfg->_ncols);
        int depth = 0;
        while(srcBlock!=destBlock){
            srcBlock = srcBlock/4;
            destBlock = destBlock/4;
            depth++;
        }
        int distance = depth*2;
        int cycles = distance + req.bits -1;
        return cycles;
    }
    else if(req.type == Request::Type::TileSend || req.type == Request::Type::Signal || req.type == Request::Type::TileReceive || req.type == Request::Type::Wait
            || req.type == Request::Type::RowLoad || req.type == Request::Type::RowStore
            || req.type == Request::Type::RowLoad_RF || req.type == Request::Type::RowStore_RF ){
        //from leaves to root
        int distance = cfg->_htreeTileDepth;
        return distance;
    }
    else if(req.type == Request::Type::RowReduce_WithinTile){
        int cycles=0;
        std::string dtype;
        if(req.precision_list[0].isfloat) dtype="float";
        else dtype = "int";
        PrecisionT::Precision p = req.precision_list[0];//take operand 0 (src)
        PrecisionT::Precision p1 = req.precision_list[1];//take operand 1, should be the max precision of reduction
        int mantissa = p.mantissa;
        int exponent = p.exponent;
        int bits = p.bits();
        int levels = req.size_list[0];
        if (dtype=="float") {
            for (int i=1; i<=levels; i++) {
                int powi2 = pow(i-1,2);
                int cycles_to_add = 2 * mantissa * exponent + 9 * mantissa + 7 * exponent + 12;
                cycles += cycles_to_add; //add
                int distance = (i%2)?(i+1):i;
                cycles += distance + bits - 1; //move bits through distance in htree
            }
        }
        else {
            for (int i=1; i<=levels; i++) {
                int powi2 = pow(i-1,2);
                int cycles_to_add = std::min(mantissa + i, p1.mantissa);
                cycles += cycles_to_add; //add
                int distance = (i%2)?(i+1):i;
                //cycles += distance + bits - 1; //move bits through distance in htree
                cycles += distance; //move bits through distance in htree
            }
        }
        return cycles;
    }
}

