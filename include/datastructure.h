#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <stdint.h>
#define TABLE_FILE "./data/kvstore.dat"
#define LIST_FILE  "./data/kvlist.dat"
#define MAX_ENTRIES 1000
#define QUERY_STRING_MAX 100
#define KEY_SIZE 32
#define VAL_SIZE 128

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
	struct KeyValue records[MAX_ENTRIES];
};

struct Table_Connection
{
	const char* filename;
	int fd;
	struct KeyValue_Table* table;
};

// List
struct Node
{
	char value[128];
	struct Node *next;
	struct Node *prev;
};

struct Node_List
{
	char name[32];
	struct Node *head;
	struct Node *tail;
};

// Todo : datastructure need consideration
struct List_Connection
{
	const char* filename;
	int fd;

	int count_entries;
	//struct Node_List *list;
	struct Node_List list [MAX_ENTRIES];
};

struct SerializedListConnection
{
	char *vaueTable[MAX_ENTRIES][MAX_ENTRIES];
};

struct Connection
{
	struct Table_Connection *tc;
	struct List_Connection  *lc;
};

// QueryObject
struct QueryObject
{
	const char *query_string;
	int status_code;
	const char *key;
	const char *val;
	const char *message;
};

extern struct KeyValue_Table* initKeyValueTable(int32_t max_size);

struct List_Connection* initNodeListConnection();

extern void freeKeyValueTable(struct KeyValue_Table *table);

void freeKeyValueList(struct Node_List *list);

// Todo : initialize a config table with given database path
extern void initConfigTable(const char* dbPath);

extern struct Connection* initConnection(struct Table_Connection*, struct List_Connection*);
#endif