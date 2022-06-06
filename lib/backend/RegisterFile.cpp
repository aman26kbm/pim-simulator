#include "backend/RegisterFile.h"

using namespace std;
using namespace pimsim;

RegisterFile::RegisterFile(){}

RegisterFile::RegisterFile(int num_regs_per_rf, int num_bits_per_reg){
    this->num_regs_per_rf = num_regs_per_rf;
    this->num_bits_per_reg = num_bits_per_reg;
}
