#include "backend/mesh.h"
#include "backend/Config.h"
#include <math.h> 

using namespace pimsim;

mesh::mesh(){}

mesh::mesh(int height, int width, int wordsize_block2block, int _ncols, int _nrows, int num_regs_per_rf, int num_bits_per_reg){
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
    #ifdef DEBUG_OUTPUT
    printf("creating hTree with depth %d, wordsize_block2block %d, ncols %d, nrows %d:\n", depth, wordsize_block2block, _ncols, _nrows);
    #endif
    this->height = height;
    this->width = width;
    this->wordsize_block2block = wordsize_block2block;
    this->_ncols = _ncols;
    this->_nrows = _nrows;
    this->num_regs_per_rf = num_regs_per_rf;
    this->num_bits_per_reg = num_bits_per_reg;
}


int mesh::get_source_index(Request req){
    if(req.type == Request::Type::RowLoad_RF || req.type == Request::Type::RowStore_RF){
        AddrT addr;
        addr = req.addr_list[0];
        int row = addr/this->num_regs_per_rf;
        return row*width;
    }
    else{
        AddrT addr;
        //Source
        int _ncols = this->_ncols;
        int _nrows = this->_nrows;
        addr = req.addr_list[0];
        int block_index = addr/(_ncols*_nrows);
        
        return block_index;
    }
    
}

int mesh::get_dest_index(Request req){
    int chip_idx; int tile_idx; int block_idx; int row_idx; int col_idx;
    AddrT addr;
    //Source
    int _ncols = this->_ncols;
    int _nrows = this->_nrows;
    addr = req.addr_list[1];
    int block_index = addr/(_ncols*_nrows);
    return block_index;
}

void mesh::receive_request(Request* req){
    #ifdef DEBUG_OUTPUT
     printf("mesh receives a request (%s), src_tile %d, dest_tile %d\n",  
                    req->print_name(req->type).c_str(), req->src_tile, req->dst_tile);
    #endif
    //request should be TileSend, TileReceive, BlockSend or BlockReceive
    assert(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive || req->type == Request::Type::BlockSend_Receive 
    || req->type == Request::Type::RowLoad || req->type == Request::Type::RowStore
    || req->type == Request::Type::RowLoad_RF || req->type == Request::Type::RowStore_RF);
    if(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive || req->type == Request::Type::BlockSend_Receive){
        int source_index = get_source_index(*req);
        int dest_index = get_dest_index(*req);

        //for tileSend or tileReceive
        //try to find paired request in current trans_list_list
        //if there is any match, add request to reqPair
        //for blockSend_Receive
        //match = false
        int match = false;
        if(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive){
            for(int i=0; i<trans_list_list.size(); i++){
                if(trans_list_list[i][0].source_index == source_index && trans_list_list[i][0].dest_index == dest_index){
                    match = true;
                    if(req->type == Request::Type::TileSend){
                        assert(reqPair_list[i]->receive_req!=NULL && reqPair_list[i]->send_req == NULL);
                        reqPair_list[i]->send_req = req;
                    }
                    else{
                        assert(reqPair_list[i]->send_req!=NULL && reqPair_list[i]->receive_req == NULL);
                        reqPair_list[i]->receive_req = req;
                    }
                    //there should be only 1 match. 
                    //This is because once a send/receive request is send, the tile will wait until the request is finished and thus removed from reqPair_list and trans_list_list
                    //so break is not necessary
                    break;
                }
            }
        }
        

        //no match found: add entry to reqPair_list and trans_list_list
        if(!match){
            //add entry to reqPair_list
            //if BlockSend_Receive, add to both send and receive_req
            if(req->type == Request::Type::BlockSend_Receive){
                ReqPair* reqPair = new ReqPair{NULL, NULL};
                reqPair->send_req = req;
                reqPair->receive_req = req;
                reqPair_list.push_back(reqPair);
            }
            //if TileSend, add to send_req
            else if(req->type == Request::Type::TileSend){
                ReqPair* reqPair = new ReqPair{NULL, NULL};
                reqPair->send_req = req;
                reqPair_list.push_back(reqPair);
            }
            //if TileReceive, add to receive_req
            else{
                ReqPair* reqPair = new ReqPair{NULL, NULL};
                reqPair->receive_req = req;
                reqPair_list.push_back(reqPair);
            }
            //add entry to trans_list_list
            if(req->type == Request::Type::TileSend || req->type == Request::Type::TileReceive){
                std::vector<Transmission> trans_list;
                for(int i=0; i<width; i++){
                    Transmission trans = {.source_index = source_index+i, .dest_index = dest_index+i};
                    trans_list.push_back(trans);
                }
                trans_list_list.push_back(trans_list);
            }
            //Request is blockSend_Receive
            else{
                std::vector<Transmission> trans_list;
                Transmission trans = {.source_index = source_index, .dest_index = dest_index};
                trans_list.push_back(trans);
                trans_list_list.push_back(trans_list);
            }
        }
    }
    //Request is DRAM rowLoad/Store request
    else if(req->type == Request::Type::RowLoad || req->type == Request::Type::RowStore){
        int block_index = get_source_index(*req);
        ReqPair* reqPair = new ReqPair{NULL, NULL};
        reqPair->send_req = req;
        reqPair->receive_req = req;
        reqPair_list.push_back(reqPair);
        
        std::vector<Transmission> trans_list;
        for(int i=0; i<width; i++){
            Transmission trans;
            if(req->type == Request::Type::RowLoad){
                trans = {.source_index = -1, .dest_index = block_index+i};
            }
            else if(req->type == Request::Type::RowStore){
                trans = {.source_index = block_index+i, .dest_index = -1};
            }
            trans_list.push_back(trans);
        }
        trans_list_list.push_back(trans_list);
    }
    else{
        //RowLoad_RF or RowStore_RF
        int block_index = get_source_index(*req);
        ReqPair* reqPair = new ReqPair{NULL, NULL};
        reqPair->send_req = req;
        reqPair->receive_req = req;
        reqPair_list.push_back(reqPair);

        std::vector<Transmission> trans_list;
        for(int i=0; i<width; i++){

            Transmission trans;
            if(req->type == Request::Type::RowLoad_RF){
                trans = {.source_index = -1, .dest_index = block_index+i};
            }
            else if(req->type == Request::Type::RowStore_RF){
                trans = {.source_index = block_index+i, .dest_index = -1};
            }
            trans_list.push_back(trans);
        }
        trans_list_list.push_back(trans_list);
    }

}

