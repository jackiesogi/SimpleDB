#include "../include/hirebis.h"
#include "../include/datastructure.h"
#include "../include/log.h"
#include "../include/loadsave.h"
#include "../include/crud.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

rebisContext* rebisConnect(const char* ip, int port)
{
    if ( strncmp(ip, "127.0.0.1", 9) == 0)
    {
        struct Connection *connection = load_data_from_file(localhost);     // localhost只是 "../data/kvstore.dat" 這個檔案的代稱而已

        if (connection->fd == -1)
        {
            const char* message = "Error: Connection refused";
            fprintf(stderr, "%s\n", message);

            return NULL;
        }
        else
        {
            rebisContext *rebiscontext = (rebisContext*)malloc(sizeof(rebisContext));
            rebiscontext->ip = "127.0.0.1";
            rebiscontext->port = port;
            rebiscontext->connection = connection;

            return rebiscontext;
        }
    }
    else
    {
        logError("Error: Connection refused");
        return NULL;
    }
    
}

rebisReply* rebisCommand(rebisContext* rebiscontext, char* str)
{
    rebisReply *reply = (rebisReply*)calloc(1, sizeof(rebisReply));
    struct QueryObject *qobj = type_command(str, rebiscontext->connection);

    reply->type = 5;
    reply->str = qobj->result;

    return reply;
}