#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../include/crud.h"

/* Source file of crud.h */

bool store_value_by_key(struct KeyValue_Table *table, const char *key, const char *value)
{
	if (table->count_entries >= MAX_ENTRIES)
	{
		puts("[Error] Maximum number of entries reached.");
		return false;
	}
	
	strcpy(table->records[table->count_entries].key, key);
	strcpy(table->records[table->count_entries].value, value);
	(table->count_entries) ++;
	puts("OK");
	return true;
}

char *retrieve_value_by_key(struct KeyValue_Table *table, const char *search_key)
{
	// Currently it is a linear search, hash function to be added.
	int size = table->count_entries;
	for (int i = 0; i < size; i++)
	{
		if (strcmp(table->records[i].key, search_key) == 0)
		{
			printf("\"%s\"\n", table->records[i].value);
			return table->records[i].value;
		}
	}

	puts("(nil)");
	return NULL;
}

void delete_value_by_key(struct KeyValue_Table *table, const char *search_key)
{
	int size = table->count_entries;
	for(int i = 0; i < size; i++)
	{
		if (strcmp(table->records[i].key, search_key) == 0)
		{
			table->records[i].value[0] = '\0';
			table->records[i].key[0] = '\0';
			puts("OK");
			return;
		}
	}
	puts("(nil)");
}