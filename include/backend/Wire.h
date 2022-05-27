#ifndef _WIRE_H_
#define _WIRE_H_
#include <vector>
#include <math.h>
#include <assert.h>

namespace pimsim {

class Wire{
public:
    /*if wire's path is 021, positive=false, index=((0+1)*4^2 + (2+1)*4^1 + (1+1)*4^0)*2 = (16 + 12 + 2)*2-1 =60
    if wire's path is 021, positive=true, index=((0+1)*4^2 + (2+1)*4^1 + (1+1)*4^0)*2 = (16 + 12 + 2)*2+1 =61*/
    int index;
    std::vector<int> path;
    /*index of the connected wire which is at a higher or same level*/
    int parent_side;
    //int child_side;
    bool positive;
    /*for a depth = 3 htree, 
    wires of path 0-3 has bitwidth 1024; 
    wires of path 00-33 has bandwidth 512;
    wires of path 000-333 has bitwidth 256*/
    int bitwidth;
    int available_bitwidth;
    Wire();
    Wire(int index, int width);
};

}

#endif