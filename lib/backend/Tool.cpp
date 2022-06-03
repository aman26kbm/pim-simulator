//
// Created by caspar on 11/10/20.
//

#include <cmath>
#include "Tool.h"

int h_tree_distance(int index_A, int index_B, int total_size) {
    int jump = 1;

    int tree_height = h_tree_height(total_size);

    if (index_A == total_size || index_B == total_size)
        return tree_height;

//    std::cout << tree_height << std::endl;

    std::vector<int> index_list_A = get_index_list(index_A, tree_height);
    std::vector<int> index_list_B = get_index_list(index_B, tree_height);


    for (int i = 0; i < tree_height; i++) {
        if (index_list_A[i] != index_list_B[i]) {
            jump = (tree_height - i) * 2;
            break;
        }
    }

    return jump;
}

std::vector<int> h_tree_switches(int index_A, int index_B, int total_size) {
    std::vector<int> switch_list;

    std::vector<int> switch_list_A;
    std::vector<int> switch_list_B;

    //For total_size=64, tree_height is 4.
    //tree_size represents how many levels of hierarchy there are.
    //the lowest level is a group of 4 blocks.
    int tree_height = h_tree_height(total_size);

    //for index_a = 23, index_list is 3,1,1,0
    //for index_b = 12, index_list is 0,3,0,0
    //Basically index list gives us the location coordinates of the block given its index.
    //in each chunk of 4 blocks, there are 4 blocks numbered 0,1,2,3.
    //Say 0 is the top left, 1 is the top right, 2 is the bottom left, 3 is the bottom right.
    //So a block with index list 3,1,1,0 can be easily located.
    std::vector<int> index_list_A = get_index_list(index_A, tree_height);
    std::vector<int> index_list_B = get_index_list(index_B, tree_height);

    if (index_A == total_size || index_B == total_size) {
        if (index_A == total_size) {
            int switch_index = 0;
            for (int i = 0; i < tree_height-1; i++) {
                switch_index += (index_list_B[i]) * pow(4, i-1);
                switch_list.push_back(switch_index);
                switch_index += pow(4, i);
            }
            return switch_list;
        } else {
            int switch_index = 0;
            for (int i = 0; i < tree_height-1; i++) {
                switch_index += (index_list_A[i]) * pow(4, i-1);
                switch_list.push_back(switch_index);
                switch_index += pow(4, i);
            }
            return switch_list;
        }
    }



    int switch_index_A = 0;
    for (int i = 0; i < tree_height-1; i++) {
        switch_index_A += (index_list_A[i]) * pow(4, i-1);
        switch_list_A.push_back(switch_index_A);
        switch_index_A += pow(4, i);
    }

    int switch_index_B = 0;
    for (int i = 0; i < tree_height-1; i++) {
        switch_index_B += (index_list_B[i]) * pow(4, i-1);
        switch_list_B.push_back(switch_index_B);
        switch_index_B += pow(4, i);
    }

//    for (int i = 0; i < switch_list_A.size(); i++)
//        std::cout << switch_list_A[i] << ", ";
//    std::cout << std::endl;
//
//    for (int i = 0; i < switch_list_B.size(); i++)
//        std::cout << switch_list_B[i] << ", ";
//    std::cout << std::endl;

    int size = switch_list_A.size();
    for (int i = size - 1; i >= 0; i--) {
        if (switch_list_A[i] == switch_list_B[i]) {
            switch_list.push_back(switch_list_A[i]);
            break;
        } else {
            switch_list.push_back(switch_list_A[i]);
            switch_list.push_back(switch_list_B[i]);
        }
    }

    return switch_list;
}

int h_tree_height(int total_size) {
    if (total_size == 0)
        return 0;
    int height = 0;
    int current_size = 1;
    while (current_size < total_size) {
        current_size = current_size * 4;
        height++;
    }
    return height;
}

int h_tree_size(int total_size) {
    int height = h_tree_height(total_size);
    int size = 0;
    for (int i = 0; i <= height; i++)
        size += pow(4, i);
    return size;
}

std::vector<int> get_index_list(int index, int height) {
    std::vector<int> index_list;

    for (int i = 0; i < height; i++) {
        index_list.push_back(0);
    }

    for (int i = 1; i <= height; i++) {
        index_list[height - i] = index % 4;
        index /= 4;
    }

    return index_list;
}

