#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_utils.h"
#include "global.h"
#include "symbol.h"
#include "error_handling.h"
#include "pre_assembler.h"
#include "data.h"
#include "instruction.h"
#include "assembler.h"

AssemblerSecondRunResult assembler_second_run(AssemblerFirstRunResult firstRunResult) {
    /*
     * go over all instructions in first result
     *  - for each instruction, if it has a symbol, get the IC/DC from the symbol
     *  - pass the instruction and the extra data to a function that will convert it to binary
     *
     * */
}
