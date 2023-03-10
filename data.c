#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_utils.h"
#include "global.h"
#include "error_handling.h"
#include "data.h"

int num_to_code(int number, Data *data) {
    int index, current_digit;
    int mask = 1;
    for (index = 0; index < WORD_LEN; index++) {
        current_digit = mask & (number >> index);
        data->code[WORD_LEN - 1 - index] = current_digit; /* we populate the array from the end */
    }
    return 0;
}

/* TODO: add error handling */
Result store_dot_data(Data *data, char *content, char *no_macro_file_path, int line_num) {
    Result res;
    long current_num;
    int len = 0;
    char *current_word = malloc(sizeof(char) * MAX_LINE_LEN);

    char *word = strtok(NULL, ","); /* get to the start of the numbers */
    while (word != NULL)            /* loop over the numbers until they end */
    {
        word_trim_spaces(current_word, word);
        if (*current_word == '+') /* if its a positive number noted by `+` then skip to the number itself */
        {
            current_num = strtol((current_word + 1), NULL, 10);
        } else {
            current_num = strtol(current_word, NULL, 10);
        }
        num_to_code(current_num, data);
        data++;
        len++;
        word = strtok(NULL, ",");
    }
    free(current_word);
    res.has_errors = 0;
    res.len = len;
    return res;
}

/* TODO: add error handling */
Result store_dot_string(Data *data, char *content, char *no_macro_file_path, int line_num) {
    Result res;
    int len = 0;
    char *word = strtok(NULL, " ");
    while (*word != '"') /* get to first `"`" */
        word++;
    word++;              /* skip first `"`" */
    while (*word != '"') /* loop until the end of the string */
    {
        num_to_code((int) (*word), data);
        data++;
        len++;
        word++;
    }
    num_to_code(0, data); /* add null terminator */
    data++;
    len++;
    res.has_errors = 0;
    res.len = len;
    return res;
}
