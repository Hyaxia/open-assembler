#ifndef ASSEMBLER_H_ /* Include guard */
#define ASSEMBLER_H_

#include "symbol.h"
#include "data.h"
#include "instruction.h"

typedef struct AssemblerResult {
    int has_errors;
    Symbol *symbols;
    Data *datas;
    Instruction *instructions;
} AssemblerResult;

#endif /* ASSEMBLER_H_ */
