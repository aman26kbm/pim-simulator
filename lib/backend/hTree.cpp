#include "backend/hTree.h"
#include <math.h> 

using namespace pimsim;

hTree::hTree(){}

hTree::hTree(int depth){
    /*
    wire -1 has index 0. This is the wire to dram.
    wire 0 has index 1 (negative direction) and index 2 (positive)
    wire 1 has index 3 (negative) and index 4 (positive)
    wire 2 has index 5 (negative) and index 6 (positive)
    wire 3 has index 7 (negative) and index 8 (positive)
    wire 00 has index 9 (negative) and index 10 (positive)
    ...
    wire 33 has index 39 (negative) and index 40 (positive)
    wire 000 has index 41 (negative) and index 42 (positive)
    ...
    wire ijk has index ((i+1)*4^2 + (j+1)*4^1 + (k+1)*4^0)*2 (positive) and ((i+1)*4^2 + (j+1)*4^1 + (k+1)*4^0)*2 - 1 (negative)
    */
    this->hTree_depth = depth;
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

bool hTree::try_configure(Transmission trans){
    int source_index = trans.source_index;
    int dest_index = trans.dest_index;
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

bool hTree::configure(Transmission trans){
    int source_index = trans.source_index;
    int dest_index = trans.dest_index;
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

bool hTree::disconfigure(Transmission trans){
    int source_index = trans.source_index;
    int dest_index = trans.dest_index;
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
    //request should be TileSend, TileReceive, BlockSend or BlockReceive
    if(!(req->type == Request::Type::TileSend || req->type == Request::Type::BlockSend || req->type == Request::Type::TileReceive || req->type == Request::Type::BlockReceive)){
        assert(true);
    }
    int source_index = get_source_index(*req);
    int dest_index = get_dest_index(*req);
    //try to find paired request in current trans_list_list
    //if there is any match, add request to reqPair
    int match = false;
    for(int i=0; i<trans_list_list.size(); i++){
        if(trans_list_list[i][0].source_index == source_index && trans_list_list[i][0].dest_index == dest_index){
            match = true;
            if(req->type == Request::Type::TileSend || req->type == Request::Type::BlockSend){
                reqPair_list[i]->send_req = req;
            }
            else{
                reqPair_list[i]->receive_req = req;
            }
            //there should be only 1 match. 
            //This is because once a send/receive request is send, the tile will wait until the request is finished and thus removed from reqPair_list and trans_list_list
            //so break is not necessary
            break;
        }
    }

    //no match found: add entry to reqPair_list and trans_list_list
    if(!match){
        //add entry to reqPair_list
        if(req->type == Request::Type::TileSend || req->type == Request::Type::BlockSend){
            ReqPair* reqPair = new ReqPair;
            reqPair->send_req = req;
            reqPair_list.push_back(reqPair);
        }
        else{
            ReqPair* reqPair = new ReqPair;
            reqPair->receive_req = req;
            reqPair_list.push_back(reqPair);
        }
        //add entry to trans_list_list
        if(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive){

            //request is tileSend of tileReceive
            //bias of all block indices:
            //binary = +0, +01, +10, +11, + 100, + 101, +110, +111, +1000 ....
            //bias =   +0, +1,  +4,  +5,  +16 ,  +17,   +20,  +21,  +64
            //i->binary->as quaternary->bias
            //total 2^hTree_depth blocks in a tile
            std::vector<Transmission> trans_list;
            for(int i=0; i<pow(2, hTree_depth); i++){
                int pos = 0;
                int binary = i;
                int bias = 0;
                while(binary){
                    bias+= (int)pow(4,pos)*(binary%2);
                    binary = (int)binary/2;
                    pos++;
                }
                Transmission trans = {.source_index = source_index+bias, .dest_index = dest_index+bias};
                trans_list.push_back(trans);
            }
            trans_list_list.push_back(trans_list);
        }
        //Request is blockSend or blockReceive
        else{
            std::vector<Transmission> trans_list;
            Transmission trans = {.source_index = source_index, .dest_index = dest_index};
            trans_list.push_back(trans);
            trans_list_list.push_back(trans_list);
        }
    }

}

void hTree::tick(){
    for(int i=0; i<reqPair_list.size(); i++){
        //if a reqPair has both send and receive request, and is not hTree_ready, try configure all transactions in it.
        if (!reqPair_list[i]->send_req->hTree_ready && reqPair_list[i]->send_req && reqPair_list[i]->receive_req){
            bool success = true;
            for(int j=0; j<trans_list_list[i].size(); j++){
                if(!try_configure(trans_list_list[i][j])){
                    success = false;
                }
            }
            if(success){
                for(int j=0; j<trans_list_list[i].size(); j++){
                    configure(trans_list_list[i][j]);
                }
                reqPair_list[i]->send_req->hTree_ready = true;
                reqPair_list[i]->receive_req->hTree_ready = true;
            }
        }
        //if a reqPair is finished, disconfigure it and remove from reqPair_list and trans_list_list
        if(reqPair_list[i]->send_req->send_receive_finished){
            for(int j=0; j<trans_list_list[i].size(); j++){
                disconfigure(trans_list_list[i][j]);
            }
            reqPair_list.erase(reqPair_list.begin()+i); 
            trans_list_list.erase(trans_list_list.begin()+i); 
        }
    }
}

