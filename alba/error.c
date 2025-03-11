#include <stdio.h>

#include "include/error.h"

void printerror_macro (const char *error_info, const char *file, int line)
{
    printf(ANSI_COLOR_RED "[%s:%d] " ANSI_COLOR_RESET "- %s ·> ", file, line, 
            error_info);
    fflush(0); 
    perror("");
}

void printdebug_macro(const char *debug_info, const char *file, int line)
{
    printf(ANSI_COLOR_PURPLE_BACKGROUND "[debug]:" ANSI_COLOR_RESET 
            " [%s:%d] - %s\n", file, line, debug_info);
}