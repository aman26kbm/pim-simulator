#include "backend/Wire.h"
#include <iostream>

using namespace pimsim;
using namespace std;

Wire::Wire(int index, int width){
    this->index = index;
    this->parent_side = -1;
    //this->child_side = -1;
    this->bitwidth = width;
    this->available_bitwidth = width;
    this->positive = !(index%2);
}