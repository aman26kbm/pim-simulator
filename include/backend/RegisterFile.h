#ifndef _REGISTER_FILE_H_
#define _REGISTER_FILE_H_

#include "Util.h"
#include "Config.h"
#include "Message.h"
#include "Request.h"

#include "System.h"
#include "MemoryBlock.h"
#include "MemoryChip.h"
#include "MemoryComponent.h"

namespace pimsim {

class RegisterFile {
public:

    int num_regs_per_rf;
    int num_bits_per_reg;
    RegisterFile();
    RegisterFile(int num_regs_per_rf, int num_regs_per_reg);

};

}

#endif

