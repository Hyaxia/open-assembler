#ifndef ASSEMBLER_SECOND_RUN_H_ /* Include guard */
#define ASSEMBLER_SECOND_RUN_H_

#include "assembler.h"

/* performs logic for first run of assembler */
int parse_instraction(Instruction instruction, Symbol *symbols, int symbols_len, Data *data_to_fill);
AssemblerSecondRunResult assembler_second_run(AssemblerFirstRunResult firstRunResult);
#endif /* ASSEMBLER_SECOND_RUN_H_ */
