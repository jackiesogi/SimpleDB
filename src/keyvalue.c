#include <stdlib.h>
#include "keyvalue.h"

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