void mesh::tick(){
    for(int i=0; i<reqPair_list.size(); i++){
        //if a reqPair doesn't have both send and receive request, just continue to next reqPair
        if(!reqPair_list[i]->send_req || !reqPair_list[i]->receive_req){
            continue;
        }
        

       //set mesh_ready
        reqPair_list[i]->send_req->mesh_ready = true;
        reqPair_list[i]->receive_req->mesh_ready = true;
        
    }
    //if a reqPair is finished, disconfigure it and remove from reqPair_list and trans_list_list
    int i=0;
    while(i<reqPair_list.size()){
        if(!reqPair_list[i]->send_req || !reqPair_list[i]->receive_req){
            i++;
            continue;
        }
        if(reqPair_list[i]->send_req->send_receive_finished){
            /*
            for(int j=0; j<trans_list_list[i].size(); j++){
                disconfigure(trans_list_list[i][j]);
            }
            */
            #ifdef DEBUG_OUTPUT
            printf("mesh removes a request (%s), src_tile %d, dest_tile %d\n",  
                    reqPair_list[i]->send_req->print_name(reqPair_list[i]->send_req->type).c_str(), reqPair_list[i]->send_req->src_tile, reqPair_list[i]->send_req->dst_tile);
            printf("mesh removes a request (%s), src_tile %d, dest_tile %d\n",  
                    reqPair_list[i]->receive_req->print_name(reqPair_list[i]->receive_req->type).c_str(), reqPair_list[i]->receive_req->src_tile, reqPair_list[i]->receive_req->dst_tile);
            #endif
            reqPair_list.erase(reqPair_list.begin()+i); 
            trans_list_list.erase(trans_list_list.begin()+i);
        }
        else{
            i++;
        }
    }
}

int mesh::get_mesh_time(int source_index, int dest_index, PrecisionT precision){
    int source_row = source_index/width;
    int source_col = source_index%width;
    int dest_row = dest_index/width;
    int dest_col = dest_index%width;
    assert(source_row<height && source_col<width && dest_row<height && dest_col<width);
    int bits = 0;
    switch(precision) {
        case PrecisionT::FP8_e3m4:  //fp8_e3m4
            bits = 8;
            break;
        case PrecisionT::BF16_e8m7:  //bf16_e8m7
            bits = 16;
            break;
        case PrecisionT::FP16_e5m10:  //fp16_e5m10
            bits = 16;
            break;
        case PrecisionT::FP32_e8m23:  //fp32_e8m23
            bits = 32;
            break;
        case PrecisionT::INT4:  //INT4
            bits = 4;
            break;
        case PrecisionT::INT8:  //INT8
            bits = 8;
            break;
        case PrecisionT::INT16:  //INT16
            bits = 16;
            break;
        case PrecisionT::INT32:  //INT32
            bits = 32;
            break;
        default: 
            bits = -1;
            break;
    }
    int distance = abs(dest_row - source_row) + abs(dest_col - source_col);
    return distance + bits-1;
    
}