Mailbox::Mailbox() {
    _arrived = false;
    _time = 0;
    _max_count = 1;
    _count = 0;
}
Mailbox::Mailbox(int count) {
    _arrived = false;
    _time = 0;
    _max_count = count;
    _count = 0;
}
Mailbox::~Mailbox() {
}
void Mailbox::signal(TimeT t) {
    _arrived = true;
    _time = t;
    _count++;
}
bool Mailbox::status() {
    if (_max_count == 1) {
        return _arrived;
    }
    else {
        return (_arrived && (_count >= _max_count));
    }
}
TimeT Mailbox::getTime() {
    return _time;
}
void Mailbox::reset() {
    _arrived = false;
    _time = 0;
    _count = 0;
}
/*
    wire -1 has index 0 (negative direction) and 1(positive). This is the wire to dram.
    wire 0 has index 2 (negative direction) and index 3 (positive)
    wire 1 has index 4 (negative) and index 5 (positive)
    wire 2 has index 6 (negative) and index 7 (positive)
    wire 3 has index 8 (negative) and index 9 (positive)
    wire 00 has index 10 (negative) and index 11 (positive)
    ...
    wire 33 has index 40 (negative) and index 41 (positive)
    wire 000 has index 42 (negative) and index 43 (positive)
    ...
    wire ijk has index ((i+1)*4^2 + (j+1)*4^1 + (k+1)*4^0)*2 (negative) and ((i+1)*4^2 + (j+1)*4^1 + (k+1)*4^0)*2 + 1 (positive)
    */
std::vector<int> wire_index2path(int index){
    std::vector<int> path;
    std::vector<int> zero_path;
    
    zero_path.push_back(0);
    while(index >= path2index(zero_path, false)){
        zero_path.push_back(0);
    }
    int depth = zero_path.size() - 1;

    index = index/2;
    // for(int i = depth-1; i>0; i--){
    //     int sum_of_pow=0;
    //     for(int j=0; j<=i-1; j++){
    //         sum_of_pow += (int)pow(4, j);
    //     }
    //     path.push_back((index-(int)pow(4, i-1))/(int)pow(4, i)-1);
    //     index = (index-sum_of_pow) % (int)pow(4,i) + sum_of_pow;
    // }

    //convert to 4 base
    for(int i = depth-1; i>=0; i--){
        path.push_back(index/(int)pow(4, i));
        index = index % (int)pow(4,i);
    }
    //make sure all bits are >= 1
    //borrow 1 from previous bit if current bit is 0
    bool all_bits_greater_than_one = true;
    for(int i = 0; i<depth; i++){
        if(path[i]==0){
            all_bits_greater_than_one = false;
        }
    }
    while(!all_bits_greater_than_one){
        for(int i = 0; i<depth-1; i++){
            if(path[i+1]==0){
                path[i]--;
                path[i+1] = 4;
            }
        }
        all_bits_greater_than_one = true;
        for(int i = 0; i<depth; i++){
            if(path[i]==0){
                all_bits_greater_than_one = false;
            }
        }
    }
    for(int i = 0; i<depth; i++){
        path[i]--;
    }
    //path.push_back(index-1);
    return path;
}
std::vector<int> block_index2path(int index, int depth){
    std::vector<int> path;
    for(int i = depth-1; i>=0; i--){
        path.push_back(index/(int)pow(4, i));
        index = index % (int)pow(4,i);
    }
    return path;
}
/*
    wire -1 has index 0 (negative direction) and 1(positive). This is the wire to dram.
    wire 0 has index 2 (negative direction) and index 3 (positive)
    wire 1 has index 4 (negative) and index 5 (positive)
    wire 2 has index 6 (negative) and index 7 (positive)
    wire 3 has index 8 (negative) and index 9 (positive)
    wire 00 has index 10 (negative) and index 11 (positive)
    ...
    wire 33 has index 40 (negative) and index 41 (positive)
    wire 000 has index 42 (negative) and index 43 (positive)
    ...
    wire ijk has index ((i+1)*4^2 + (j+1)*4^1 + (k+1)*4^0)*2 (negative) and ((i+1)*4^2 + (j+1)*4^1 + (k+1)*4^0)*2 + 1 (positive)
    */
int path2index(std::vector<int> path, bool positive){
    int depth = path.size();
    int index = 0;
    for(int i=0; i<depth; i++){
        index += (path[i]+1)*pow(4,depth-1-i);
    }
    if(positive){
        return index*2+1;
    }
    else{
        return index*2;
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
        if(path1.size()!=0){
            for(int i=0; i<path1.size()-1;i++){
                assert(path1[i]==path2[i]);
            }
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
        if(path2.size()!=0){
            for(int i=0; i<path2.size()-1;i++){
                assert(path2[i]==path1[i]);
            }
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
        if(path1.size()!=0){
            for(int i=0; i<path1.size()-1;i++){
                assert(path1[i]==path2[i]);
            }
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
        if(path2.size()!=0){
            for(int i=0; i<path2.size()-1;i++){
                assert(path2[i]==path1[i]);
            }
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
        if(path1.size()!=0){
            for(int i=0; i<path1.size()-1;i++){
                assert(path1[i]==path2[i]);
            }
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
        if(path2.size()!=0){
            for(int i=0; i<path2.size()-1;i++){
                assert(path1[i]==path2[i]);
            }
        }
        // wire2's child side should be connected to wire1's parent side
        assert(wire1->parent_side == wire2->index);
        //disconnect wire1's parent side
        wire1->parent_side = -1;
        return true;
    }
}