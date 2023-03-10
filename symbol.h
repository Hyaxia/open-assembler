#ifndef SYMBOL_H_ /* Include guard */
#define SYMBOL_H_

#define SYMBOL_MAX_LEN 31 /* 30 + 1 which is the null terminator `\0` */
#define SYMBOL_TYPE_MAX_LEN 10

#include "global.h"
typedef enum{
    External_Symbol,
    Data_Symbol,
    Code_Symbol
}SymbolType;

typedef struct {
    char *name;
    int counter;
    SymbolType type;
} Symbol;


/* a method that reutrns 1 if the word is a lable and 0 otherwise */
int is_symbol_definition(char *word, int word_len);

/* check if the word passed is a valid symbol */
int is_symbol_name_valid(char *word, int word_len);

/* Check if symbol is already defined in a list of symbols */
Symbol* get_symbol(char *symbol_name, Symbol *symbols, int symbols_len);

/* adds an external type symbol to the symbols array */
Result add_external_symbol(Symbol *symbols, int symbols_len, char *symbol_name, int symbol_name_len);

/* adds a data type symbol to the symbols array */
Result add_data_symbol(Symbol *symbols, int symbols_len, char *symbol_name, int symbol_name_len, int DC);

/* adds a code type symbol to the symbols array */
Result add_code_symbol(Symbol *symbols, int symbols_len, char *symbol_name, int symbol_name_len, int IC);

#endif /* SYMBOL_H_ */
