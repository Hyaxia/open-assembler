#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macro.h"
#include "pre_assembler.h"
#include "string_utils.h"
#include "global.h"
#include "error_handling.h"

int pre_assembler(char *file_path) {
    FILE *fp, *no_macro_fp;
    char line[MAX_LINE_LEN];
    char *trimmed_line, *no_macro_file_path, *macro_name;
    size_t len = 0;
    int has_errors = 0, macros_size = 0, macros_allocated = 3, inside_macro = 0, line_num = 1;
    Macro *macros, *target_macro;
    int new_macro_content_alloc = sizeof(char) * MACRO_MAX_LENGTH;

    fp = fopen(file_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "File \"%s\" not found\n", file_path);
        return 1;
    }

    trimmed_line = malloc(sizeof(char) * MACRO_MAX_LENGTH);
    no_macro_file_path = malloc(sizeof(char) * (strlen(file_path) + strlen(PRE_ASSEMBLER_OUTPUT_EXTENSION)));

    macros = malloc(sizeof(Macro) * macros_allocated);

    replace_extension(no_macro_file_path, file_path, PRE_ASSEMBLER_OUTPUT_EXTENSION);
    no_macro_fp = fopen(no_macro_file_path, "w+");
    while (fgets(line,MAX_LINE_LEN,fp)){
        /* if line too long, the buffer doesn't include the '\n' char OR the file isn't on end. */
        if (strchr(line, NEW_LINE) == NULL && !feof(fp)) {
            log_error("line is longer than 80 characters", file_path, line_num);
            has_errors = 1;
        }
        if (inside_macro == 1) {
            if (has_word(line, MACRO_END, MACRO_END_LEN)) {
                macros_size++;
                if (macros_size == macros_allocated) /* if about to reach full memory of allocated macros */
                {
                    macros = realloc(macros, sizeof(Macro) * (macros_allocated + macros_allocated));
                    macros_allocated = macros_allocated + macros_allocated;
                }
                inside_macro = 0;
                continue;
            }
            fputs(line, no_macro_fp);
            if (len + macros[macros_size].content_size > new_macro_content_alloc) {
                macros[macros_size].content = realloc(macros[macros_size].content,
                                                      (new_macro_content_alloc + MAX_LINE_LEN * 3) * sizeof(char));
            }
            strcat(macros[macros_size].content, line);
        } else {
            if (has_word(line, MACRO_START, MACRO_START_LEN)) {
                strtok(line, " ");
                macro_name = strtok(NULL, " ");
                if (macro_name == NULL || isspace(*macro_name)) {
                    log_error("mcr declaration without mcr name", file_path, line_num);
                    has_errors = 1;
                    continue;
                }
                trim_whitespaces(trimmed_line, MAX_LINE_LEN, macro_name);
                macros[macros_size].name = malloc(sizeof(char) * MACRO_MAX_LENGTH);
                macros[macros_size].content = malloc(sizeof(char) * MAX_LINE_LEN);
                strcpy(macros[macros_size].name, trimmed_line);
                inside_macro = 1;
                continue;
            }
            target_macro = get_macro(trimmed_line, line, macros, macros_size, len);
            if (target_macro != NULL) {
                if (!has_errors) {
                    fputs(target_macro->content, no_macro_fp);
                }
                continue;
            }
            if (!has_errors) {
                fputs(line, no_macro_fp);
            }
        }
        line_num++;
    }

    free(trimmed_line);
    free(no_macro_file_path);
    free_macros(macros, macros_size);
    fclose(fp);
    fclose(no_macro_fp);
    /*
        TODO: check if we need to delete the `no_macro_fp` if there are errors
    */
    return has_errors;
}
