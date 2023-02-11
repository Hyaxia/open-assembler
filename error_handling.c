#include <stdio.h>
#include <stdlib.h>

void log_error(char *message, char *file_path, int line_num) {
    fprintf(stderr, "File: %s, line number %d, error: %s\n", file_path, line_num, message);
}
