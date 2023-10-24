#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

// #define FILENAME "./data/kvstore.dat"
#define MAX_ENTRIES 100000
#define QUERY_STRING_MAX 100
#define KEY_SIZE 32
#define VAL_SIZE 128

struct KeyValue
{
	char  key[KEY_SIZE];
	char  value[VAL_SIZE];
    struct  KeyValue *next;
};

struct KeyValue_Table
{
	int32_t  count_entries;
    int32_t  max_size;
    struct   KeyValue **records;
};

unsigned int customHash(const char *str, int max_entries) {
    unsigned int hash = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        hash = (hash * 31) + (unsigned int)str[i];
    }
    return hash % max_entries;
}

// Insert a key-value pair into the hash table
void insert(struct KeyValue_Table *table, const char *key, const char *value)
{
    int index = customHash(key, table->max_size);

    struct KeyValue *new_entry = (struct KeyValue *)malloc(sizeof(struct KeyValue));
    strcpy(new_entry->key, key);
    strcpy(new_entry->value, value);

    if ( table->records[index] == NULL )
    {
        table->count_entries++;
    }

    new_entry->next = table->records[index];
    table->records[index] = new_entry;
    
    printf("Inserted key '%s' with value '%s' at index '%d\n", key, value, index);
}

// Search for a value using a key
const char *search(struct KeyValue_Table *table, const char *key)
{
    int index = customHash(key, table->max_size);
    struct KeyValue *current = table->records[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            printf("Found key '%s' with value '%s' at index '%d'\n", key, current->value, index);
            return current->value;
        }
        current = current->next;
    }

    printf("Key '%s' not found\n", key);
    return NULL; // Key not found
}

// Function to delete a key-value pair from the hash table
void delete(struct KeyValue_Table *table, const char *key)
{
    int index = customHash(key, table->max_size);
    struct KeyValue *current = table->records[index];
    struct KeyValue *prev = NULL;

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (prev == NULL)
            {
                table->records[index] = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            printf("Deleted key '%s'\n", key);

            free(current);
            table->count_entries--;
            return;
        }

        prev = current;
        current = current->next;
    }

    printf("Key '%s' not found for deletion\n", key);
}

// Initialize the hash table
void initHashTable(struct KeyValue_Table *table, int max_size) {
    table->count_entries = 0;
    table->max_size = MAX_ENTRIES;
    table->records = (struct KeyValue **)malloc(max_size * sizeof(struct KeyValue *));
    for (int i = 0; i < max_size; i++) {
        table->records[i] = NULL;
    }
}

// Remember to clean up the table when you're done
void cleanupHashTable(struct KeyValue_Table *table)
{
    for (int i = 0; i < table->max_size; i++)
    {
        struct KeyValue *current = table->records[i];
        while (current != NULL)
        {
            struct KeyValue *next = current->next;
            free(current);
            current = next;
        }
    }

    free(table->records);
}

int main()
{
    int max_size = MAX_ENTRIES;
    struct KeyValue_Table ht;
    initHashTable(&ht, max_size);

    // Insert key-value pairs
    insert(&ht, "apple", "red");
    insert(&ht, "banana", "yellow");
    insert(&ht, "grape", "purple");

    // Search for values using keys
    const char *value = search(&ht, "banana");

    // Delete a key-value pair
    delete(&ht, "banana");

    // Cleanup and free memory
    cleanupHashTable(&ht);

    return 0;
}