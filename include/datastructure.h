#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <stdint.h>
#define FILENAME "./data/kvstore.dat"
#define MAX_ENTRIES 50
#define QUERY_STRING_MAX 100
#define KEY_SIZE 32
#define VAL_SIZE 128

struct KeyValue
{
	char key[32];
	char value[128];
	struct KeyValue *next;
};

struct KeyValue_Table
{
	int32_t count_entries;
	int32_t max_size;
	struct KeyValue records[MAX_ENTRIES];
};

extern struct KeyValue_Table* initKeyValueTable();

extern void freeKeyValueTable(struct KeyValue_Table *table);

// #include "../src/datastructure.c"

#endif

// void loadHashTable(struct KeyValue_Table *table)
// {
//     int fd = open(FILENAME, O_RDWR, S_IRUSR | S_IWUSR);
// 	if (fd == -1)
// 	{
// 		// File doesn't exist, create it.
// 		fd = open(FILENAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
// 		if (fd == -1)
// 		{
// 			perror("open");
// 			exit(1);
// 		}

// 		// Initialize the file with zeros
// 		ftruncate(fd, sizeof(struct KeyValue_Table));
// 	}

//     // Map the file into memory.
//     table = (struct KeyValue_Table *) mmap(NULL, sizeof(struct KeyValue_Table), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//     if (table == MAP_FAILED)
//     {
//         perror("mmap");
//         exit(1);
//     }

//     // initHashTable(table, MAX_ENTRIES);

// }

// void saveHashTable(struct KeyValue_Table *table)
// {

// }