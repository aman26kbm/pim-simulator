#include"backend/Mapper.h"
#include"backend/global.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace pimsim;

Mapper::Mapper(Config* cfg){
    this->cfg = cfg;
}

Mapper::VecInfoEntry::VecInfoEntry(const VecInfoEntry &re){
    origVecName = re.origVecName;
    offset = re.offset;
    partition_size = re.partition_size;
    bits = re.bits;
    length = re.length;
    ben = re.ben;
    men = re.men;
    samt = re.samt;
    bcnt = re.bcnt;
}

Mapper::VecInfoEntry::VecInfoEntry(const std::string &origVecName, int offset, int partition_size, int bits, int length, int ben, int men, int samt, int bcnt){
    this->origVecName = origVecName;
    this->bits = bits;
    this->offset = offset;
    this->partition_size = partition_size;
    this->length = length;
    this->ben = ben;
    this->men = men;
    this->samt = samt;
    this->bcnt = bcnt;
}

bool Mapper::VecInfoEntry::operator<(const VecInfoEntry& other) const{
    if(origVecName.compare(other.origVecName)<0) return true;
    if(origVecName.compare(other.origVecName)>0) return false;
    if(offset < other.offset) return true;
    if(offset > other.offset) return false;
    if(partition_size < other.partition_size) return true;
    if(partition_size > other.partition_size) return false;
    if(bits < other.bits) return true;
    if(bits > other.bits) return false;
    if(length < other.length) return true;
    if(length > other.length) return false;
    if(ben < other.ben) return true;
    if(ben > other.ben) return false;
    if(men < other.ben) return true;
    if(men > other.ben) return false;
    if(samt < other.samt) return true;
    if(samt > other.samt) return false;
    if(bcnt < other.bcnt) return true;
    if(bcnt > other.bcnt) return false;
    return false;
}
Mapper::TileAllocInfoEntry::TileAllocInfoEntry(int tile, int shift_cnt){
    this->tile = tile;
    this->shift_cnt = shift_cnt;
    this->occupied_rows = std::vector<int>{};
}
Mapper::AllocInfoEntry::AllocInfoEntry(){
    this->tileAllocInfo = std::vector<TileAllocInfoEntry>();
}
std::vector<int> Mapper::AllocInfoEntry::getTiles(){
    std::vector<int> tiles;
    for(TileAllocInfoEntry t : this->tileAllocInfo){
        tiles.push_back(t.tile);
    }
    return tiles;
}
bool Mapper::AllocInfoEntry::addTile(int tile){
    for(TileAllocInfoEntry t : this->tileAllocInfo){
        if(t.tile == tile) return false;
    }
    this->tileAllocInfo.push_back(TileAllocInfoEntry(tile,0));
    return true;
}
bool Mapper::AllocInfoEntry::shiftAtTile(int tile, int shift_cnt){
    for(int i=0; i<tileAllocInfo.size(); i++){
        if(tileAllocInfo[i].tile == tile){
            tileAllocInfo[i].shift_cnt+=shift_cnt;
            return true;
        }
    }
    return false;
}


bool Mapper::allocVecAtTile(int tile, const std::string &origVecName, int offset, int partition_size, int bits, int length, int ben, int men, int samt, int bcnt){
    //when allocating, offset should be multiple of  partition_size
    assert(offset%partition_size==0);
    VecInfoEntry vecInfoEntry = VecInfoEntry(origVecName, offset, partition_size, bits, length, ben, men, samt, bcnt);
    if(vecMap.find(vecInfoEntry) == vecMap.end()){
        //create new allocation on tile
        AllocInfoEntry allocInfoEntry = AllocInfoEntry();
        assert(allocInfoEntry.addTile(tile));
        vecMap[vecInfoEntry] = allocInfoEntry;
        return true;
    }
    else{
        //find vecMap[vecInfoEntry] in the map, need to update its allocInfoEntry
        return vecMap[vecInfoEntry].addTile(tile);
    }
    //     allocInfoEntry.tiles.push_back(tile);
    // }
    
    // std::vector<int> allocated_tiles = getTileOfVec(origVecName, offset, bits, length, ben, men, samt, bcnt);
    // for(int t : allocated_tiles){
    //     if(t == tile){
    //         //don't allocate again
    //         return false;
    //     }
    // }
    // //allocate
    
    // allocated_tiles.push_back(tile);
    // AllocInfoEntry allocInfoEntry = AllocInfoEntry(allocated_tiles);
    // vecMap[vecInfoEntry] = allocInfoEntry;
    // return true;
}
bool Mapper::isVecAllocated(const std::string &origVecName, int offset, int partition_size, int bits, int length, int ben, int men, int samt, int bcnt){
    if(getTileOfVec(origVecName, offset, partition_size, bits, length, ben, men, samt, bcnt).empty()){
        return false;
    }
    else{
        return true;
    }
}
std::vector<int> Mapper::getTileOfVec(const std::string &origVecName, int offset, int partition_size, int bits, int length, int ben, int men, int samt, int bcnt){
    VecInfoEntry vecInfoEntry = VecInfoEntry(origVecName, offset, partition_size, bits, length, ben, men, samt, bcnt);
    if(vecMap.find(vecInfoEntry) == vecMap.end()){
        return {};
    }
    else{
        return vecMap[vecInfoEntry].getTiles();
    }
}


bool Mapper::isVecAllocatedAtTile(int tile, const std::string &origVecName, int offset, int partition_size, int bits, int length, int ben, int men, int samt, int bcnt){
    std::vector<int> allocated_tiles = getTileOfVec(origVecName, offset, partition_size, bits, length, ben, men, samt, bcnt);
    for(int t : allocated_tiles){
        if(t == tile){
            return true;
        }
    }
    return false;
}

void Mapper::shiftVec(int shift_cnt, int tile, const std::string &origVecName, int offset, int partition_size, int bits, int length, int ben, int men, int samt, int bcnt){
    VecInfoEntry vecInfoEntry = VecInfoEntry(origVecName, offset, partition_size, bits, length, ben, men, samt, bcnt);
    if(vecMap.find(vecInfoEntry) == vecMap.end()){
        assert(false);
    }
    else{
        //find vecMap[vecInfoEntry] in the map, need to update its allocInfoEntry
        assert(vecMap[vecInfoEntry].shiftAtTile(tile, shift_cnt));
    }
}
