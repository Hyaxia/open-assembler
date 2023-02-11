#ifndef ASSEMBLER_H_ /* Include guard */
#define ASSEMBLER_H_

#include "symbol.h"
#include "data.h"

typedef struct AssemblerResult
{
    int has_errors;
    Symbol *symbols;
    Data *datas;
} AssemblerResult;

#endif /* ASSEMBLER_H_ */
