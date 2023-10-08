#include "Memory_characteristics.h"
#include "backend/Config.h"
#include "backend/global.h"
#include <iostream>
#include <cmath>

using namespace pimsim;
using namespace std;

//Helper function to find the number of clocks for a given request's precision and op type
int MemoryCharacteristics::getClocksForReq(std::vector<pimsim::PrecisionT::Precision> precision_list, string op, int levels) {
    int clocks;
    int bits;
    string dtype;
    int mantissa;
    int exponent;

    if (op=="add") {
        if(precision_list[0].isfloat) dtype="float";
        else dtype = "int";
        PrecisionT::Precision p = precision_list[0].bits()>precision_list[1].bits()? precision_list[0] : precision_list[1];//take bigger src
        p = p.bits()>precision_list[2].bits()?precision_list[2] : p;//take smaller of p, dest
        mantissa = p.mantissa;
        exponent = p.exponent;
        bits = p.bits();
        if (dtype=="float") {
            clocks = 2 * mantissa * exponent + 9 * mantissa + 7 * exponent + 12;
        }
        else {
            clocks = mantissa+1; 
        }
    }
    else if (op=="add_cram_rf") {
        if(precision_list[0].isfloat) dtype="float";
        else dtype = "int";
        PrecisionT::Precision p = precision_list[0].bits()>precision_list[1].bits()? precision_list[0] : precision_list[1];//take bigger src
        p = p.bits()>precision_list[2].bits()?precision_list[2] : p;//take smaller of p, dest 
        mantissa = p.mantissa;
        exponent = p.exponent;
        bits = p.bits();
        if (dtype=="float") {
            clocks = 2 * mantissa * exponent + 9 * mantissa + 7 * exponent + 12;
        }
        else {
            clocks = mantissa+1; 
        }
    }
    else if (op=="compare") {
        if(precision_list[0].isfloat) dtype="float";
        else dtype = "int";
        PrecisionT::Precision p = precision_list[0].bits()>precision_list[1].bits()? precision_list[0] : precision_list[1];//take bigger src
        p = p.bits()>precision_list[2].bits()?precision_list[2] : p;//take smaller of p, dest
        mantissa = p.mantissa;
        exponent = p.exponent;
        bits = p.bits();
        if (dtype=="float") {
            clocks = 0;
            //Not supported currently
            cout<<"Need to update code for cycles to compare 2 float numbers"<<std::endl;
            assert(0);
        }
        else {
            //two numbers are present in a column
            //3 steps involved:
            //1: copy second number to the result rows 
            //2: invert the second number
            //3: add the two numbers
            //4: using msb of the result as mask bit, overwrite the result rows
            
            //       copy         invert        add      mask overwrite
            clocks = mantissa + mantissa +  mantissa+1 + mantissa;
        }
    }
    else if (op=="mul") {
        if(precision_list[0].isfloat) dtype="float";
        else dtype = "int";
        PrecisionT::Precision p = precision_list[0].bits()>precision_list[1].bits()? precision_list[0] : precision_list[1];//take bigger of src
        p = p.bits()>precision_list[2].bits()?precision_list[2] : p;//take smaller of p and dest
        mantissa = p.mantissa;
        exponent = p.exponent;
        bits = p.bits();
        if (dtype=="float") {
            clocks = mantissa * mantissa + 6 * mantissa + 2 * exponent + 10;
        }
        else {
            clocks = mantissa * mantissa + 3 * mantissa - 2;
        }
    }
    else if (op=="mul_cram_rf") {
        if(precision_list[0].isfloat) dtype="float";
        else dtype = "int";
        PrecisionT::Precision p = precision_list[0].bits()>precision_list[1].bits()? precision_list[0] : precision_list[1];//take bigger of src
        p = p.bits()>precision_list[2].bits()?precision_list[2] : p;//take smaller of p and dest
        mantissa = p.mantissa;
        exponent = p.exponent;
        bits = p.bits();
        if (dtype=="float") {
            clocks = (mantissa * mantissa + 6 * mantissa + 4 * exponent + 13) / 2;
        }
        else {
            clocks = (mantissa * mantissa + 3 * mantissa - 2) / 2;
        }
    }
    else if (op=="dp_cram_rf") {
        if(precision_list[0].isfloat) dtype="float";
        else dtype = "int";

        // we're doing ax+by
        //a and b are inside the cram
        //x and y are outside the cram (in the rf)
        //in the arguments, operands 0 and 1 are a and b, and operands 2 and 3 are x and y

        if (dtype=="float") {
            PrecisionT::Precision p1 = precision_list[0].bits()>precision_list[1].bits()? precision_list[0] : precision_list[1];//take bigger of src in cram
            PrecisionT::Precision p2 = precision_list[2].bits()>precision_list[3].bits()? precision_list[2] : precision_list[3];//take bigger of src in rf
            PrecisionT::Precision p3 = p1.bits()>p2.bits()? p1 : p2;
            mantissa = p3.mantissa;
            exponent = p3.exponent;

            //This is not the optimal algo. Just the naive one.
            //Cycles for calculating ax
            clocks = (mantissa * mantissa + 6 * mantissa + 4 * exponent + 13) / 2;
            //Cycles for calculating by
            //Cycles for adding ax and by
        }
        else {
            PrecisionT::Precision p1 = precision_list[0].bits()>precision_list[1].bits()? precision_list[0] : precision_list[1];//take bigger of src in cram
            PrecisionT::Precision p2 = precision_list[2].bits()>precision_list[3].bits()? precision_list[2] : precision_list[3];//take bigger of src in rf
            //p = p.bits()>precision_list[2].bits()?precision_list[2] : p;//take smaller of p and dest
        
            int c_mantissa;
            int c_exponent;
            int r_mantissa;
            int r_exponent;
            c_mantissa = p1.mantissa;
            c_exponent = p1.exponent;
            r_mantissa = p2.mantissa;
            r_exponent = p2.exponent;

            int num_bits_result = 2*max(c_mantissa, r_mantissa)+1;
            int cyc_for_init = num_bits_result - min(c_mantissa, r_mantissa); //we only need to initialize the upper bits of the result. lower bits will be initialize by 'copy' instead of 'add' in the first instruction
            int cyc_for_a_plus_b = c_mantissa + 1; //we first perform a_plus_b and save it
            int num_bitpair_evals = r_mantissa; //number of times we look at the bits of x and y
            int cyc_for_each_add = cyc_for_a_plus_b + 1; //we may be adding c_plus_b every time into the partial result

            clocks = cyc_for_init + cyc_for_a_plus_b + (num_bitpair_evals * cyc_for_each_add)*(3/4); //3/4 because 25% of the times both bits wil be 0
        }
    }
    else if (op=="reduce") {
        //reduce by add and shift
        if(precision_list[0].isfloat) dtype="float";
        else dtype = "int";
        PrecisionT::Precision p = precision_list[0];//take operand 0 (src)
        mantissa = p.mantissa;
        exponent = p.exponent;
        bits = p.bits();
        if (dtype=="float") {
            clocks = 0;
            for (int i=1; i<=levels; i++) {
                int powi2 = pow(i-1,2);
                int cycles_to_add = 2 * mantissa * exponent + 9 * mantissa + 7 * exponent + 12;
                clocks += cycles_to_add; //add
                clocks += cycles_to_add * powi2; //shift
            }
        }
        else {
            clocks = 0;
            for (int i=1; i<=levels; i++) {
                int powi2 = pow(i-1,2);
                int cycles_to_add = mantissa + i;
                clocks += cycles_to_add; //add
                clocks += cycles_to_add * powi2; //shift
            }
        }
    
    }
    else if (op=="RowReduce_WithinTile") {
        if(precision_list[0].isfloat) dtype="float";
        else dtype = "int";
        PrecisionT::Precision p = precision_list[0];//take operand 0 (src)
        mantissa = p.mantissa;
        exponent = p.exponent;
        bits = p.bits();
        if (dtype=="float") {
            clocks = 0;
            for (int i=1; i<=levels; i++) {
                int powi2 = pow(i-1,2);
                int cycles_to_add = 2 * mantissa * exponent + 9 * mantissa + 7 * exponent + 12;
                clocks += cycles_to_add; //add
                clocks += i * precision_list[0].bits(); //concurrent tilesend_receive
            }
        }
        else {
            clocks = 0;
            for (int i=1; i<=levels; i++) {
                int powi2 = pow(i-1,2);
                int cycles_to_add = mantissa + i;
                clocks += cycles_to_add; //add
                clocks += i * precision_list[0].bits(); //concurrent tilesend_receive
            }
        }
    
    }
    else if (op=="read" || op=="write") {
        PrecisionT::Precision p = precision_list[0];//take operand 0 (cram)
        bits = p.bits();
        clocks = bits;
    }
    else {
        //unsopported operation
        clocks = -1;
    }

    return clocks;
}