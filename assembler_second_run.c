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
#include "assembler_second_run.h"


AssemblerSecondRunResult assembler_second_run(AssemblerFirstRunResult firstRunResult) {
    /*
     * go over all instructions in first result
     *  - for each instruction, if it has a symbol, get the IC/DC from the symbol
     *  - pass the instruction and the extra data to a function that will convert it to binary
     *
     * */
    int i = 0;
    for (i = 0; i < firstRunResult.instructions_len; i++)
    {
       parse_instraction(firstRunResult.instructions[i],firstRunResult.symbols,
                         firstRunResult.symbols_len,NULL);
    }
    AssemblerSecondRunResult a;
    return a;
}

int parse_instraction(Instruction instruction, Symbol *symbols, int symbols_len, Data *data_to_fill) {
/* test oprands and params types
 * based of the type we know how to parse it to bin
 */
    return 1;
}
