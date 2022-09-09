#include "backend/Switch.h"
using namespace pimsim;

Switch::Switch(){}

Switch::Switch(int index, int bitwidth){
    this->index = index;
    this->bitwidth = bitwidth;
    this->input_port = 0;
    //this->output_port = 0;
    this->usr_count = 0;
    #ifdef DEBUG_OUTPUT
    printf("create switch %d with bitwidth:", index);
    print(this->bitwidth);
    printf("\n");
    #endif
}