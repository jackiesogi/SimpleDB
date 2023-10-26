#include "../include/log.h"
#include <stdio.h>

void logError(const char* message)
{
    fprintf(stderr, "%s\n", message);
}
