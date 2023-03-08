#ifndef ASSEMBLER_H_ /* Include guard */
#define ASSEMBLER_H_

#include "symbol.h"
#include "data.h"
#include "instruction.h"

typedef struct {
    int has_errors;
    Symbol *symbols;
    int symbols_len;
    Data *datas;
    int datas_len;
    Instruction *instructions;
    int instructions_len;
} AssemblerFirstRunResult;

typedef struct {
    int has_errors;
    Data *datas;
    int datas_len;
    Symbol *symbols;
    int symbols_len;
    Data *instructions;
    int instructions_len;
} AssemblerSecondRunResult;

#endif /* ASSEMBLER_H_ */
