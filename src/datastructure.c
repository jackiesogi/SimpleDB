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
	table = (struct KeyValue_Table*)calloc(1, sizeof(struct KeyValue_Table));

	table->max_size = 50;
	table->count_entries = 0;

	return table;
}

void freeKeyValueTable(struct KeyValue_Table *table)
{
	if (table != NULL)
	{
    	free(table);
	}

}
