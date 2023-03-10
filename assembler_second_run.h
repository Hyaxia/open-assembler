#ifndef ASSEMBLER_SECOND_RUN_H_ /* Include guard */
#define ASSEMBLER_SECOND_RUN_H_

#include "assembler.h"
#define ARE_SIZE 2
#define TYPE_SIZE 2
#define OPCODE_SIZE 4
#define PRAM_SIZE 2
#define REG_SIZE 6

#define IMMIEDIATE_SIZE 12
#define SYMBOL_SIZE 12

#define ARE_INDEX 0
#define TYPE_DES_INDEX 2
#define TYPE_SRC_INDEX 4
#define OPCODE_INDEX 6
#define PRAM_1_INDEX 12
#define PRAM_2_INDEX 10

#define DEST_REG_INDEX 2
#define SRC_REG_INDEX 8

#define IMMIEDIATE_INDEX 2
#define SYMBOL_INDEX 2


/* performs logic for first run of assembler */
int instraction_to_data(Instruction instruction, Symbol *symbols, int symbols_len, Data *data_to_fill);
AssemblerSecondRunResult assembler_second_run(AssemblerFirstRunResult firstRunResult);
#endif /* ASSEMBLER_SECOND_RUN_H_ */
