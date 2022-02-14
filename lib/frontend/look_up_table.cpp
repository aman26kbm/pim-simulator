//
// Created by caspar on 11/23/20.
//

#include "look_up_table.h"
#include "math.h"

//LookUpTable::LookUpTable(int nnode_1d, int refinement_level, int dimension)
//{
//    this->nnode_1d = nnode_1d;
//    this->refinement_level = refinement_level;
//    this->dimension = dimension;
//
//    num_refinement_children = pow(2, dimension);
//    num_element_database = pow(2, dimension * refinement_level);
//    nnode_face = nnode_1d * nnode_1d;
//    nnode = nnode_1d * nnode_1d * nnode_1d;
//
//    neighbor_list = new int*[dimension * 2];
//    for (int i = 0; i < dimension * 2; i++) {
//        neighbor_list[i] = new int[num_element_database];
//        for (int j = 0; j < num_element_database; j++) {
//            neighbor_list[i][j] = generate_neighbor_list(i, j);
//        }
//    }
//
//    index_list = new int*[dimension * 2];
//    for (int i = 0; i < dimension * 2; i++) {
//        index_list[i] = new int[nnode_face];
//        for (int j = 0; j < nnode_face; j++) {
//            index_list[i][j] = getNodeIndex(i, j);
//        }
//    }
//}

LookUpTable::LookUpTable(int nchips, int ntiles, int nblocks, int nrows, int ncols, int datatype)
{
    neighbor_list = new int* [DIMENSION * 2];
    index_list = new int* [DIMENSION * 2];
    for (int i = 0; i < DIMENSION * 2; i++) {
        neighbor_list[i] = new int [NUM_ELEMENT_DATABASE];
        index_list[i] = new int [NNODE_FACE];
    }

    _nchips = nchips;
    _ntiles = ntiles;
    _nblocks = nblocks;
    _nrows = nrows;
    _ncols = ncols;

    _datatype = datatype;
    _blocksize = ncols * nrows;
    _tilesize = _blocksize * _nblocks;
    _chipsize = _tilesize * _ntiles;

    for (int i = 0; i < DIMENSION * 2; i++) {
        for (int j = 0; j < NUM_ELEMENT_DATABASE; j++) {
            neighbor_list[i][j] = generate_neighbor_list(i, j);
        }
    }

    for (int i = 0; i < DIMENSION * 2; i++) {
        for (int j = 0; j < NNODE_FACE; j++) {
            index_list[i][j] = getNodeIndex(i, j);
        }
    }

    int neighbor_lookUpTable_size = DIMENSION * 2 * NUM_ELEMENT_DATABASE * _datatype;
    int index_lookUpTable_size = DIMENSION * 2 * NNODE_FACE * _datatype;

    int neighbor_lookUpTable_num_blocks = (neighbor_lookUpTable_size - 1) / _blocksize + 1;
    int index_lookUpTable_num_blocks = (index_lookUpTable_size - 1) / _blocksize + 1;

//    std::cout << "neighbor_lookUpTable_num_blocks: " << neighbor_lookUpTable_num_blocks << std::endl;
//    std::cout << "index_lookUpTable_num_blocks: " << index_lookUpTable_num_blocks << std::endl;
//    std::cout << "_chipsize: " << std::hex << _chipsize << std::endl;

    index_lookUpTable_base_address_list = new AddrT[_ntiles];
    for (int i = 0; i < _ntiles; i++) {
        index_lookUpTable_base_address_list[i] = _tilesize * (i + 1) - index_lookUpTable_num_blocks * _blocksize;
//        std::cout << std::hex << index_lookUpTable_base_address_list[i] << std::endl;
    }

    neighbor_lookUpTable_base_address = _chipsize - neighbor_lookUpTable_num_blocks * _blocksize;
    neighbor_lookUpTable_base_address = index_lookUpTable_base_address_list[_ntiles - 1] - neighbor_lookUpTable_num_blocks * _blocksize;
//    std::cout << "_blocksize: " << std::hex << _blocksize << std::endl;
//    std::cout << "index_lookUpTable_base_address_list: " << std::hex << neighbor_lookUpTable_base_address << std::endl;

}


