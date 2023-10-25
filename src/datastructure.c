#include "../include/datastructure.h"
#include <stdio.h>
#include <stdlib.h>

struct KeyValue_Table* initKeyValueTable()
{
	struct KeyValue_Table *table = NULL;
	if ( table )
	{
		printf("table does not need to initialize!\n");
		return NULL;
	}
	table = (struct KeyValue_Table*)malloc(sizeof(struct KeyValue_Table));
	table->records = (struct KeyValue **)malloc(sizeof(struct KeyValue*));
	for(int i = 0; i < MAX_ENTRIES; i++)
	{
		table->records[i] = (struct KeyValue *)malloc(sizeof(struct KeyValue));
	}
	table->max_size = 50;
	table->count_entries = 0;

	return table;
}

void freeKeyValueTable(struct KeyValue_Table *table)
{
	free(table);
}
