#ifndef PRE_ASSEMBLER_H_ /* Include guard */
#define PRE_ASSEMBLER_H_

#define PRE_ASSEMBLER_OUTPUT_EXTENSION ".am"

/* performs pre assembler logic - replaces macros with definitions and outputs file without macros
   returns 0 if there are no erros in the pre assmbler stage, otherwise 1
 */
int pre_assembler(char *file_path);

#endif /* PRE_ASSEMBLER_H_ */
