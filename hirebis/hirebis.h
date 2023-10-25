/*

This is a header file called "hirebis.h",
providing an interface to retrieve data from "Rebis" (A fake database simulate Redis),
which has similar function with <hiredis.h>

(A tribute to the Redis open source project)
*/

#ifndef
#define HIREBIS_H

#define localhost "../data/kvstore.dat"

typedef struct rebisContext
{
    const char* ip;
    int port;
}
rebisContext;

typedef struct rebisReply
{
    const char* str;
    const int type;
}
rebisReply;

extern rebisContext* rebisConnect(const char* ip, int port);

extern rebisReply* rebisCommand(rebisContext* instance, char* str);

#endif