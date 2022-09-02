#ifndef _SWITCH_H_
#define _SWITCH_H_

#include <assert.h>

namespace pimsim {

class Switch{
public:
    int index;
    int input_port;//can take values 0,1,2,3,4; 0 for local array, 1 for top, 2 for right, 3 for bottom, 4 for left.
    //int output_port;//can take values 0,1,2,3,4; 0 for local array, 1 for top, 2 for right, 3 for bottom, 4 for left.
    //initially 0, add 1 if a transfer use this switch. -1 when a transfer that uses this switch is finished.
    //controller look at this value to decide if switch is free to be configured.
    int usr_count;
    int bitwidth;
    Switch();
    Switch(int index, int bitwidth);
};

}

#endif