int LookUpTable::generate_neighbor_list(int FaceID, int ElementDataBaseID)
{
    int primaryFaceID                  = FaceID;
    // This is the primary ElementDataBase Face

    int primaryElementDataBaseID       = ElementDataBaseID;
    // This is the primary ElementDataBase ID

    int axis                           = primaryFaceID / 2;
    // This is the axis where the face is.
    // e.g., X = 0, Y = 1, Z = 2

    int primaryFaceNormal;
    // Also known as a normal vector to the face.
    if(primaryFaceID % 2 == 0)
    {
        // e.g., for face 0, 2, or 4, the direction will be to the negative side of axis.
        primaryFaceNormal = -1;
    }
    else
    {
        // e.g., for face 1, 3, or 5, the direction will be to the positive side of axis.
        primaryFaceNormal = +1;
    }

    int neighborFaceNormal        = (-1)*primaryFaceNormal;
    int neighborFaceID            = primaryFaceID + neighborFaceNormal;
    // This is the neighbor ElementDataBase Face

    int neighborElementDataBaseID = get_neighbors(axis, primaryFaceNormal, primaryElementDataBaseID);
    return neighborElementDataBaseID;
}


int LookUpTable::get_neighbors(int axis, int direction, int ElementDataBaseID)
{
    // e.g., X axis has value of 0
    //       Y axis has value of 1
    //       Z axis has value of 2

    // e.g., direction +1 means that it goes to positive side of axis
    //       direction -1 means that it goes to negative side of axis

    int temp_id         = ElementDataBaseID;
    // This is used for finding which cube the ElementDataBase belong to at a particular
    // level of refinement level.

    int axis_factor     = pow(2, axis);
    // Axis factor is used for multiplication or division purpose.
    // e.g., X axis has factor of 1
    //       Y axis has factor of 2
    //       Z axis has factor of 4

    int neighbor_id     = -1;
    // This is the id of the neighbor cube.
    // initialize with -1, means it has no neighbor for that face

    int child_step      = 1;
    // This is the step or increment of the ElementDataBaseID for each refinement level.
    // For three dimensions, the increment will be as follows.
    // e.g., Refinement Level 0 has child step of 1   (this is the smallest cube)
    //       Refinement Level 1 has child step of 8   (since the Level 1 has 8   Level 0 cubes)
    //       Refinement Level 2 has child step of 64  (since the Level 2 has 64  Level 0 cubes)
    //       Refinement Level 3 has child step of 512 (since the Level 3 has 512 Level 0 cubes)
    // For two dimensions, the increment will be as follows.
    // e.g., Refinement Level 0 has child step of 1   (this is the smallest square)
    //       Refinement Level 1 has child step of 4   (since the Level 1 has 4   Level 0 squares)
    //       Refinement Level 2 has child step of 16  (since the Level 2 has 16  Level 0 squares)
    //       Refinement Level 3 has child step of 64  (since the Level 3 has 64  Level 0 squares)

    int substractor     = 0;
    // This is used to accumulate the child_step * axis_factor for previous refinement levels.
    // It will be used to substract the current child_step * axis_factor for current refinement level.

    int local_child;
    // This indicates where the ElementDataBaseID position in relative to the child in the particular
    // refinement level.
    // For three dimensions, the possible value will be 0, 1, 2, 3, 4, 5, 6, 7.
    // For two dimensions, the possible value will be 0, 1, 2, 3.

    int local_coordinate;
    // This indicates the coordinate of the local child for a particular axis.
    // The value of local coordinate should be between 0 and 1 due to how the refinement works.
    // For three dimensions:
    //   Axis X: child 0, 2, 4, 6 are at coordinate 0
    //           child 1, 3, 5, 7 are at coordinate 1
    //   Axis Y: child 0, 1, 4, 5 are at coordinate 0
    //           child 2, 3, 6, 7 are at coordinate 1
    //   Axis Z: child 0, 1, 2, 3 are at coordinate 0
    //           child 4, 5, 6, 7 are at coordinate 1
    // For two dimensions:
    //   Axis X: child 0, 2 are at coordinate 0
    //           child 1, 3 are at coordinate 1
    //   Axis Y: child 0, 1 are at coordinate 0
    //           child 2, 3 are at coordinate 1

    int neighbor_coordinate;
    // This is the coordinate for the neighbor element database.
    // If the coordinate falls within {0, 1}, then the neighbor can be determined since it is in the same parent.
    // If the coordinate is -1 or +2, then we need to go to the higher refinement level to determine the neighbor ID.
    // since the neighbor is not at the same parent. Maybe, when we go higher, we can find the parent who contains
    // both the ElementDataBase and its neighbor.
    // Basically, neighbor_coordinate is local_coordinate +/- direction.

    // start from level 0 of refinement level and going up to the highest refinement level
    for(int i = 0; i < REFINEMENT_LEVEL; i++)
    {
        // Find which child it is belong to for current level of refinements
        local_child   = temp_id % NUM_REFINEMENT_CHILDREN; // Mod the ID with the number of children (depends on dimension)
        temp_id       = temp_id / NUM_REFINEMENT_CHILDREN; // Div the ID to use for the higher refinement level.

        // Find where the child is located along an axis
        local_coordinate    = (local_child / axis_factor) % 2; // This should be between 0 and 1

        // Find where the neighbor coordinate is, depends on the direction.
        neighbor_coordinate = local_coordinate + direction;

        if((neighbor_coordinate==0 || neighbor_coordinate==1) & neighbor_id==-1)
        {
            // If the neighbor is located in the same parent, we can directly infer its ID
            neighbor_id = ElementDataBaseID + direction * (child_step * axis_factor - substractor);
        }

        // The subsequent loops only useful if the neighbor is not located in the same parent
        // since we need to go to higher refinement level and we may find the (super)-parent who
        // contains the ElementDataBase and its neighbors.

        substractor = substractor + child_step * axis_factor;
        child_step  = child_step * pow(2, DIMENSION);
    }
    return neighbor_id;
}



