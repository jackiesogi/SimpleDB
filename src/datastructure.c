#include "datastructure.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// struct KeyValue_Table* initKeyValueTable(int32_t max_size)
// {
// 	struct KeyValue_Table *table = NULL;

// 	if ( table )
// 	{
// 		//printf("table does not need to initialize!\n");
// 		log_message("table does not need to initialize!");
// 		return NULL;
// 	}
	
// 	table = (struct KeyValue_Table*)calloc(1, sizeof(struct KeyValue_Table));

// 	table->max_size = max_size;
// 	table->count_entries = 0;

// 	return table;
// }

struct KeyValue_Table* initKeyValueTable(int32_t max_size)
{
	struct KeyValue_Table *table = (struct KeyValue_Table*)malloc(sizeof(struct KeyValue_Table));
	table->max_size = max_size;
	table->count_entries = 0;
	// Note : records[] not initialized.
	return table;
}

void freeKeyValueTable(struct KeyValue_Table *table)
{
	if (table != NULL)
	{
    	free(table);
	}
}

// Todo : 改善 bad naming 和 refactor Node_List 結構
struct List_Connection* initNodeListConnection()
{
	struct List_Connection *list = (struct List_Connection*)calloc(1, sizeof(struct List_Connection));
	return list;
}

void freeKeyValueList(struct Node_List *list)
{
	if (list != NULL)
	{
    	free(list);
	}
}

struct Connection* initConnection(struct Table_Connection *tconnection, struct List_Connection *lconnection)
{
	struct Connection *connection = (struct Connection*)malloc(sizeof(struct Connection));
	connection->tc = tconnection;
	connection->lc = lconnection;
	return connection;
}