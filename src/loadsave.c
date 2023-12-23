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
#include <assert.h>

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

// struct List_Connection* load_list_from_file(const char *filename)
// {
//     int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

//     if (fd == -1)
//     {
//         perror("open");
//     }
    
//     struct List_Connection *newConnection = (struct List_Connection *)malloc(sizeof(struct List_Connection));
//     if (newConnection == NULL) {
//         perror("malloc");
//         return NULL;
//     }

//     // Initialize the fields
//     newConnection->filename = filename;
//     newConnection->fd = fd;
//     newConnection->count_entries = 0; // Initialize count_entries

//     // Read the data from the file and populate the list array
//     ssize_t bytes_read = read(fd, newConnection->list, sizeof(newConnection->list));
//     if (bytes_read == -1) {
//         perror("read");
//         free(newConnection);
//         return NULL;
//     }

//     // Calculate the number of entries based on the bytes read
//     newConnection->count_entries = bytes_read / sizeof(struct Node_List);

//     return newConnection;
// }

// void save_list_to_file(const struct List_Connection *connection)
// {
//     if (lseek(connection->fd, 0, SEEK_SET) == -1)
//     {
//         perror("lseek");
//         close(connection->fd);
//         exit(1);
//     }

//     for (int i = 0; i < connection->count_entries; i++)
//     {
//         if (write(connection->fd, connection->list, sizeof((connection->list[i]))) == -1)
//         {
//             perror("write");
//         }
//     }
// }


// void save_list_to_file(const struct List_Connection *lconnection)
// {
//     struct SerializedListConnection *slc = (struct SerializedListConnection*)calloc(1, sizeof(struct SerializedListConnection));

//     for (int i = 0; i < MAX_ENTRIES; i++)
//     {
//         int element = 0;
//         for (struct Node *current = lconnection->list[i].head; current != NULL; current = current->next)
//         {
//             slc->vaueTable[i][element++] = current->value;
//             if (write(lconnection->fd, current, sizeof(struct Node)) == -1)
//             {
//                     perror("write");
//             }
//         }
//     }   
// }

// Function to save a List_Connection to a file
void save_list_to_file(const struct List_Connection *connection) {
    if (lseek(connection->fd, 0, SEEK_SET) == -1) {
        perror("lseek");
        close(connection->fd);
        exit(1);
    }

    // Save the count_entries attribute
    if (write(connection->fd, &connection->count_entries, sizeof(int)) == -1) {
        perror("write count_entries");
        close(connection->fd);
        exit(1);
    }

    // Save the data for each Node in the list
    for (int i = 0; i < connection->count_entries; i++)
    {
        int elements_written = 0;
        const struct Node_List *list = &(connection->list[i]);
        
        // 寫入list的名稱 (32 Bytes)
        write(connection->fd, &(connection->list[i].name), sizeof(connection->list[i].name));
        // 寫入list的長度(4 Bytes)
        write(connection->fd, &(connection->list[i].length), sizeof(connection->list[i].length));
        
        for (struct Node *current = list->head; current != NULL; current = current->next)
        {
            if (write(connection->fd, current->value, sizeof(current->value)) == -1)
            {
                perror("write value");
                close(connection->fd);
                exit(1);
            }
            elements_written++;
        }
        
        // 如果寫入的元素數不等於list上紀錄的length 報錯
        assert(elements_written == connection->list[i].length);
    }

    // Ensure that the file is large enough to contain the saved data
    off_t fileSize = lseek(connection->fd, 0, SEEK_CUR);
    if (fileSize == -1) {
        perror("lseek");
        close(connection->fd);
        exit(1);
    }
}


// Function to load a List_Connection from a file
struct List_Connection *load_list_from_file(const char *filename) {
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    if (fd == -1) {
        perror("open");
        return NULL;
    }

    struct List_Connection *newConnection = (struct List_Connection *)malloc(sizeof(struct List_Connection));
    if (newConnection == NULL) {
        perror("malloc");
        return NULL;
    }

    // Initialize the fields
    // 需要想想這個initialization要放在這邊還是initNodeList()
    newConnection->filename = filename;
    newConnection->fd = fd;
    newConnection->count_entries = 0; // Initialize count_entries

    // Check the file size
    off_t fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == -1)
    {
        perror("lseek");
        free(newConnection);
        close(fd);
        return NULL;
    }

    if (fileSize >= (off_t)sizeof(int)) {
        // Read the count_entries attribute
        if (lseek(fd, 0, SEEK_SET) == -1) {
            perror("lseek");
            free(newConnection);
            close(fd);
            return NULL;
        }

        if (read(fd, &newConnection->count_entries, sizeof(int)) == -1) {
            perror("read count_entries");
            free(newConnection);
            close(fd);
            return NULL;
        }

        // Read and load the data for each Node in the list
        for (int i = 0; i < newConnection->count_entries; i++) {
            struct Node_List *list = &(newConnection->list[i]);
            list->head = NULL;
            list->tail = NULL;

            char listname[32];
            int element_read = 0;
            int length = 0;

            // 讀list的name
            read(fd, &listname, sizeof(listname));
            strncpy(newConnection->list[i].name, listname, 32);
            // 讀list的length
            read(fd, &length, sizeof(length));
            newConnection->list[i].length = length;

            while (1)
            {    
                if (element_read == length)
                {
                    break;
                }

                char value[128];
                if (read(fd, value, sizeof(value)) == -1)
                {
                    perror("read value");
                    free(newConnection);
                    close(fd);
                    return NULL;
                }

                struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));

                strncpy(newNode->value, value, sizeof(value));
                newNode->next = NULL;
                newNode->prev = NULL;

                if (list->tail == NULL)
                {
                    list->head = newNode;
                    list->tail = newNode;
                }
                else
                {
                    // 把新讀進來的value放到tail
                    list->tail->next = newNode;
                    newNode->prev = list->tail;
                    list->tail = newNode;
                }

                element_read++;
            }
        }
    }
    // 到檔案結束的offset沒有大於一個integer(照理說就是檔案是空的)
    else { }

    return newConnection;
}