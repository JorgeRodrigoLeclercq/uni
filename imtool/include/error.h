#ifndef ERROR_H
#define ERROR_H

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define print_error(error_info) print_error_macro(error_info, __FILE__, __LINE__)

void print_error_macro(const char *error_info, const char *file, int line);

#endif // ERROR_H