#include <stdlib.h>
#include <stdio.h>
#include "pre_assembler.h"
#include "assembler_first_run.h"
#include "assembler_second_run.h"
#include "assembler.h"
#include "instruction.h"
#include "file_utils.h"

/* TODO: remove this func after development is done*/
void print_result(AssemblerFirstRunResult res) {
    int i, j;
    printf("\n__symbols are__\n");
    for (i = 0; i < res.symbols_len; i++) {
        printf("symbol %s of type - %d with counter - %d\n", res.symbols[i].name, res.symbols[i].type,
               res.symbols[i].counter);
    }

    printf("\n__data(%d) is__\n", res.datas_len);
    for (i = 0; i < res.datas_len; i++) {
        Data data = res.datas[i];
        for (j = 13; j >= 0; j--) {
            printf("%c", data.code[j]);
        }
        printf("\n");
    }

    printf("\n__instructions are__\n");
    for (i = 0; i < res.instructions_len; i++) {
        printf("instruction - op: %s __ IC: %d __ number_of_lines: %d __ src_operand(%d): %s __ dest_operand(%d): %s __ param1(%d): %s __ param2(%d): %s \n",
               instruction_names[res.instructions[i].opcode],
               res.instructions[i].IC,
               res.instructions[i].number_of_lines,
               res.instructions[i].src_operand_address_type,
               res.instructions[i].src_operand,
               res.instructions[i].dest_operand_address_type,
               res.instructions[i].dest_operand,
               res.instructions[i].first_param_address_type,
               res.instructions[i].first_param,
               res.instructions[i].second_param_address_type,
               res.instructions[i].second_param
        );
    }
}

void print_sec_run(AssemblerSecondRunResult assemblerSecondRunResult) {
    int i, j;
    printf("\n__code(%d) is__\n", assemblerSecondRunResult.code_len);
    for (i = 0; i < assemblerSecondRunResult.code_len; i++) {
        Data data = assemblerSecondRunResult.code[i];
        for (j = 13; j >= 0; j--) {
            printf("%c", data.code[j]);
        }
        printf("\n");
    }
}

char cast_to_special_encoding(char bin_char) {
    if (bin_char == '0')
        return '.';
    else
        return '/';
}

int write_object_file(char *file_name, Data *code, int code_len, Data *data, int data_len) {
    int i, j;
    FILE *ob_f = open_file_with_new_extention(file_name,OBJ_FILE_EXTENSION,"w+");
    if(ob_f == NULL){
       return -1;/*ERROR*/
    }
    fprintf(ob_f,"%d %d\n", code_len, data_len);
    printf("%d %d\n", code_len, data_len);
    for (i = 0; i < code_len; i++) {
        fprintf(ob_f,"%04d ", IC_OFFSET + i);
        printf("%04d ", IC_OFFSET + i);
        for (j = 13; j >= 0; j--) {
            fprintf(ob_f,"%c", cast_to_special_encoding(code[i].code[j]));
            printf("%c", cast_to_special_encoding(code[i].code[j]));
        }
        fprintf(ob_f,"\n");
        printf("\n");
    }
    for (; i < data_len + code_len; i++) {
        fprintf(ob_f,"%04d ", IC_OFFSET + i);
        printf("%04d ", IC_OFFSET + i);
        for (j = 13; j >= 0; j--) {
            fprintf(ob_f,"%c", cast_to_special_encoding(data[i - code_len].code[j]));
            printf("%c", cast_to_special_encoding(data[i - code_len].code[j]));
        }
        fprintf(ob_f,"\n");
        printf("\n");
    }
    return 0;
}

int write_symbols_files(char *file_name, Symbol *symbols, int symbols_lens) {
    FILE *ent_f, *ext_f;
    int i;
    ext_f = NULL;
    ent_f = NULL;
    for (i = 0; i < symbols_lens; i++) {
        if(symbols[i].type == External_Symbol){
            if(ext_f == NULL){
                ext_f = open_file_with_new_extention(file_name,EXT_FILE_EXTENSION,"w+");
                if(ext_f == NULL)
                    return -1; /*ERROR*/
            }
            fprintf(ext_f, "%s %d\n", symbols[i].name, symbols[i].counter);
            printf( "%s %d\n", symbols[i].name, symbols[i].counter);
        }
        if(symbols[i].type == Code_Symbol || symbols[i].type == Data_Symbol){
            if(ent_f == NULL){
                ent_f = open_file_with_new_extention(file_name,ENT_FILE_EXTENSION,"w+");
            }
            fprintf(ent_f, "%s %d\n", symbols[i].name, symbols[i].counter);
            printf( "%s %d\n", symbols[i].name, symbols[i].counter);
        }
    }

    if(ext_f != NULL)
        fclose(ext_f);
    if(ent_f != NULL)
        fclose(ent_f);
    return 0;
}
void free_all_memoeries(AssemblerFirstRunResult assemblerFirstRunResult,AssemblerSecondRunResult assemblerSecondRunResult){
    int i;
    free(assemblerFirstRunResult.instructions);
    free(assemblerFirstRunResult.datas);
    for(i=0;i<assemblerFirstRunResult.symbols_len;i++){
        free(assemblerFirstRunResult.symbols[i].name);
    }
    free(assemblerFirstRunResult.symbols);
    free(assemblerSecondRunResult.code);
}
int main(int argc, char *argv[]) {
    AssemblerFirstRunResult res;
    AssemblerSecondRunResult secondRunResult;
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

        secondRunResult = assembler_second_run(res);
        print_result(res);
        print_sec_run(secondRunResult);
        printf("\n\nfinished handling %s\n", *file_name);
        write_object_file(*file_name, secondRunResult.code, secondRunResult.code_len,
                          res.datas, res.datas_len);

        write_symbols_files(*file_name, res.symbols, res.symbols_len);
        free_all_memoeries(res, secondRunResult);
    }

    return 0;
}
