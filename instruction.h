#ifndef INSRTUCTION_H_ /* Include guard */
#define INSRTUCTION_H_

#include "config.h"

typedef struct Instruction {
    int ERA[2];
    int src_counter[2];
    int dest_counter[2];
    int opcode[4];
    int first_param[2];
    int second_param[2];
} Instruction;

enum InstructionCode {
    mov,
    cmp,
    add,
    sub,
    not,
    clr,
    lea,
    inc,
    dec,
    jmp,
    bne,
    red,
    prn,
    jsr,
    rts,
    stop
};

/* returns the instruction code, -1 if the instruction is not recognized */
int get_instruction_code(char *word);

/* returns number of operands for the relevant instruction code, if instruction doesnt exist returns -1 */
int num_of_operands(int instruction_code);

/* receives the relevant instruction and stores it into an Instruction object in a convenient way */
Result handle_instruction(Instruction *instruction);

#endif /* INSRTUCTION_H_ */
