#include "backend/MemoryBlock.h"

using namespace pimsim;
using namespace std;

MemoryBlock::MemoryBlock(int n_rows, int n_cols)
    : MemoryComponent(MemoryComponent::Level::Block)
{
    _nrows = n_rows;
    _ncols = n_cols;
    _blocksize = _nrows * _ncols;

    _nchildren = 0;

#ifdef DEBUG_OUTPUT
     printf("Create a block!\n");
#endif
}

bool
MemoryBlock::send2Child(Request& req)
{
    cout << "Error: cannot send request to components lower than bank!\n";
    return false;
}

bool
MemoryBlock::isReady(Request& req)
{
    cout<<"Unused code";
    assert(0);
    //_ctrl refers to the controller of the tile (each block doesn't have a separate controller)
    TimeT cur_time = _ctrl->getTime();
    if (req.isPIM()) {
        TimeT next = getNextGlobalTime();
        if (next <= cur_time) {
            return true;
        } else {
            return false;
        }
    } else {
        cout << "Error: only PIM request can be called at block level!\n";
        return false;
    }
}

void
MemoryBlock::finishReq(Request& req)
{
    MemoryComponent::finishReq(req);
}

void
MemoryBlock::commitReq(Request& req)
{
    switch (req.type)
    {
    case Request::Type::RowRead:
    case Request::Type::ColRead:
    case Request::Type::RowSearch:
    case Request::Type::ColSearch:
        n_reads++;
        break;
    case Request::Type::RowWrite:
    case Request::Type::ColWrite:
    case Request::Type::RowMv:
    case Request::Type::ColMv:
    case Request::Type::RowSet:
    case Request::Type::ColSet:
    case Request::Type::RowReset:
    case Request::Type::ColReset:
    case Request::Type::RowAdd:
    case Request::Type::ColAdd:
    case Request::Type::RowSub:
    case Request::Type::ColSub:
    case Request::Type::RowMul:
    case Request::Type::ColMul:
    case Request::Type::RowDiv:
    case Request::Type::ColDiv:
    case Request::Type::RowBitwise:
    case Request::Type::ColBitwise:
    case Request::Type::RowReduce:
        n_writes++;
        break;
    case Request::Type::BlockSend:
    case Request::Type::BlockReceive:
    case Request::Type::BlockSend_Receive:
    case Request::Type::TileSend:
    case Request::Type::TileReceive:
    case Request::Type::TileSend_Receive:
    case Request::Type::ChipSend_Receive:
        n_unexpected_reqs++;
        break;
    default:
        break;
    }
}

void
MemoryBlock::outputStats(FILE* rstFile)
{
    MemoryComponent::outputStats(rstFile);
    fprintf(rstFile, "Block-level statistics: #Reads(%lu), #Writes(%lu), #UNEXPECT(%lu)\n", 
            n_reads, n_writes, n_unexpected_reqs);
}
