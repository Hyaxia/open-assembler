#include <string.h>
#include "instruction.h"
#include "string_utils.h"

char *instructions[16] = {
        "mov", "cmp", "add", "sub", "not", "clr",
        "lea", "inc", "dec", "jmp", "bne", "red",
        "prn", "jsr", "rts", "stop"};

char *registers[8] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

int get_instruction_code(char *word) {
    int code;
    for (code = mov; code <= stop; code++) {
        if (strcmp(instructions[code], word) == 0) {
            return code;
        }
    }
    return -1;
}

int get_num_of_operands(int instruction_code) {
    switch (instruction_code) {
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

int is_immediate(char *word) {
    if (*word == '#') {
        return 1;
    }
    return 0;
}

int is_register(char *word) {
    for (int reg = r0; reg <= r7; reg++) {
        if (strcmp(registers[reg], word) == 0) {
            return 1;
        }
    }
    return 0;
}

void extract_single_operand(char *operand) {
    char *word = strtok(NULL, ",");
    word_trim_spaces(operand, word);
}

void extract_two_operands(char *first_operand, char *second_operand) {
    extract_single_operand(first_operand);
    extract_single_operand(second_operand);
}

int handle_address_type(char *word) {
    if (is_immediate(word)) {
        return 0;
    }
    if (is_register(word)) {
        return 3;
    }
    return 1;
}

void handle_instruction_with_two_operands(Instruction *instruction, int instruction_code) {
    extract_two_operands(instruction->first_operand, instruction->second_operand);
    instruction->first_operand_address_type = handle_address_type(instruction->first_operand);
    instruction->second_operand_address_type = handle_address_type(instruction->second_operand);
    instruction->size = 2;
    if (instruction->first_operand_address_type != 3 || instruction->second_operand_address_type != 3) {
        instruction->size++;
    }
}

void handle_instruction_without_operands(Instruction *instruction, int instruction_code) {
    instruction->size = 1;
}

void handle_instruction_with_one_operand(Instruction *instruction, int instruction_code) {
    extract_single_operand(instruction->first_operand);
    instruction->first_operand_address_type = handle_address_type(instruction->first_operand);
    instruction->size = 2;
}


Result handle_instruction(Instruction *instruction, int instruction_code) {
    Result res;
    instruction->opcode = instruction_code;
    int num_of_operands = get_num_of_operands(instruction_code);
    if (num_of_operands == 0) {
        handle_instruction_without_operands(instruction, instruction_code);
    } else if (num_of_operands == 1) {
        handle_instruction_with_one_operand(instruction, instruction_code);
    } else {
        handle_instruction_with_two_operands(instruction, instruction_code);
    }
    return res;
}


