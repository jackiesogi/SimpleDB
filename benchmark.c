#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/hirebis.h"

void insert_test_data(const char* filepath, char* , char delim) {
    
}

int main()
{
	rebisContext *connect = rebisConnect("127.0.0.1", 8888);
	
    char* command[10] = {
        "get name",
        "set name",
        "set aaa",
        "get vvv",
        "get aaa",
        "get aaa",
        "get aaa",
        "set vvv c",
        "get vvv",
        "help"
    };

    for (int i = 0; i < 10; i++)
    {    
        rebisReply *reply = rebisCommand(connect, command[i]);

        // printf("%ld\n", strlen(reply->str));
        printf("%s\n", reply->str);

        free(reply);
    }

    rebisDisconnect(connect);

    free(connect);    
    return 0;
}