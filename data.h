#ifndef DATA_H_ /* Include guard */
#define DATA_H_

#define WORD_LEN 14

typedef struct {
    int code[14];
} Data;

/* receives the relevant data and stores it into a Data object in a convenient way */
Result store_dot_data(Data *data, char *content, char *no_macro_file_path, int line_num);

/* receives the relevant data and stores it into a Data object in a convenient way */
Result store_dot_string(Data *data, char *content, char *no_macro_file_path, int line_num);

#endif /* DATA_H_ */
