#include <string.h>
#include <stdlib.h>
#include "macro.h"
#include "string_utils.h"

int free_macros(Macro *macros, int macros_size) {
    int index = 0;
    Macro *macro_pt = macros;
    while (index < macros_size) {
        free(macros->name);
        free(macros->content);
        macros++;
        index++;
    }
    free(macro_pt);
    return 0;
}

Macro *get_macro(char *trimmed_line, char *line, Macro *macros, int macros_size, int line_len) {
    int index = 0;

    trim_whitespaces(trimmed_line, line_len, line);

    while (index < macros_size) {
        if (strcmp(trimmed_line, macros->name) == 0) {
            return macros;
        }
        macros++;
        index++;
    }
    return NULL;
}
