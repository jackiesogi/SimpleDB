#include "log.h"
#include "cli.h"
#include <stdio.h>
#include <string.h>

void log_message(const char* message)
{
    printf(ANSI_COLOR_RESET);
    if(strncmp(message, "", 2) == 0)
    {
      printf("%s", message);
    }
    else
    {
      printf("%s\n", message);
    }
}

void log_error(const char* message)
{
    printf(ANSI_COLOR_RED);
    fprintf(stderr, "%s\n", message);
    printf(ANSI_COLOR_RESET);
}
