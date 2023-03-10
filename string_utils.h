#ifndef STRING_UTILS_H_ /* Include guard */
#define STRING_UTILS_H_

/* a method that trims leading and trailing spaces from `str` and puts it inside `out` */
int trim_whitespaces(char *out, int len, char *str);

/* a method that returns 1 if the word appears in the string as a standalone word */
int has_word(char *line, char *word, int word_len);

/* a method that replaces file path with a new extension and puts the result into `new_path` */
void replace_extension(char *new_path, char *original_path, char *new_extension);

/* a method that copies a string from src to dest and removes the last char from dest */
int remove_last_char(char *dest, char *src, int word_len);

/* a method that returns 1 if the entire string is spaces, otherwise 0 */
int is_all_spaces_or_newline(char *word);

/* trim leading and trailing spaces around a word */
int word_trim_spaces(char *out, char *str);

/* returns 1 if there are brackets in the string, otherwise 0 */
int includes_brackets(char *word);

#endif /* STRING_UTILS_H_ */
