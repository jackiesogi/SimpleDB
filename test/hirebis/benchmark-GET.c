#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "../include/hirebis.h"

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
    rebisContext *connect = rebisConnect("127.0.0.1", 8888);
    
    // 載入SET測試資料
    char **command = insert_test_data("test/hirebis/test_data/test_data_for_GET.txt");
    
    // 獲取起始時間
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    for (int j = 0; j < MAX_LINES; j++)
    {   
        // strdup() 會讓 string literal 變得可以修改（待研究）
        rebisReply *reply = rebisCommand(connect, strdup(command[j]));
        //printf("%s\n", reply->str);
        free(reply);
    }
    
    // 获取结束时间
    gettimeofday(&end, NULL);
    
    // 算 Runtime
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Retrieval time: %.6f seconds\n", elapsed);
    
    // 記憶體用量
    struct rusage usage;
    
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        long memory_usage = usage.ru_maxrss; // in kilobytes
        printf("Memory usage: %ld KB\n", memory_usage);
    } else {
        printf("Unable to determine memory usage\n");
    }
    
    rebisDisconnect(connect);

    free(connect);

    return 0;
}