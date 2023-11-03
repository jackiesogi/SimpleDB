#include "../include/loadsave.h"
#include "../include/datastructure.h"
#include "../include/log.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

struct Table_Connection* load_table_from_file(const char* filename, int32_t table_size)
{
    // 打開 data/kvstore.dat (主要資料儲存的檔案)
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    // 準備一張hash table
    struct KeyValue_Table* table = initKeyValueTable(table_size);

    if (fd == -1)
    {
        perror("open");
        exit(1);
    }

	// 確保有足夠空間
    if (lseek(fd, sizeof(*table) - 1, SEEK_SET) == -1)
    {
        perror("lseek");
        close(fd);
        exit(1);
    }

    if (write(fd, "", 1) == -1)
    {
        perror("write");
        close(fd);
        exit(1);
    }

    // 把cursor的offset設回0
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        perror("lseek");
        close(fd);
        exit(1);
    }

	if (read(fd, table, sizeof(*table)) == -1)
    {
        perror("read");
        close(fd);
        exit(1);
    }

    // Note : 這個檢查治標不治本 待想替代方案
    // 如果 kvstore.dat 裡面沒有任何紀錄 再初始化一次
    if ( table->count_entries == 0 )
    {
        table->max_size = table_size;
    }
    else if ( table->max_size > MAX_ENTRIES )
    {
        log_message("[Error] Table size has been adjusted.\n");
        exit(1);
    }

    struct Table_Connection *newConnection = (struct Table_Connection*)malloc(sizeof(struct Table_Connection));
    newConnection->filename = filename;
    newConnection->fd = fd;
    newConnection->table = table;

    return newConnection;
}

void save_table_to_file(const struct Table_Connection* connection)
{
    if (lseek(connection->fd, 0, SEEK_SET) == -1)
    {
        perror("lseek");
        close(connection->fd);
        exit(1);
    }

    if (write(connection->fd, connection->table, sizeof((*connection->table))) == -1)
    {
        perror("write");
    }
}

void free_connection(struct Connection *connection)
{
    freeKeyValueTable(connection->tc->table);
    //freeKeyValueList(connection->lc->list);
    close(connection->tc->fd);
    close(connection->lc->fd);
    free(connection);
}

struct List_Connection* load_list_from_file(const char *filename)
{
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    if (fd == -1)
    {
        perror("open");
    }
    
    struct List_Connection *newConnection = (struct List_Connection *)malloc(sizeof(struct List_Connection));
    if (newConnection == NULL) {
        perror("malloc");
        return NULL;
    }

    // Initialize the fields
    newConnection->filename = filename;
    newConnection->fd = fd;
    newConnection->count_entries = 0; // Initialize count_entries

    // Read the data from the file and populate the list array
    ssize_t bytes_read = read(fd, newConnection->list, sizeof(newConnection->list));
    if (bytes_read == -1) {
        perror("read");
        free(newConnection);
        return NULL;
    }

    // Calculate the number of entries based on the bytes read
    newConnection->count_entries = bytes_read / sizeof(struct Node_List);

    return newConnection;
}

void save_list_to_file(const struct List_Connection *connection)
{
    if (lseek(connection->fd, 0, SEEK_SET) == -1)
    {
        perror("lseek");
        close(connection->fd);
        exit(1);
    }

    for (int i = 0; i < connection->count_entries; i++)
    {
        if (write(connection->fd, connection->list, sizeof((connection->list[i]))) == -1)
        {
            perror("write");
        }
    }
}