#include "include/error.h"

#include <stdio.h>

void printerror_macro (const char *error_info, const char *file, int line)
{
    printf(ANSI_COLOR_RED "[%s:%d] " ANSI_COLOR_RESET "- %s ·> ", file, line, 
            error_info);
    fflush(0); 
    perror("");
}