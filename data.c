#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "string_utils.h"
#include "global.h"
#include "error_handling.h"
#include "data.h"

int num_to_code(int number, int *code, int number_size) {
    int index, current_digit;
    int mask = 1;
    for (index = 0; index < number_size; index++) {
        current_digit = mask & (number >> index);
        code[index] = current_digit; /* we populate the array from the end */
    }
    return 0;
}

int num_to_data(int number, Data *data) {
    return num_to_code(number, data->code, WORD_LEN);
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
        num_to_data(current_num, data);
        data++;
        len++;
        word = strtok(NULL, ",");
    }
    free(current_word);
    res.has_errors = 0;
    res.len = len;
    return res;
}

Result store_dot_string(Data *data, char *content, char *no_macro_file_path, int line_num) {
    Result res;
    int len = 0;
    char *word = strtok(NULL, "");
    while (isspace(*word) && *word != EOL) /* get to first `"`" */
        word++;
    if (*word != '"') {
        log_error("did not find opening quotes", no_macro_file_path, line_num);
        res.has_errors = 1;
        res.len = 0;
        return res;
    }
    word++;              /* skip first `"`" */
    while (*word != '"' && *word != EOL) /* loop until the end of the string */
    {
        num_to_data((int) (*word), data);
        data++;
        len++;
        word++;
    }
    if (*word == EOL) {
        log_error("did not find closing quotes", no_macro_file_path, line_num);
        res.has_errors = 1;
        res.len = 0;
        return res;
    }
    num_to_data(0, data); /* add null terminator */
    data++;
    len++;
    res.has_errors = 0;
    res.len = len;
    return res;
}
