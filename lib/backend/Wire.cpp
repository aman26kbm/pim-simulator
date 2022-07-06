#include "backend/Wire.h"
#include "backend/Tool.h"
using namespace pimsim;

Wire::Wire(){}
void print(std::vector<int> const &input)
{
    for (int i = 0; i < input.size(); i++) {
        std::cout << input.at(i) << ' ';
    }
}
Wire::Wire(int index, int width){
    this->index = index;
    this->parent_side = -1;
    //this->child_side = -1;
    this->bitwidth = width;
    this->available_bitwidth = width;
    this->positive = (index%2);
    this->path = wire_index2path(index);
    #ifdef DEBUG_OUTPUT
    printf("create wire %d with path:", index);
    print(this->path);
    printf("\n");
    #endif
}