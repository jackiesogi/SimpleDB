#include "../include/loadsave.h"
#include "../include/datastructure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

struct Connection* load_data_from_file(const char* filename)
{
    //loadData
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    struct KeyValue_Table* table = initKeyValueTable();

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

    struct Connection *newConnection = (struct Connection*)malloc(sizeof(struct Connection));
    newConnection->filename = filename;
    newConnection->fd = fd;
    newConnection->table = table;

    return newConnection;
}

void save_data_to_file(const struct Connection* connection)
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
    freeKeyValueTable(connection->table);
    close(connection->fd);
    free(connection);
}

// void leave_program(struct Connection* connection)
// {
//     freeKeyValueTable(connection->table);
//     close(connection->fd);
//     free(connection);
// }