#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "symbol.h"
#include "config.h"
#include "string_utils.h"

int is_symbol_defined(char *symbol_name, Symbol *symbols, int symbols_len)
{
    int index;
    for (index = 0; index < symbols_len; index++)
    {
        if (strcmp(symbols[index].name, symbol_name) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int is_symbol_deinition(char *word, int word_len)
{
    if (*(word + word_len - 1) == ':') /* its a symbol if the last char is `:` */
    {
        return 1;
    }
    return 0;
}

int is_symbol_name_valid(char *word, int word_len)
{
    int index;
    if (word_len > SYMBOL_MAX_LEN)
    {
        return 0;
    }
    index = 0;
    while (index < word_len)
    {
        if (!isalnum(*word))
        {
            return 0;
        }
        index++;
        word++;
    }
    return 1;
}

Result add_external_symbol(Symbol *symbols, int symbols_len, char *word)
{
    Result res;
    int symbol_name_len;
    char *symbol_name = malloc(sizeof(char) * SYMBOL_MAX_LEN);

    word = strtok(NULL, " ");
    symbol_name_len = word_trim_spaces(symbol_name, word);

    symbols[symbols_len].counter = 0;
    symbols[symbols_len].name = malloc(sizeof(char) * symbol_name_len);
    symbols[symbols_len].type = malloc(sizeof(char) * SYMBOL_TYPE_MAX_LEN);
    strcpy(symbols[symbols_len].name, symbol_name);
    strcpy(symbols[symbols_len].type, "external");
    /* TODO: add a check if there is already the name of the symbol. */

    free(symbol_name);
    res.has_errors = 0;
    res.len = 1;
    return res;
}

Result add_data_symbol(Symbol *symbols, int symbols_len, char *symbol_name, int symbol_name_len, int DC)
{
    Result res;

    symbols[symbols_len].counter = DC;
    symbols[symbols_len].name = malloc(sizeof(char) * symbol_name_len);
    symbols[symbols_len].type = malloc(sizeof(char) * SYMBOL_TYPE_MAX_LEN);
    strcpy(symbols[symbols_len].name, symbol_name);
    strcpy(symbols[symbols_len].type, "data");
    /* TODO: add a check if there is already the name of the symbol. */

    res.has_errors = 0;
    res.len = 1;
    return res;
}

Result add_code_symbol(Symbol *symbols, int symbols_len, char *symbol_name, int symbol_name_len, int IC)
{
    Result res;

    symbols[symbols_len].counter = IC;
    symbols[symbols_len].name = malloc(sizeof(char) * symbol_name_len);
    symbols[symbols_len].type = malloc(sizeof(char) * SYMBOL_TYPE_MAX_LEN);
    strcpy(symbols[symbols_len].name, symbol_name);
    strcpy(symbols[symbols_len].type, "code");
    /* TODO: add a check if there is already the name of the symbol. */
    /* TODO: we need to identify the symbol and handle it */

    res.has_errors = 0;
    res.len = 1;
    return res;
}
