#ifndef CONFIG_H_ /* Include guard */
#define CONFIG_H_

#define MAX_LINE_LEN 81 /* length including `\0` at the end */
#define NEW_LINE '\n'
#define EOL '\0'

typedef struct Result {
    int len;
    int has_errors;
} Result;

enum RegisterCode {
    r0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7
};

extern char *register_names[8];

#endif /* CONFIG_H_ */
