#include "backend/hTree.h"
#include <math.h> 

using namespace pimsim;

hTree::hTree(int depth){
    // for(int i=0; i<n_blocks; i++){
    //     EndPoint* block = new EndPoint(i);
    //     block_list.push_back(block);
    // }
    // hTree_depth = 0;
    // for(int i=n_blocks; i/4>0; i=i/4){
    //     hTree_depth++;
    // }
    int index=0;
    for(int i=0; i<=depth; i++){
        for(int j=0; j<pow(4,i); i++){
            Wire* wire = new Wire(index, 256*pow(2,depth-i));
            wire_list.push_back(wire);
            index++;
        }
    }
}


int get_source_index(Request req){
    int chip_idx; int tile_idx; int block_idx; int row_idx; int col_idx;
    AddrT addr;
    //Source
    int _ncols = 256;
    int _nrows = 256;
    int _nblocks = 4;
    int _ntiles = 2;
    addr = req.addr_list[0];
    int block_index = addr/(_ncols*_nrows);
    //change block index into new index in h_tree
    switch(block_index){
        case 0:return 0;
        case 1:return 1;
        case 2:return 4;
        case 3:return 5;
        case 4:return 2;
        case 5:return 3;
        case 6:return 6;
        case 7:return 7;
        case 8:return 8;
        case 9:return 9;
        case 10:return 12;
        case 11:return 13;
        case 12:return 10;
        case 13:return 11;
        case 14:return 14;
        case 15:return 15;
    }
}

int get_dest_index(Request req){
    int chip_idx; int tile_idx; int block_idx; int row_idx; int col_idx;
    AddrT addr;
    //Source
    int _ncols = 256;
    int _nrows = 256;
    int _nblocks = 4;
    int _ntiles = 2;
    addr = req.addr_list[1];
    int block_index = addr/(_ncols*_nrows);
    //change block index into new index in h_tree
    switch(block_index){
        case 0:return 0;
        case 1:return 1;
        case 2:return 4;
        case 3:return 5;
        case 4:return 2;
        case 5:return 3;
        case 6:return 6;
        case 7:return 7;
        case 8:return 8;
        case 9:return 9;
        case 10:return 12;
        case 11:return 13;
        case 12:return 10;
        case 13:return 11;
        case 14:return 14;
        case 15:return 15;
    }
}

bool hTree::try_configure(Request req){
    int source_index = get_source_index(req);
    int dest_index = get_dest_index(req);
    //block_index*2 is index of the positive wire attached to this block. This wire has the same path as this block.
    std::vector<int> source_path = index2path(source_index*2);
    std::vector<int> dest_path = index2path(dest_index*2);
    int i=0;
    while(source_path[i]==dest_path[i]){
        i++;
    }

    std::vector<int> wire1_path = source_path;
    wire1_path.resize(i+1);
    int wire1_index = path2index(wire1_path, false);
    std::vector<int> wire2_path = dest_path;
    wire2_path.resize(i+1);
    int wire2_index = path2index(wire2_path, true);
    if(!wire_try_connect(wire_list[wire1_index], wire_list[wire2_index])){
        return false;
    }
    if(wire_list[wire1_index]->available_bitwidth<256 || wire_list[wire2_index]->available_bitwidth<256){
        return false;
    }

    while(i+1 < hTree_depth){
        std::vector<int> source_wire1_path = source_path;
        source_wire1_path.resize(i+1);
        int source_wire1_index = path2index(source_wire1_path, false);
        std::vector<int> source_wire2_path = source_path;
        source_wire2_path.resize(i+1+1);
        int source_wire2_index = path2index(source_wire2_path, false);
        if(!wire_try_connect(wire_list[source_wire1_index], wire_list[source_wire2_index])){
            return false;
        }
        if(wire_list[source_wire1_index]->available_bitwidth<256 || wire_list[source_wire2_index]->available_bitwidth<256){
            return false;
        }

        std::vector<int> dest_wire1_path = dest_path;
        dest_wire1_path.resize(i+1);
        int dest_wire1_index = path2index(dest_wire1_path, true);
        std::vector<int> dest_wire2_path = dest_path;
        dest_wire2_path.resize(i+1+1);
        int dest_wire2_index = path2index(dest_wire2_path, true);
        if(!wire_try_connect(wire_list[dest_wire1_index], wire_list[dest_wire2_index])){
            return false;
        }
        if(wire_list[dest_wire1_index]->available_bitwidth<256 || wire_list[dest_wire2_index]->available_bitwidth<256){
            return false;
        }

        i++;
    }
    return true;
}

