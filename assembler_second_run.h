#ifndef ASSEMBLER_SECOND_RUN_H_ /* Include guard */
#define ASSEMBLER_SECOND_RUN_H_

#include "assembler.h"
#define ARE_SIZE 2
#define TYPE_SIZE 2
#define OPCODE_SIZE 4
#define PRAM_SIZE 2
#define REG_SIZE 6

#define IMMIEDIATE_SIZE 12
#define SYMBOL_SIZE 12

#define ARE_INDEX 0
#define TYPE_DES_INDEX 2
#define TYPE_SRC_INDEX 4
#define OPCODE_INDEX 6
#define PRAM_1_INDEX 12
#define PRAM_2_INDEX 10

#define DEST_REG_INDEX 2
#define SRC_REG_INDEX 8

#define IMMIEDIATE_INDEX 2
#define SYMBOL_INDEX 2


/* performs logic for first run of assembler */
int instruction_to_data(Instruction instruction, Symbol *symbols, int symbols_len, Data *data_to_fill);
void update_data_type_symbols_counter(int data_location_offset, Symbol *symbols, int symbols_len);
AER get_AER_from_SymbolType(SymbolType symbolType);
void create_instruction_row(Instruction instruction, Data *data_to_fill);
void create_regs_row(int src_reg, int dest_reg, Data *data_to_fill);
void create_immediate_row(int immediate_value, Data *data_to_fill);
void create_label_row(Symbol symbol, Data *data_to_fill);
int get_register_number(char *reg_string);
int get_immediate_value(char *immediate_string);
int parse_and_create_label_row(char *symbol_str, Symbol *symbols, int symbols_len, Data *data_to_fill);
void parse_and_create_regs_row(char *src_reg, char *dest_reg, Data *data_to_fill);
void parse_and_create_immediate_row(char *immediate_value, Data *data_to_fill);
int add_additional_words(char *first_pram, AddressType first_parm_address_type, char *second_pram,
                         AddressType second_parm_address_type, Data *data_to_fill, Symbol *symbols, int symbols_len, int number_of_words_to_add);
AssemblerSecondRunResult assembler_second_run(AssemblerFirstRunResult firstRunResult);
#endif /* ASSEMBLER_SECOND_RUN_H_ */