int LookUpTable::getNodeIndex(int faceID, int nodeID)
{
    int primary_axis = faceID / 2;
    // This is the axis where the face is located at.
    // Basically, if its primary axis is X, then it is a YZ surface.
    // Likewise, if its primary axis is Y, then it is a XZ surface.
    // Likewise, if its primary axis is Z, then it is a XY surface.
    // We use the same coding for axis as follows.
    // X = 0, Y = 1, Z = 2

    int face_position = faceID % 2;
    // This is the face position relative to its primary axis.
    // e.g., South, West, Bottom will have face_position of 0
    //       North, East, Top will have face_position of 1

    int axis_step     = 1;
    // This is the step for node_index that depends on the axis.
    // e.g., X axis will have node_index step of 1
    //       Y axis will have node_index step of NNODE_1D
    //       Z axis will have node_index step of NNODE_1D * NNODE_1D

    int node_index    = 0;
    // This is the node_index that we want to find.

    int temp_nodeID = nodeID;
    // This is the nodeID that we use to find the node_index at particular axis.

    int local_coordinate;
    // This is the coordinate of a node having nodeID at particular axis.

    // Start with lowest dimension
    for (int i = 0; i < DIMENSION; i++)
    {
        if (i!=primary_axis)
        {
            // If the axis is not the primary axis, we can start calculating the node_index
            // since the face is a surface laid on the other axis.
            local_coordinate = temp_nodeID % NNODE_1D;
            temp_nodeID      = temp_nodeID / NNODE_1D;
            node_index       = node_index + local_coordinate * axis_step;
        }
        else
        {
            // If the axis is the primary axis, we can calculate the offset for node_index
            // from the origin since the step is based on the primary axis.
            local_coordinate = local_coordinate; // no change
            temp_nodeID      = temp_nodeID;      // no change
            node_index       = node_index + face_position * (NNODE_1D - 1) * axis_step;
        }

        // increment the axis step
        axis_step = axis_step * NNODE_1D;
    }
    return node_index;
}