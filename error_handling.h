#ifndef ERROR_HANDLING_H_ /* Include guard */
#define ERROR_HANDLING_H_

/* a method that writes an error log to stderr */
void log_error(char *message, char *file_path, int line_num);

#endif /* ERROR_HANDLING_H_ */
