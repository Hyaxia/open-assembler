#include <stdio.h>
#include "pre_assembler.h"
#include "assembler_first_run.h"
#include "assembler.h"
#include "instruction.h"

/* TODO: remove this func after development is done*/
void print_result(AssemblerFirstRunResult res) {
    int i, j;
    printf("\n__symbols are__\n");
    for (i = 0; i < res.symbols_len; i++) {
        printf("symbol %s of type - %s with counter - %d\n", res.symbols[i].name, res.symbols[i].type,
               res.symbols[i].counter);
    }

    printf("\n__data is__\n");
    for (i = 0; i < res.datas_len; i++) {
        Data data = res.datas[i];
        for (j = 0; j < 14; j++) {
            printf("%d", data.code[j]);
        }
        printf("\n");
    }

    printf("\n__instructions are__\n");
    for (i = 0; i < res.instructions_len; i++) {
        printf("instruction - op: %s __ IC: %d __ operand1: %s __ operand2: %s __ param1: %s __ param2: %s \n",
               instruction_names[res.instructions[i].opcode],
               res.instructions[i].IC,
               res.instructions[i].first_operand,
               res.instructions[i].second_operand,
               res.instructions[i].first_param,
               res.instructions[i].second_param
        );
    }
}

int main(int argc, char *argv[]) {
    AssemblerFirstRunResult res;
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
        print_result(res);
        res = assembler_second_run(res);
        printf("\n\nfinished handling %s\n", *file_name);
    }

    return 0;
}
