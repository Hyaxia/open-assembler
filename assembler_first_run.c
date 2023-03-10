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

AssemblerFirstRunResult assembler_first_run(char *file_path) {
    FILE *fp;
    char line[MAX_LINE_LEN];
    char *no_macro_file_path, *current_word, *symbol_name, *word;
    int IC = 0, DC = 0;                 /* current counter for instructions */
    int line_num = 0;                   /* start at 0, as we enter to while we increase by 1 */
    int has_symbol = 0, has_errors = 0; /* flags */
    int current_word_len, symbol_name_len, symbols_len = 0, instructions_len = 0;
    int datas_allocated = MAX_LINE_LEN, symbols_allocated = MAX_LINE_LEN, instructions_allocated = MAX_LINE_LEN;
    int instruction_code;
    Symbol *symbols;
    Data *datas;
    Instruction *instructions;
    Result res;
    AssemblerFirstRunResult assembler_res;

    no_macro_file_path = malloc(sizeof(char) * (strlen(file_path) + strlen(PRE_ASSEMBLER_OUTPUT_EXTENSION)));
    replace_extension(no_macro_file_path, file_path, PRE_ASSEMBLER_OUTPUT_EXTENSION);

    fp = fopen(no_macro_file_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "File \"%s\" not found\n", no_macro_file_path);
        free(no_macro_file_path);
        assembler_res.has_errors = 1;
        return assembler_res;
    }

    current_word = malloc(sizeof(char) * MAX_LINE_LEN);
    symbol_name = malloc(sizeof(char) * MAX_LINE_LEN);
    symbols = malloc(sizeof(Symbol) * symbols_allocated);
    datas = malloc(sizeof(Data) * datas_allocated);
    instructions = malloc(sizeof(Instruction) * instructions_allocated);

    while (fgets(line, MAX_LINE_LEN, fp)) {
        has_symbol = 0; /* turn symbol flag off */
        line_num++;
        /* here starts the logic for processing the line */
        if (is_all_spaces_or_newline(line) == 1) {
            continue;
        }
        word = strtok(line, " ");
        current_word_len = word_trim_spaces(current_word, word);
        if (is_symbol_deinition(current_word, current_word_len)) /* handle symbol */
        {
            if (symbols_len == symbols_allocated) /* perform reallocation of symbols table if necessary */
            {
                symbols_allocated = symbols_allocated + symbols_allocated;
                symbols = realloc(symbols, sizeof(Symbol) * symbols_allocated);
            }
            symbol_name_len = remove_last_char(symbol_name, current_word,
                                               current_word_len); /* we want to remove the `:` char */
            if (!is_symbol_name_valid(symbol_name, symbol_name_len)) {
                log_error("invalid symbol name", no_macro_file_path, line_num);
                has_errors = 1;
                continue;
            }
            if (is_symbol_defined(symbol_name, symbols, symbols_len)) {
                log_error("symbol already defined", no_macro_file_path, line_num);
                has_errors = 1;
                continue;
            }
            has_symbol = 1; /* turn the symbol flag on*/
        }
        if (has_symbol) {
            word = strtok(NULL, " "); /* we already saved the symbol name, so move to the next value */
            word_trim_spaces(current_word, word);
        }
        if (strcmp(current_word, ".data") == 0 || strcmp(current_word, ".string") == 0) {
            if (has_symbol) /* add a `data` symbol to the symbol table */
            {
                res = add_data_symbol(symbols, symbols_len, symbol_name, symbol_name_len, DC);
                symbols_len = symbols_len + res.len;
            }
            if (strcmp(current_word, ".data") == 0) /* handle data symbol */
            {
                res = store_dot_data(&datas[DC], word, no_macro_file_path, line_num);
            }
            if (strcmp(current_word, ".string") == 0) /* handle dot string */
            {
                res = store_dot_string(&datas[DC], word, no_macro_file_path, line_num);
            }
            if (res.has_errors == 1) {
                has_errors = 1;
            }
            /* handle memory allocation for data array */
            if (DC > datas_allocated - MAX_LINE_LEN) {
                datas_allocated = datas_allocated + datas_allocated;
                datas = realloc(datas, sizeof(Data) * (datas_allocated));
            }
            DC = DC + res.len;
            continue; /* we are done handling this line */
        }
        if (strcmp(current_word, ".extern") == 0) {
            res = add_external_symbol(symbols, symbols_len, word);
            symbols_len = symbols_len + res.len;
            continue; /* we are done handling this line */
        }
        if (strcmp(current_word, ".entry") == 0) {
            continue; /* we are done handling this line */
        }
        if (has_symbol) { /* add symbol of type `code` */
            res = add_code_symbol(symbols, symbols_len, symbol_name, symbol_name_len, IC);
            symbols_len++;
        }

        if (instructions_len == instructions_allocated) { /* make sure we have enough allocated instructions */
            instructions_allocated = instructions_allocated + instructions_allocated;
            instructions = realloc(instructions, sizeof(Instruction) * instructions_allocated);
        }

        instruction_code = get_instruction_code(current_word);
        if (instruction_code == -1) {
            log_error("instruction doesnt exist", no_macro_file_path, line_num);
            has_errors = 1;
            continue;
        }
        res = handle_instruction(&instructions[instructions_len], instruction_code, IC);
        if (res.has_errors == 1) {
            has_errors = 1;
            continue;
        }
        IC += res.len;
        instructions_len++;
    }

    free(current_word);
    free(symbol_name);
    free(no_macro_file_path);
    fclose(fp);

    assembler_res.datas = datas;
    assembler_res.datas_len = DC;
    assembler_res.instructions = instructions;
    assembler_res.instructions_len = instructions_len;
    assembler_res.symbols = symbols;
    assembler_res.symbols_len = symbols_len;
    assembler_res.has_errors = has_errors;
    return assembler_res;
}
