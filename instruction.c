#include <string.h>
#include "instruction.h"
#include "string_utils.h"

char *instruction_names[16] = {
        "mov", "cmp", "add", "sub", "not", "clr",
        "lea", "inc", "dec", "jmp", "bne", "red",
        "prn", "jsr", "rts", "stop"};

int get_instruction_code(char *word) {
    int code;
    for (code = mov; code <= stop; code++) {
        if (strcmp(instruction_names[code], word) == 0) {
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
    int reg;
    for (reg = r0; reg <= r7; reg++) {
        if (strcmp(register_names[reg], word) == 0) {
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

AddressType handle_address_type(char *word) {
    if (is_immediate(word))
        return Immediate;
    else if (is_register(word))
        return Register;
    else
        return Tag;
}

Result handle_instruction_with_two_operands(Instruction *instruction) {
    Result res;
    extract_two_operands(instruction->src_operand, instruction->dest_operand);
    instruction->src_operand_address_type = handle_address_type(instruction->src_operand);
    instruction->dest_operand_address_type = handle_address_type(instruction->dest_operand);
    res.len = 2;
    if (instruction->src_operand_address_type != Register || instruction->dest_operand_address_type != Register) {
        res.len++;
    }
    res.has_errors = 0;
    return res;
}

Result handle_instruction_without_operands(void) {
    Result res;
    res.len = 1;
    res.has_errors = 0;
    return res;
}

Result handle_instruction_with_one_operand(Instruction *instruction, int instruction_code) {
    Result res;
    char *line, *operand, *first_parameter, *second_parameter;
    instruction->src_operand_address_type = None;
    if (instruction_code == jsr || instruction_code == bne || instruction_code == jmp) {
        line = strtok(NULL, ""); /* get the rest of the line */
        if (includes_brackets(line)) { /* check if we have parameters */
            /* TODO: add validation that the structure of the instruction is correct (brackets and backticks and such...) */
            operand = strtok(line, "(");
            first_parameter = strtok(NULL, ",");
            second_parameter = strtok(NULL, ")"); /* second parameter without the closing bracket */
            word_trim_spaces(instruction->dest_operand, operand);
            word_trim_spaces(instruction->first_param, first_parameter);
            word_trim_spaces(instruction->second_param, second_parameter);
            instruction->dest_operand_address_type = Jump_with_params;
            instruction->first_param_address_type = handle_address_type(instruction->first_param);
            instruction->second_param_address_type = handle_address_type(instruction->second_param);
            res.len = 3;
            if (instruction->first_param_address_type != Register ||
                instruction->second_param_address_type != Register) {
                res.len++;
            }
        } else { /* instruction without parameters */
            operand = strtok(line, ",");
            word_trim_spaces(instruction->dest_operand, operand);
            instruction->dest_operand_address_type = handle_address_type(instruction->dest_operand);
            res.len = 2;
        }
    } else {
        extract_single_operand(instruction->dest_operand);
        instruction->dest_operand_address_type = handle_address_type(instruction->dest_operand);
        res.len = 2;
    }
    res.has_errors = 0;
    return res;
}

Result handle_instruction(Instruction *instruction, int instruction_code, int IC) {
    Result res;
    int num_of_operands;
    instruction->opcode = instruction_code;
    instruction->IC = IC;
    num_of_operands = get_num_of_operands(instruction_code);
    if (num_of_operands == 0) {
        res = handle_instruction_without_operands();
    } else if (num_of_operands == 1) {
        res = handle_instruction_with_one_operand(instruction, instruction_code);
    } else {
        res = handle_instruction_with_two_operands(instruction);
    }
    instruction->number_of_lines = res.len;
    return res;
}


