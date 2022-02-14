//
// Created by caspar on 11/23/20.
//

#ifndef PIM_SIM_LOOK_UP_TABLE_H
#define PIM_SIM_LOOK_UP_TABLE_H

#include <vector>
#include <iostream>
#include "global.h"
#include "util.h"

using namespace pimsim;

class LookUpTable {
    public:
//        int nnode_1d;
//        int refinement_level;
//        int dimension;
//
//        int num_refinement_children;
//        int num_element_database;
//        int nnode_face;
//        int nnode;

        int _nchips, _ntiles, _nblocks, _nrows, _ncols, _datatype;
        AddrT _chipsize, _tilesize, _blocksize;

        int **neighbor_list;
        int **index_list;

        AddrT neighbor_lookUpTable_base_address;
        AddrT *index_lookUpTable_base_address_list;

        LookUpTable(int nchips, int ntiles, int nblocks, int nrows, int ncols, int datatype);

    private:
        int generate_neighbor_list(int i, int j);
        int get_neighbors(int axis, int direction, int ElementDataBaseID);
        int getNodeIndex(int faceID, int nodeID);
};



#endif //PIM_SIM_LOOK_UP_TABLE_H
