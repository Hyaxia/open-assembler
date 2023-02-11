#ifndef MACRO_H_ /* Include guard */
#define MACRO_H_

#define MACRO_START "mcr"
#define MACRO_START_LEN 3
#define MACRO_END "endmcr"
#define MACRO_END_LEN 6
#define MACRO_MAX_LENGTH 80

typedef struct Macro
{
    char *name;
    char *content;
    int content_size;
} Macro;

/* a method that frees a list of Macros */
int free_macros(Macro *macros, int macros_size);

/* a mehtod that returns the relevant macro referenced in `line` if there is one */
Macro *get_macro(char *trimmed_line, char *line, Macro *macros, int macros_size, int line_len);

#endif /* MACRO_H_ */