bool hTree::configure(Request req){
    int source_index = get_source_index(req);
    int dest_index = get_dest_index(req);
    //block_index*2 is index of the positive wire attached to this block. This wire has the same path as this block.
    std::vector<int> source_path = index2path(source_index*2);
    std::vector<int> dest_path = index2path(dest_index*2);
    int i=0;
    while(source_path[i]==dest_path[i]){
        i++;
    }

    std::vector<int> wire1_path = source_path;
    wire1_path.resize(i+1);
    int wire1_index = path2index(wire1_path, false);
    std::vector<int> wire2_path = dest_path;
    wire2_path.resize(i+1);
    int wire2_index = path2index(wire2_path, true);
    if(!wire_connect(wire_list[wire1_index], wire_list[wire2_index])){
        return false;
    }
    if(wire_list[wire1_index]->available_bitwidth<256 || wire_list[wire2_index]->available_bitwidth<256){
        return false;
    }
    wire_list[wire1_index]->available_bitwidth -= 256;
    wire_list[wire2_index]->available_bitwidth -= 256;

    while(i+1 < hTree_depth){
        std::vector<int> source_wire1_path = source_path;
        source_wire1_path.resize(i+1);
        int source_wire1_index = path2index(source_wire1_path, false);
        std::vector<int> source_wire2_path = source_path;
        source_wire2_path.resize(i+1+1);
        int source_wire2_index = path2index(source_wire2_path, false);
        if(!wire_connect(wire_list[source_wire1_index], wire_list[source_wire2_index])){
            return false;
        }
        if(wire_list[source_wire1_index]->available_bitwidth<256 || wire_list[source_wire2_index]->available_bitwidth<256){
            return false;
        }
        wire_list[source_wire2_index]->available_bitwidth -= 256;

        std::vector<int> dest_wire1_path = dest_path;
        dest_wire1_path.resize(i+1);
        int dest_wire1_index = path2index(dest_wire1_path, true);
        std::vector<int> dest_wire2_path = dest_path;
        dest_wire2_path.resize(i+1+1);
        int dest_wire2_index = path2index(dest_wire2_path, true);
        if(!wire_connect(wire_list[dest_wire1_index], wire_list[dest_wire2_index])){
            return false;
        }
        if(wire_list[dest_wire1_index]->available_bitwidth<256 || wire_list[dest_wire2_index]->available_bitwidth<256){
            return false;
        }
        wire_list[dest_wire2_index]->available_bitwidth -= 256;

        i++;
    }
    return true;
}

bool hTree::disconfigure(Request req){
    int source_index = get_source_index(req);
    int dest_index = get_dest_index(req);
    //block_index*2 is index of the positive wire attached to this block. This wire has the same path as this block.
    std::vector<int> source_path = index2path(source_index*2);
    std::vector<int> dest_path = index2path(dest_index*2);
    int i=0;
    while(source_path[i]==dest_path[i]){
        i++;
    }

    std::vector<int> wire1_path = source_path;
    wire1_path.resize(i+1);
    int wire1_index = path2index(wire1_path, false);
    std::vector<int> wire2_path = dest_path;
    wire2_path.resize(i+1);
    int wire2_index = path2index(wire2_path, true);
    //wire1 and wire2 should have >=256 bits used
    assert(wire_list[wire1_index]->bitwidth - wire_list[wire1_index]->available_bitwidth >= 256);
    assert(wire_list[wire2_index]->bitwidth - wire_list[wire2_index]->available_bitwidth >= 256);
    //free 256 bit of wire1 and wire2
    wire_list[wire1_index]->available_bitwidth += 256;
    wire_list[wire2_index]->available_bitwidth += 256;
    // if all bits are freed in wire1 and wire2, disconnect them
    if(wire_list[wire1_index]->available_bitwidth == wire_list[wire1_index]->bitwidth && wire_list[wire2_index]->available_bitwidth == wire_list[wire2_index]->bitwidth){
        assert(!wire_disconnect(wire_list[wire1_index], wire_list[wire2_index]));
    }

    

    while(i+1 < hTree_depth){
        std::vector<int> source_wire1_path = source_path;
        source_wire1_path.resize(i+1);
        int source_wire1_index = path2index(source_wire1_path, false);
        std::vector<int> source_wire2_path = source_path;
        source_wire2_path.resize(i+1+1);
        int source_wire2_index = path2index(source_wire2_path, false);
        Wire* source_wire1 = wire_list[source_wire1_index];
        Wire* source_wire2 = wire_list[source_wire2_index];
        //source_wire2 should have >=256 bits used
        assert(source_wire2->bitwidth - source_wire2->available_bitwidth >= 256);
        //free 256 bits of source_wire2
        source_wire2->available_bitwidth +=256;
        //if source_wire2 has all bits free'd, disconnect it from source_wire1
        if(source_wire2->available_bitwidth == source_wire2->bitwidth){
            assert(wire_disconnect(source_wire1, source_wire2));
        }

        std::vector<int> dest_wire1_path = dest_path;
        dest_wire1_path.resize(i+1);
        int dest_wire1_index = path2index(dest_wire1_path, false);
        std::vector<int> dest_wire2_path = dest_path;
        dest_wire2_path.resize(i+1+1);
        int dest_wire2_index = path2index(dest_wire2_path, false);
        Wire* dest_wire1 = wire_list[dest_wire1_index];
        Wire* dest_wire2 = wire_list[dest_wire2_index];
        //dest_wire2 should have >=256 bits used
        assert(dest_wire2->bitwidth - dest_wire2->available_bitwidth >= 256);
        //free 256 bits of dest_wire2
        dest_wire2->available_bitwidth +=256;
        //if source_wire2 has all bits free'd, disconnect it from source_wire1
        if(dest_wire2->available_bitwidth == dest_wire2->bitwidth){
            assert(wire_disconnect(dest_wire1, dest_wire2));
        }

        i++;
    }
    return true;
}



void hTree::receive_request(Request* req){
    request_list.push_back(req);
}

void hTree::tick(){
    for(int i=0; i<request_list.size(); i++){
        if (!request_list[i]->hTree_ready){
            if(try_configure(*request_list[i])){
                configure(*request_list[i]);
                request_list[i]->hTree_ready = true;
            }
        }
        if(request_list[i]->send_receive_finished){
            disconfigure(*request_list[i]);
            request_list[i]->hTree_ready = false;
            request_list[i]->send_receive_finished = false;
        }
    }
}

