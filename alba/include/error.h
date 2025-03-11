#ifndef ERROR_H
#define ERROR_H

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_PURPLE_BACKGROUND    "\e[45m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define printerror(error_info) printerror_macro(error_info, __FILE__, __LINE__)
#define printdebug(debug_info) printdebug_macro(debug_info, __FILE__, __LINE__)

void printerror_macro(const char *error_info, const char *file, int line);
void printdebug_macro(const char *debug_info, const char *file, int line);

#endif // ERROR_H