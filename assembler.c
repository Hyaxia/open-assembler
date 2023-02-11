#include <stdio.h>
#include "pre_assembler.h"
#include "assembler_first_run.h"
#include "assembler.h"

int main(int argc, char *argv[]) {
    AssemblerResult res;
    int pre_assembler_result;
    char **file_name;
    if (argc < 2) {
        printf("ERROR: No files specified.\n");
        return -1;
    }
    for (file_name = argv + 1; *file_name != argv[argc]; file_name++) {
        printf("starting to handle %s\n\n", *file_name);
        pre_assembler_result = pre_assembler(*file_name);
        if (pre_assembler_result == 1) /* if there were errors then done perform assembler logic */
        {
            printf("\n\nfinished handling %s\n", *file_name);
            continue;
        }
        res = assembler_first_run(*file_name);
        if (res.has_errors) {
            printf("file %s has errors\n", *file_name);
        }
        printf("\n\nfinished handling %s\n", *file_name);
    }

    return 0;
}
