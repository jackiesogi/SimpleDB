/*
This is a header file called "hirebis.h",
providing an interface to retrieve data from "Rebis" (A fake database simulate Redis),
which has similar function with <hiredis.h>

(A tribute to the Redis open source project)
*/

#ifndef HIREBIS_H
#define HIREBIS_H

#include "../include/datastructure.h"
#define localhost "./data/kvstore.dat"

typedef struct rebisContext
{
    const char* ip;
    int port;
    struct Connection* connection;
}
rebisContext;   // typedef 定義別名後不用打 struct

typedef struct rebisReply
{
    const char* str;
    int type;
}
rebisReply;

extern rebisContext* rebisConnect(const char* ip, int port);

extern rebisReply* rebisCommand(rebisContext* instance, char* str);

extern void rebisDisconnect(rebisContext* instance);

#endif