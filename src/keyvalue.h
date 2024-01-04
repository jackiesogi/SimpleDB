#ifndef KEY_VALUE_H
#define KEY_VALUE_H

#include <stdint.h>
#define TABLE_FILE "./data/kvstore.dat"
#define KV_TABLE_MAX_ENTRIES 1000
// Key-value pair
struct KeyValue
{
	char key[32];
	char value[128];
	struct KeyValue *next;
	//struct KeyValue *prev;
};

struct KeyValue_Table
{
	int32_t count_entries;
	int32_t max_size;
	struct KeyValue records[KV_TABLE_MAX_ENTRIES];
};

struct Table_Connection
{
	const char* filename;
	int fd;
	struct KeyValue_Table* table;
};


extern struct KeyValue_Table* initKeyValueTable(int32_t max_size);

extern void freeKeyValueTable(struct KeyValue_Table *table);

#endif