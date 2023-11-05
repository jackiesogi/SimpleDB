#include "../include/log.h"
#include "../include/cli.h"
#include <stdio.h>

void log_message(const char* message)
{
    printf(ANSI_COLOR_RESET);
    printf("%s\n", message);
}

void log_error(const char* message)
{
    printf(ANSI_COLOR_RED);
    printf("%s\n", message);
    printf(ANSI_COLOR_RESET);
}