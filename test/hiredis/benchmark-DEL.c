#include <stdio.h>
#include <stdlib.h>
#include <hiredis/hiredis.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LINES 40000

char* data[MAX_LINES];

char** insert_test_data(const char* filepath) {
    
    FILE* file = fopen(filepath, "r");
    
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    int line_count = 0;

    while (line_count < MAX_LINES && fgets(line, sizeof(line), file))
    {
        // Remove the newline character at the end of the line
        line[strcspn(line, "\n")] = '\0';

        // Allocate memory for the string and copy the line
        data[line_count] = strdup(line);
        line_count++;
    }

    fclose(file);

    return data;
}

int main()
{
    char **command = insert_test_data("test/hirebis/test_data/test_data_for_DEL.txt");  // 和我的 hirebis 使用一樣的資料

    redisContext *c;
    c = redisConnect("127.0.0.1", 6379);
    if (c == NULL || c->err)
    {
        if (c)
        {
            printf("Error: %s\n", c->errstr);
            redisFree(c);
        }
        else
        {
            printf("Can't allocate redis context\n");
        }
        exit(1);
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < MAX_LINES; i++)
    {
        redisReply *reply = (redisReply *)redisCommand(c, command[i]);
        //printf("%s\n", reply->str);
        freeReplyObject(reply);
    }

    gettimeofday(&end, NULL);

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Deletion time: %.6f seconds\n", elapsed);

    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0)
    {
        long memory_usage = usage.ru_maxrss; // in kilobytes
        printf("Memory usage: %ld KB\n", memory_usage);
    }
    else
    {
        printf("Unable to determine memory usage\n");
    }

    redisFree(c);

    return 0;
}