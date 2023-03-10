#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "symbol.h"
#include "data.h"
#include "instruction.h"
#include "assembler.h"
#include "assembler_second_run.h"


AssemblerSecondRunResult assembler_second_run(AssemblerFirstRunResult firstRunResult) {
    /*
     * go over all instructions in first result
     *  - for each instruction, if it has a symbol, get the IC/DC from the symbol
     *  - pass the instruction and the extra data to a function that will convert it to binary
     *
     * */
    int i, code_len;
    Data *datas;
    AssemblerSecondRunResult assemblerSecondRunResult;
    Instruction last_instraction = firstRunResult.instructions[firstRunResult.instructions_len - 1];
    code_len = last_instraction.IC - IC_OFFSET + last_instraction.number_of_lines;
    update_data_type_symbols_counter(code_len + IC_OFFSET, firstRunResult.symbols, firstRunResult.symbols_len);
    datas = (Data *)malloc(sizeof(Data) * code_len);
    for (i = 0; i < firstRunResult.instructions_len; i++) {
        instruction_to_data(firstRunResult.instructions[i], firstRunResult.symbols,
                            firstRunResult.symbols_len, datas);
    }
    assemblerSecondRunResult.code_len = code_len;
    assemblerSecondRunResult.code = datas;

    return assemblerSecondRunResult;
}

void update_data_type_symbols_counter(int data_location_offset, Symbol *symbols, int symbols_len) {
    int i;
    for (i = 0; i < symbols_len; i++) {
        if (symbols[i].type == Data_Symbol) {
            symbols[i].counter += data_location_offset;
        }
    }
}

AER get_AER_from_SymbolType(SymbolType symbolType) {
    if (symbolType == External_Symbol)
        return E;
    else
        return R;
}

void create_instruction_row(Instruction instruction, Data *data_to_fill) {
    num_to_code(A, &data_to_fill->code[ARE_INDEX], ARE_SIZE);
    num_to_code(instruction.dest_operand_address_type, &data_to_fill->code[TYPE_DES_INDEX], TYPE_SIZE);
    num_to_code(instruction.src_operand_address_type, &data_to_fill->code[TYPE_SRC_INDEX], TYPE_SIZE);
    num_to_code(instruction.opcode, &data_to_fill->code[OPCODE_INDEX], OPCODE_SIZE);
    num_to_code(instruction.second_param_address_type, &data_to_fill->code[PRAM_2_INDEX], PRAM_SIZE);
    num_to_code(instruction.first_param_address_type, &data_to_fill->code[PRAM_1_INDEX], PRAM_SIZE);
}

void create_regs_row(int src_reg, int dest_reg, Data *data_to_fill) {
    num_to_code(A, &data_to_fill->code[ARE_INDEX], ARE_SIZE);
    num_to_code(dest_reg, &data_to_fill->code[DEST_REG_INDEX], REG_SIZE);
    num_to_code(src_reg, &data_to_fill->code[SRC_REG_INDEX], REG_SIZE);
}

void create_immediate_row(int immediate_value, Data *data_to_fill) {
    num_to_code(A, &data_to_fill->code[ARE_INDEX], ARE_SIZE);
    num_to_code(immediate_value, &data_to_fill->code[IMMIEDIATE_INDEX], IMMIEDIATE_SIZE);
}

void create_label_row(Symbol symbol, Data *data_to_fill) {
    num_to_code(get_AER_from_SymbolType(symbol.type), &data_to_fill->code[ARE_INDEX], ARE_SIZE);
    num_to_code(symbol.counter, &data_to_fill->code[SYMBOL_INDEX], SYMBOL_SIZE);
}

int get_register_number(char *reg_string) {
    /*If the reg_string is empty will return 0*/
    if (strcmp(reg_string, "") == 0)
        return 0;
    else
        return atoi(reg_string + 1); /*skip the r in the begging*/
}

int get_immediate_value(char *immediate_string) {
    return atoi(immediate_string + 1); /*skip the # in the begging*/
}


