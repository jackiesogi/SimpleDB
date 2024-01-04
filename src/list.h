#ifndef LIST_H
#define LIST_H

#define LIST_FILE  "./data/kvlist.dat"
#define LIST_MAX_ENTRIES 1000
// List data structure
struct Node
{
	char value[128];
	struct Node *next;
	struct Node *prev;
};

struct Node_List
{
	char name[32];
	int length;
	struct Node *head;
	struct Node *tail;
};


struct List_Connection
{
	const char* filename;
	int fd;

	int count_entries;
	//struct Node_List *list;
	struct Node_List list [LIST_MAX_ENTRIES];
};

struct List_Connection* initNodeListConnection();

#endif