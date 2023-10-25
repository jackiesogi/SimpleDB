#include "./hirebis.h"
#include "../"

rebisContext* rebisConnect(const char* ip, int)
{
    int fd;
    fd = 
    ...
    if (fd == -1)
    {
        const char* message = "Error: Connection refused";
        fprintf(stderr, "%s\n", message);
    }
}

rebisReply* rebisCommand(rebisContext* instance, char* str)
{

}