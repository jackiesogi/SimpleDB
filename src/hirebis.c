#include "hirebis.h"
#include "datastructure.h"
#include "log.h"
#include "loadsave.h"
#include "crud.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// 建立一個與 kvstore.dat 的
// Todo: 更新Connection結構
rebisContext* rebisConnect(const char* ip, int port)
{
    if ( strncmp(ip, "127.0.0.1", 9) == 0)
    {
        struct Table_Connection *tconnection = load_table_from_file(localhost, KV_TABLE_MAX_ENTRIES);     // localhost只是 "./data/kvstore.dat" 這個檔案的代稱而已 定義在 hirebis.h
        // Todo : filename 待改
        struct List_Connection  *lconnection = load_list_from_file(localhost);
        struct Connection *connection = initConnection(tconnection, lconnection);

        if (connection->tc->fd == -1)
        {
            log_message("Error: No data file was found");
            return NULL;
        }
        else
        {
            rebisContext *rebiscontext = (rebisContext*)malloc(sizeof(rebisContext));
            rebiscontext->ip = strdup("127.0.0.1");
            rebiscontext->port = port;
            rebiscontext->connection = connection;

            return rebiscontext;
        }
    }
    else
    {
        log_message("Error: Connection refused");
    }

    return NULL;
}

rebisReply* rebisCommand(rebisContext* rebiscontext, char* str)
{
    rebisReply *reply = (rebisReply*)calloc(1, sizeof(rebisReply));
    struct QueryObject *qobj = type_command(str, rebiscontext->connection);

    reply->type = 5;
    reply->str = strdup(qobj->message);

    return reply;
}

void rebisDisconnect(rebisContext* rebiscontext)
{
    save_table_to_file(rebiscontext->connection->tc);
}

void freeReplyObject(rebisReply* rebisreply)
{
    free(rebisreply);
}