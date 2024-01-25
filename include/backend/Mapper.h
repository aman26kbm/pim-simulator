#ifndef _MAPPER_H_
#define _MAPPER_H_

//#define _DRAM_DEBUG_OUTPUT_

#include "Config.h"

namespace pimsim{

//compile time mapper(actually works at sending request) 
class Mapper{
private:
    Config* cfg;
    struct VecInfoEntry{
        std::string origVecName;
        int offset;
        //size of each partition for 1 load. 
        //Used to determine shifting. 
        //Assumption: If offset is not divisable by partition_size, then this vector is shifted from its belonging partition.
        int partition_size;
        int bits;
        int length;
        int ben;
        int men;
        int samt;
        int bcnt;
        VecInfoEntry() {}
        VecInfoEntry(const VecInfoEntry &re);
        VecInfoEntry(const std::string &origVecName, int offset, int partition_size, int bits, int length, int ben=0, int men=0, int samt=0, int bcnt=0);
        bool operator<(const VecInfoEntry& other) const;
    };
    struct TileAllocInfoEntry{
        int tile;
        int shift_cnt;
        std::vector<int> occupied_rows;
        TileAllocInfoEntry(){}
        TileAllocInfoEntry(int tile, int shift_cnt);
    };
    struct AllocInfoEntry{
        std::vector<TileAllocInfoEntry> tileAllocInfo;
        AllocInfoEntry();
        std::vector<int> getTiles();
        //add allocation at tile, initialize shift count = 0
        //return false if allocation on tile already exist
        bool addTile(int tile);
        //shift additionally shift_cnt times on tile if allocated
        //return false if not allocated on tile
        bool shiftAtTile(int tile, int shift_cnt);
    };
    


    std::map<VecInfoEntry, AllocInfoEntry> vecMap;

public:
    Mapper(){};
    Mapper(Config* cfg);
    //returns true if the vector is allocated at this tile by this function call
    bool allocVecAtTile(int tile, const std::string &origVecName, int offset, int partition_size, int bits, int length, int ben=0, int men=0, int samt=0, int bcnt=0);
    //is vector allocated at all?
    bool isVecAllocated(const std::string &origVecName, int offset, int partition_size, int bits, int length, int ben=0, int men=0, int samt=0, int bcnt=0);
    //returns the tiles that the vector is allocated at
    std::vector<int> getTileOfVec(const std::string &origVecName, int offset, int partition_size, int bits, int length, int ben=0, int men=0, int samt=0, int bcnt=0);
    bool isVecAllocatedAtTile(int tile, const std::string &origVecName, int offset, int partition_size, int bits, int length, int ben=0, int men=0, int samt=0, int bcnt=0);
    void shiftVec(int shift_cnt, int tile, const std::string &origVecName, int offset, int partition_size, int bits, int length, int ben=0, int men=0, int samt=0, int bcnt=0);
};

}

#endif