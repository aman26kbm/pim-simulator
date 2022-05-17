#ifndef _WIRE_H_
#define _WIRE_H_
#include <vector>
#include <math.h>
#include <assert.h>

namespace pimsim {

class Wire{
public:
    /*if wire's path is 021, positive=false, index=((0+1)*4^2 + (2+1)*4^1 + (1+1)*4^0)*2-1 = (16 + 12 + 2)*2-1 =59
    if wire's path is 021, positive=true, index=((0+1)*4^2 + (2+1)*4^1 + (1+1)*4^0)*2 = (16 + 12 + 2)*2-1 =60*/
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
    Wire(int index, int width);
};

std::vector<int> index2path(int index){
    vector<int> path;
    index = (index+1)/2;
    int depth = 0;
    while(index > pow(4,depth)){
        depth++;
    }
    for(int i = depth-1; i>=0; i--){
        path.push_back(index/(int)pow(4, i)-1);
        index = index % (int)pow(4,i);
    }
    return path;
}

int path2index(std::vector<int> path, bool positive){
    int depth = path.size();
    int index = 0;
    for(int i=0; i<depth; i++){
        index += path[i]*pow(4,depth-1-i);
    }
    if(positive){
        return index*2;
    }
    else{
        return index*2-1;
    }
}

bool wire_try_connect(Wire* wire1, Wire* wire2){
    std::vector<int> path1 = wire1->path;
    std::vector<int> path2 = wire2->path;
    if(path1.size()==path2.size()){
        //wire1 and wire 2 should have different direction
        assert(wire1->positive!=wire2->positive);
        //wire1 and wire 2 should only differ at the last entry of path
        for(int i=0; i<path1.size()-1;i++){
            assert(path1[i]==path2[i]);
        }
        assert(path1[path1.size()-1]!=path2[path1.size()-1]);
        //try connect parent sides:
        //if wire1 and wire2 are already connected return true.
        if(wire1->parent_side == wire2->index && wire2->parent_side == wire1->index){
            return true;
        }
        //if wire1 and 2 not already connected, they should both be free at parent sides.
        if(wire1->parent_side!=-1 || wire2->parent_side!=-1){
            return false;
        }
        //wire1 and wire2 are both free at parent sides.
        return true;
    }
    else if(path1.size() < path2.size()){
        //path2 should have path1 as prefix and only has 1 more entry
        assert(path1.size()==path2.size()-1);
        for(int i=0; i<path1.size()-1;i++){
            assert(path1[i]==path2[i]);
        }
        //try connect wire1's child side to wire2's parent side:
        //if wire2's parent side is already connected to wire1, return true
        if(wire2->parent_side == wire1->index){
            return true;
        }
        //if wire2's parent side is not already connected to wire1, wire2 shoud be free at parent side.
        if(wire2->parent_side!=-1){
            return false;
        }
        //wire2 is free at parent side
        return true;
    }
    //path1.size() > path2.size()
    else {
        //path1 should have path2 as prefix and only has 1 more entry
        assert(path2.size()==path1.size()-1);
        for(int i=0; i<path2.size()-1;i++){
            assert(path2[i]==path1[i]);
        }
        //try connect wire2's child side to wire1's parent side:
        //if wire1's parent side is already connected to wire2, return true
        if(wire1->parent_side == wire2->index){
            return true;
        }
        //if wire1's parent side is not already connected to wire2, wire1 shoud be free at parent side.
        if(wire1->parent_side!=-1){
            return false;
        }
        //wire1 is free at parent side
        return true;
    }
}

bool wire_connect(Wire* wire1, Wire* wire2){
    std::vector<int> path1 = wire1->path;
    std::vector<int> path2 = wire2->path;
    if(path1.size()==path2.size()){
        //wire1 and wire 2 should have different direction
        assert(wire1->positive!=wire2->positive);
        //wire1 and wire 2 should only differ at the last entry of path
        for(int i=0; i<path1.size()-1;i++){
            assert(path1[i]==path2[i]);
        }
        assert(path1[path1.size()-1]!=path2[path1.size()-1]);
        //try connect parent sides:
        //if wire1 and wire2 are already connected return true.
        if(wire1->parent_side == wire2->index && wire2->parent_side == wire1->index){
            return true;
        }
        //if wire1 and 2 not already connected, they should both be free at parent sides.
        if(wire1->parent_side!=-1 || wire2->parent_side!=-1){
            return false;
        }
        //wire1 and wire2 are both free at parent sides. Connect them
        wire1->parent_side = wire2->index;
        wire2->parent_side = wire1->index;
        return true;
    }
    else if(path1.size() < path2.size()){
        //path2 should have path1 as prefix and only has 1 more entry
        assert(path1.size()==path2.size()-1);
        for(int i=0; i<path1.size()-1;i++){
            assert(path1[i]==path2[i]);
        }
        //try connect wire1's child side to wire2's parent side:
        //if wire2's parent side is already connected to wire1, return true
        if(wire2->parent_side == wire1->index){
            return true;
        }
        //if wire2's parent side is not already connected to wire1, wire2 shoud be free at parent side.
        if(wire2->parent_side!=-1){
            return false;
        }
        //wire2 is free at parent side. Connect it to wire1.
        wire2->parent_side = wire1->index;
        return true;
    }
    //path1.size() > path2.size()
    else {
        //path1 should have path2 as prefix and only has 1 more entry
        assert(path2.size()==path1.size()-1);
        for(int i=0; i<path2.size()-1;i++){
            assert(path2[i]==path1[i]);
        }
        //try connect wire2's child side to wire1's parent side:
        //if wire1's parent side is already connected to wire2, return true
        if(wire1->parent_side == wire2->index){
            return true;
        }
        //if wire1's parent side is not already connected to wire2, wire1 shoud be free at parent side.
        if(wire1->parent_side!=-1){
            return false;
        }
        //wire1 is free at parent side. Connect it to wire2
        wire1->parent_side = wire2->index;
        return true;
    }
}

bool wire_disconnect(Wire* wire1, Wire* wire2){
    std::vector<int> path1 = wire1->path;
    std::vector<int> path2 = wire2->path;
    if(path1.size()==path2.size()){
        //wire1 and wire 2 should have different direction
        assert(wire1->positive!=wire2->positive);
        //wire1 and wire 2 should only differ at the last entry of path
        for(int i=0; i<path1.size()-1;i++){
            assert(path1[i]==path2[i]);
        }
        assert(path1[path1.size()-1]!=path2[path1.size()-1]);
        //wire1 and wire2 should be connected at parent sides
        assert(wire1->parent_side==wire2->index);
        assert(wire2->parent_side==wire1->index);
        //disconnect
        wire1->parent_side = -1;
        wire2->parent_side = -1;
        return true;
    }
    else if(path1.size() < path2.size()){
        //path2 should have path1 as prefix and only has 1 more entry
        assert(path1.size()==path2.size()-1);
        for(int i=0; i<path1.size()-1;i++){
            assert(path1[i]==path2[i]);
        }
        // wire1's child side should be connected to wire2's parent side
        assert(wire2->parent_side == wire1->index);
        //disconnect wire2's parent side
        wire2->parent_side = -1;
        return true;
    }
    //path1.size() > path2.size()
    else {
        //path1 should have path2 as prefix and only has 1 more entry
        assert(path1.size()-1==path2.size());
        for(int i=0; i<path2.size()-1;i++){
            assert(path1[i]==path2[i]);
        }
        // wire2's child side should be connected to wire1's parent side
        assert(wire1->parent_side == wire2->index);
        //disconnect wire1's parent side
        wire1->parent_side = -1;
        return true;
    }
}



}

#endif