#include <string.h>
#include <stddef.h>

typedef struct
{
    int    argc;
    char** argv;
} Arg_Type;

Arg_Type split_argument(char** cmd)
{
    if(cmd == NULL)  return NULL;
}