int parse_and_create_label_row(char *symbol_str, Symbol *symbols, int symbols_len, Data *data_to_fill) {
    Symbol *symbol_p = get_symbol(symbol_str, symbols, symbols_len);
    if (symbol_p == NULL) {
        return -1; /* ERROR*/
    }
    create_label_row(*symbol_p, data_to_fill);
    return 0;
}

void parse_and_create_regs_row(char *src_reg, char *dest_reg, Data *data_to_fill) {
    create_regs_row(get_register_number(src_reg), get_register_number(dest_reg), data_to_fill);
}

void parse_and_create_immediate_row(char *immediate_value, Data *data_to_fill) {
    create_immediate_row(get_immediate_value(immediate_value), data_to_fill);
}

int add_additional_words(char *first_pram, AddressType first_parm_address_type, char *second_pram,
                         AddressType second_parm_address_type, Data *data_to_fill, Symbol *symbols, int symbols_len, int number_of_words_to_add) {
    int offset = 0;
    if(number_of_words_to_add == offset){
        return 0; /*finish*/
    }
    if (first_parm_address_type == Register) {
        if (second_parm_address_type == Register) {
            parse_and_create_regs_row(first_pram,
                                      second_pram,
                                      &data_to_fill[offset++]);
        } else {
            parse_and_create_regs_row(first_pram,
                                      "", &data_to_fill[offset++]);
        }
    } else if (first_parm_address_type == Immediate) {
        parse_and_create_immediate_row(first_pram,
                                       &data_to_fill[offset++]);
    } else if (first_parm_address_type == Tag) {
        parse_and_create_label_row(first_pram, symbols, symbols_len, &data_to_fill[offset++]);
    }
    if(number_of_words_to_add == offset){
        return 0; /*finish*/
    }
    if (second_parm_address_type == Tag) {
        parse_and_create_label_row(second_pram, symbols, symbols_len, &data_to_fill[offset++]);
    } else if (second_parm_address_type == Register) {
        parse_and_create_regs_row("", second_pram,
                                  &data_to_fill[offset++]);
    } else if (first_parm_address_type == Immediate) {
        parse_and_create_immediate_row(second_pram,
                                       &data_to_fill[offset++]);
    }
    return 1;
}

int instruction_to_data(Instruction instruction, Symbol *symbols, int symbols_len, Data *data_to_fill) {
/* test oprands and params types
 * based of the type we know how to parse it to bin
 */
    int current_line, instruction_offset;
    current_line = 0;
    instruction_offset = instruction.IC - IC_OFFSET;
    create_instruction_row(instruction, &data_to_fill[instruction_offset + current_line++]);
    if (instruction.src_operand_address_type == None) {
        if (instruction.dest_operand_address_type == Jump_with_params || instruction.dest_operand_address_type == Tag) {
            parse_and_create_label_row(instruction.dest_operand, symbols, symbols_len,
                                       &data_to_fill[instruction_offset + current_line++]);
        } else if (instruction.dest_operand_address_type == Register) {
            parse_and_create_regs_row(instruction.dest_operand, "", &data_to_fill[instruction_offset + current_line++]);
        } else if (instruction.dest_operand_address_type == Immediate) {
            parse_and_create_immediate_row(instruction.dest_operand,
                                           &data_to_fill[instruction_offset + current_line++]);
        }

        if (instruction.dest_operand_address_type == Jump_with_params) {
            add_additional_words(instruction.first_param, instruction.first_param_address_type,
                                 instruction.second_param,
                                 instruction.second_param_address_type,
                                 &data_to_fill[instruction_offset + current_line],
                                 symbols,
                                 symbols_len, instruction.number_of_lines - current_line);
        }
    } else
        add_additional_words(instruction.src_operand, instruction.src_operand_address_type, instruction.dest_operand,
                             instruction.dest_operand_address_type, &data_to_fill[instruction_offset + current_line],
                             symbols,
                             symbols_len, instruction.number_of_lines - current_line);
    return 1;
}

