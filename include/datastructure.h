#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <stdint.h>
#define FILENAME "./data/kvstore.dat"
#define MAX_ENTRIES 100000
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

struct Connection
{
	const char* filename;
	int fd;
	struct KeyValue_Table* table;	
};

struct QueryObject
{
	int status_code;
	const char *message;
};

extern struct KeyValue_Table* initKeyValueTable();

extern void freeKeyValueTable(struct KeyValue_Table *table);

#endif