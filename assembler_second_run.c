#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_utils.h"
#include "global.h"
#include "symbol.h"
#include "error_handling.h"
#include "pre_assembler.h"
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
    int i;
    Instruction last_instraction = firstRunResult.instructions[firstRunResult.instructions_len - 1];
    Data *datas = malloc(sizeof(Data) * last_instraction.IC + last_instraction.number_of_lines);
    for (i = 0; i < firstRunResult.instructions_len; i++) {
        instraction_to_data(firstRunResult.instructions[i], firstRunResult.symbols,
                            firstRunResult.symbols_len, datas);
    }
    AssemblerSecondRunResult a;
    return a;
}
AER get_AER_from_SymbolType(SymbolType symbolType)/*TODO: is this right?*/
{
    if(symbolType == External_Symbol)
        return E;
    else
        return R;
}

void create_instraction_row(Instruction instruction, Data *data_to_fill) {
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
void create_lable_row(Symbol symbol , Data *data_to_fill) {
    num_to_code(get_AER_from_SymbolType(symbol.type), &data_to_fill->code[ARE_INDEX], ARE_SIZE);
    num_to_code(symbol.counter, &data_to_fill->code[SYMBOL_INDEX], SYMBOL_SIZE);
}

int get_register_number(char *reg_string) {
    return atoi(reg_string + 1); /*skip the r in the begging*/
}

int get_immidate_value(char *immidate_string) {
    return atoi(immidate_string +1); /*skip the # in the begging*/
}

int instraction_to_data(Instruction instruction, Symbol *symbols, int symbols_len, Data *data_to_fill) {
/* test oprands and params types
 * based of the type we know how to parse it to bin
 */
    int current_line = 0;
    create_instraction_row(instruction, &data_to_fill[instruction.IC + current_line++]);
    if (instruction.src_operand_address_type == Register) {
        if (instruction.dest_operand_address_type == Register) {
            create_regs_row(get_register_number(instruction.src_operand),
                            get_register_number(instruction.dest_operand),
                            &data_to_fill[instruction.IC + current_line++]);
        } else {
            create_regs_row(get_register_number(instruction.src_operand),
                            0, &data_to_fill[instruction.IC + current_line++]);
        }
    } else if (instruction.src_operand_address_type == Immediate) {
        create_immediate_row(get_immidate_value(instruction.src_operand),
                             &data_to_fill[instruction.IC + current_line++]);
    } else{
        return -1; //Error
    }

    if(current_line == instruction.number_of_lines)
    {
        return 0; //Finished
    }else if(current_line > instruction.number_of_lines)
    {
        return -1; //ERROR
    }
    else{
        if (instruction.dest_operand_address_type == Tag){
            Symbol* symbol_p = get_symbol(instruction.dest_operand,symbols,symbols_len);
            if(symbol_p == NULL){
                return -1; // ERROR
            }
            create_lable_row(*symbol_p,&data_to_fill[instruction.IC + current_line++]);
        }
    }

    return 1;
}

