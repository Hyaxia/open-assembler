#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_utils.h"
#include "macro.h"
#include "config.h"
#include "symbol.h"
#include "error_handling.h"
#include "pre_assembler.h"
#include "data.h"
#include "instruction.h"
#include "assembler.h"

AssemblerResult assembler_first_run(char *file_path)
{
    FILE *fp;
    size_t len = 0;
    intmax_t read;
    char *line = NULL, *no_macro_file_path, *current_word, *symbol_name, *word;
    int IC = 0;                         /* current counter for instructions */
    int line_num = 0;                   /* we start from 0 becuase as soon as we enter the while loop we increase the line_num */
    int has_symbol = 0, has_errors = 0; /* flags */
    int current_word_len, symbol_name_len, symbols_len = 0, symbols_allocated = 3;
    int datas_allocated = MAX_LINE_LEN, datas_len = 0;
    int instruction_code;
    Symbol *symbols;
    Data *datas;
    Result res;
    AssemblerResult assembler_res;

    no_macro_file_path = malloc(sizeof(char) * (strlen(file_path) + strlen(PRE_ASSEMBLER_OUTPUT_EXTENSION)));
    replace_extension(no_macro_file_path, file_path, PRE_ASSEMBLER_OUTPUT_EXTENSION);

    fp = fopen(no_macro_file_path, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "File \"%s\" not found\n", no_macro_file_path);
        free(no_macro_file_path);
        assembler_res.has_errors = 1;
        return assembler_res;
    }

    current_word = malloc(sizeof(char) * MAX_LINE_LEN);
    symbol_name = malloc(sizeof(char) * MAX_LINE_LEN);
    symbols = malloc(sizeof(Symbol) * symbols_allocated);
    datas = malloc(sizeof(Data) * datas_allocated);

    while ((getline(&line, &len, fp)) != -1)
    {
        /* preparations for new line */
        if (has_symbol && symbols_len == symbols_allocated) /* perform reallocation of symbols table if necessary */
        {
            symbols = realloc(symbols, sizeof(Symbol) * (symbols_allocated + symbols_allocated));
            symbols_allocated = symbols_allocated + symbols_allocated;
        }
        has_symbol = 0;
        line_num++;
        /* here starts the logic for processing the line */
        if (is_all_spaces_or_newline(line, len) == 1)
        {
            continue;
        }
        word = strtok(line, " ");
        current_word_len = word_trim_spaces(current_word, word);
        if (is_symbol_deinition(current_word, current_word_len)) /* handle symbol */
        {
            symbol_name_len = remove_last_char(symbol_name, current_word, current_word_len); /* we want to remove the `:` char */
            if (!is_symbol_name_valid(symbol_name, symbol_name_len))
            {
                log_error("invalid symbol name", no_macro_file_path, line_num);
                has_errors = 1;
                continue;
            }
            if (is_symbol_defined(symbol_name, symbols, symbols_len))
            {
                log_error("symbol already defined", no_macro_file_path, line_num);
                has_errors = 1;
                continue;
            }
            has_symbol = 1;
        }
        if (has_symbol)
        {
            word = strtok(NULL, " ");
            word_trim_spaces(current_word, word);
        }
        if (strcmp(current_word, ".data") == 0 || strcmp(current_word, ".string") == 0)
        {
            if (has_symbol) /* add a `data` symbol */
            {
                res = add_data_symbol(symbols, symbols_len, symbol_name, symbol_name_len, datas_len);
                symbols_len = symbols_len + res.len;
            }
            if (strcmp(current_word, ".data") == 0)
            {
                res = store_dot_data(&datas[datas_len], word, no_macro_file_path, line_num);
            }
            if (strcmp(current_word, ".string") == 0)
            {
                res = store_dot_string(&datas[datas_len], word, no_macro_file_path, line_num);
            }
            if (res.has_errors == 1)
            {
                has_errors = 1;
            }
            /* handle memory allocation for data array */
            if (datas_len > datas_allocated - MAX_LINE_LEN)
            {
                datas_allocated = datas_allocated + datas_allocated;
                datas = realloc(datas, sizeof(Data) * (datas_allocated));
            }
            datas_len = datas_len + res.len; /* set the length of Data array, basically the `DC` */
            continue;
        }
        if (strcmp(current_word, ".extern") == 0)
        {
            res = add_external_symbol(symbols, symbols_len, word);
            symbols_len = symbols_len + res.len;
            continue;
        }
        if (strcmp(current_word, ".entry") == 0)
        {
            continue;
        }
        if (has_symbol)
        {
            res = add_code_symbol(symbols, symbols_len, symbol_name, symbol_name_len, IC);
            symbols_len = symbols_len + res.len;
        }

        instruction_code = get_instruction_code(current_word);
        if (instruction_code == -1)
        {
            log_error("instruction doesnt exist", no_macro_file_path, line_num);
            has_errors = 1;
        }
    }

    int i;
    for (i = 0; i < symbols_len; i++)
    {
        printf("symbol %s of type - %s with counter - %d\n", symbols[i].name, symbols[i].type, symbols[i].counter);
    }

    for (i = 0; i < datas_len; i++)
    {
        Data data = datas[i];
        for (int j = 0; j < 14; j++)
        {
            printf("%d", data.code[j]);
        }
        printf("\n");
    }

    if (line)
        free(line);

    free(no_macro_file_path);
    fclose(fp);
//    free(datas);
    assembler_res.datas = datas;
    assembler_res.symbols = symbols;
    assembler_res.has_errors = has_errors;
    return assembler_res;
}
