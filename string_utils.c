#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "string_utils.h"

int word_trim_spaces(char *out, char *str) {
    int len = 0;
    int index = 0;
    int inside_word = 0;
    while (str[index] != '\0' && str[index] != '\n') {
        if (isspace(str[index])) {
            if (inside_word) /* first space after word */
            {
                out[len] = '\0';
                return len;
            }
            index++;
            continue;
        }
        if (!isspace(str[index])) {
            inside_word = 1;
        }
        out[len] = str[index];
        index++;
        len++;
    }
    out[len] = '\0';
    return len;
}

int trim_whitespaces(char *out, int len, char *str) {
    char *end;
    int out_size;

    if (len == 0)
        return 0;

    /* trim leading spaces */
    while (isspace((unsigned char) *str))
        str++;

    if (*str == 0) /* if all string is spaces */
    {
        *out = 0;
        return 1;
    }

    /* trim trailing spaces */
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end))
        end--;
    end++;

    /* set output size to minimum of trimmed string length and buffer size minus 1 */
    out_size = (end - str) < len - 1 ? (end - str) : len - 1;

    /* copy trimmed string and add null terminator */
    memcpy(out, str, out_size);
    *(out + out_size) = 0;

    return out_size;
}

int has_word(char *line, char *word, int word_len) {
    char *found_pt = strstr(line, word);
    char *right_side = found_pt + word_len;
    if (found_pt == NULL) {
        return 0;
    }
    if (!isspace(*right_side) && *right_side != '\n' &&
        *right_side != '\0') /* there is continuation of the word from the right */
    {
        return 0;
    }
    if (found_pt == line) /* word at start of the line */
    {
        return 1;
    }
    if (!isspace(*(found_pt - 1))) /* middle of string, make sure its the start of the word */
    {
        return 0;
    }
    return 1;
}

void replace_extension(char *new_path, char *original_path, char *new_extension) {
    if (*original_path == '.') /* if relative path */
    {
        *new_path = *original_path;
        new_path++;
        original_path++;
    }

    while (*original_path != '.') {
        *new_path = *original_path;
        new_path++;
        original_path++;
    }
    *new_path = '\0';
    strcat(new_path, new_extension);
}

int get_first_word(char *first_word, char *line, int line_len) {
    int word_size = 0, index = 0;
    while (isspace(*line) && index < line_len && *line != '\n') {
        line++;
        index++;
    }
    if (index + 1 == line_len) /* only spaces */
    {
        return word_size;
    }
    while (!isspace(*line) && index < line_len && *line != '\n') {
        *first_word = *line;
        first_word++;
        line++;
        word_size++;
    }
    *first_word = '\0';
    return word_size;
}

int remove_last_char(char *dest, char *src, int word_len) {
    strcpy(dest, src);
    *(dest + word_len - 1) = '\0';
    return word_len - 1;
}

int is_all_spaces_or_newline(char *word, int word_len) {
//    int index;
//    for (index = 0; index < word_len; index++) {
//        if (isspace(*(word + index)) != 0 || *(word + index) == '\n' || *(word + index) == '\0') {
//            continue;
//        }
//        return 0;
//    }
//    return 1;
    int i = 0;
    while (*(word + i) != '\0' && i < word_len) {
        if (!isspace((unsigned char) *(word + i))) {
            return 0;
        }
        i++;
    }
    return 1;
}
