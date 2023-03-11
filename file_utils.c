#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "string_utils.h"
FILE * open_file_with_new_extention(char *original_name,char *new_extention,char *modes) {
    FILE *fp;
    char *no_macro_file_path;
    no_macro_file_path = malloc(sizeof(char) * (strlen(original_name) + strlen(new_extention)));
    replace_extension(no_macro_file_path, original_name, new_extention);

    fp = fopen(no_macro_file_path, modes);
    free(no_macro_file_path);
    return fp;
}
