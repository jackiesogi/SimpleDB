#include "../include/datastructure.h"
#include "../include/log.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct KeyValue_Table* initKeyValueTable(int32_t max_size)
{
	struct KeyValue_Table *table = NULL;

	if ( table )
	{
		//printf("table does not need to initialize!\n");
		log_message("table does not need to initialize!");
		return NULL;
	}
	
	table = (struct KeyValue_Table*)calloc(1, sizeof(struct KeyValue_Table));

	table->max_size = max_size;
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