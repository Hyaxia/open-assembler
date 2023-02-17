#ifndef INSRTUCTION_H_ /* Include guard */
#define INSRTUCTION_H_

#include "config.h"


typedef struct {
    int opcode;
    char first_operand[MAX_LINE_LEN];
    char second_operand[MAX_LINE_LEN];
    int first_operand_address_type;
    int second_operand_address_type;
    char symbol_name[MAX_LINE_LEN];
    char first_param[MAX_LINE_LEN];
    char second_param[MAX_LINE_LEN];
    int first_param_address_type;
    int second_param_address_type;
    int size;
    int IC;
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

enum InstructionRegister {
    r0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7
};

/* returns the instruction code, -1 if the instruction is not recognized */
int get_instruction_code(char *word);

/* returns number of operands for the relevant instruction code, if instruction doesnt exist returns -1 */
int get_num_of_operands(int instruction_code);

/* receives the relevant instruction and stores it into an Instruction object in a convenient way */
Result handle_instruction(Instruction *instruction, int instruction_code);

#endif /* INSRTUCTION_H_ */
