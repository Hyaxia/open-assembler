#include <string.h>
#include "instruction.h"

char *instructions[16] = {
    "mov", "cmp", "add", "sub", "not", "clr",
    "lea", "inc", "dec", "jmp", "bne", "red",
    "prn", "jsr", "rts", "stop"};

int get_instruction_code(char *word)
{
    int code;
    for (code = mov; code <= stop; code++)
    {
        if (strcmp(instructions[code], word) == 0)
        {
            return code;
        }
    }
    return -1;
}

int num_of_operands(int instruction_code)
{
    switch (instruction_code)
    {
    case mov:
    case cmp:
    case add:
    case sub:
    case lea:
        return 2;
    case not:
    case clr:
    case inc:
    case dec:
    case jmp:
    case bne:
    case red:
    case prn:
    case jsr:
        return 1;
    case stop:
    case rts:
        return 0;
    default:
        return -1;
    }
}
