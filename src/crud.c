/* Source file of crud.h */
#include "../include/datastructure.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

unsigned int hash_function(const char *str, int max_entries)
{
    unsigned int hash = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        hash = (hash * 31) + (unsigned int)str[i];
    }
    return hash % max_entries;
}

void store_value_by_key(struct KeyValue_Table *table, const char *search_key, const char *value)
{
    unsigned int index = hash_function(search_key, table->max_size);
    struct KeyValue *new_entry = (struct KeyValue *) malloc (sizeof(struct KeyValue));
    strcpy(new_entry->key, search_key);
    strcpy(new_entry->value, value);

    //if ( table->records[index] == NULL )
    //{
        table->count_entries++;
    //}

    new_entry->next = table->records[index];
    table->records[index] = new_entry;
    
    printf("Inserted key '%s' with value '%s' at index '%d\n", search_key, value, index);
}

const char *retrieve_value_by_key(struct KeyValue_Table *table, const char *search_key)
{
	unsigned int index = hash_function(search_key, table->max_size);
    struct KeyValue *current = table->records[index];

    while (current != NULL) {
        if (strcmp(current->key, search_key) == 0) {
            printf("Found key '%s' with value '%s' at index '%d'\n", search_key, current->value, index);
            return current->value;
        }
        current = current->next;
    }

    printf("Key '%s' not found\n", search_key);
    return NULL; // Key not found
}

void delete_value_by_key(struct KeyValue_Table *table, const char *search_key)
{
	unsigned int size = table->count_entries;
	for(unsigned int i = 0; i < size; i++)
	{
		if (strcmp(table->records[i]->key, search_key) == 0)
		{
			table->records[i]->value[0] = '\0';
			table->records[i]->key[0] = '\0';
			puts("OK");
			return;
		}
	}
	puts("(nil)");
}