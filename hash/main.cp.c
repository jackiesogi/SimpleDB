#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
// #include "../include/crud.h"
#include "../include/cli.h"
// #include "../include/datastructure.h"

#define FILENAME "./kvstore.dat"
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
void store_value_by_key(struct KeyValue_Table *table, const char *key, const char *value)
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
    
    printf("Inserted key '%s' with value '%s' at index #%d\n", key, value, index);
}

// Search for a value using a key
const char *retrieve_value_by_key(struct KeyValue_Table *table, const char *key)
{
    //int index = 1;
    int index = customHash(key, table->max_size);
    struct KeyValue *current = table->records[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            printf("Found key '%s' with value '%s' at index #%d\n", key, current->value, index);
            return current->value;
        }
        current = current->next;
    }

    printf("Key '%s' not found\n", key);
    return NULL; // Key not found
}


// Function to delete a key-value pair from the hash table
void delete_value_by_key(struct KeyValue_Table *table, const char *key)
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
    table->max_size = max_size;
    table->records = (struct KeyValue **)malloc(max_size * sizeof(struct KeyValue *));
    for (int i = 0; i < max_size; i++) {
        table->records[i] = (struct KeyValue *)malloc(sizeof(struct KeyValue));
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

    printWelcomePage();

	int fd;
	struct KeyValue_Table *table;

     // Check if the kvstore.dat file exists
    if (access(FILENAME, F_OK) != -1)
    {
        // File exists, load the data from the file
        fd = open(FILENAME, O_RDWR, S_IRUSR | S_IWUSR);
        if (fd == -1)
        {
            perror("open");
            exit(1);
        }

        // Map the file into memory.
        table = (struct KeyValue_Table *)mmap(NULL, sizeof(struct KeyValue_Table), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (table == MAP_FAILED)
        {
            perror("mmap");
            exit(1);
        }
    }
    else
    {
        // File doesn't exist, create it and initialize a new KeyValue_Table
        fd = open(FILENAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        if (fd == -1)
        {
            perror("open");
            exit(1);
        }

        // Initialize the file with zeros
        ftruncate(fd, sizeof(struct KeyValue_Table));

        // Map the file into memory.
        table = (struct KeyValue_Table *)mmap(NULL, sizeof(struct KeyValue_Table), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (table == MAP_FAILED)
        {
            perror("mmap");
            exit(1);
        }

        // Initialize a new KeyValue_Table
        table->max_size = MAX_ENTRIES;
        initHashTable(table, MAX_ENTRIES);
    }
	
	// the maximum length of query_string is 99 characters plus a terminator
	char query_string[QUERY_STRING_MAX];

	while (1)
	{
		printNewLine();
		fgets(query_string, sizeof(query_string), stdin);

		// Remove newline character added by fgets
		size_t len = strlen(query_string);
		if (len > 0 && query_string[len - 1] == '\n')
		{
			query_string[len - 1] = '\0';
		}

		if (strcmp(query_string, "exit") == 0 || strcmp(query_string, "EXIT") == 0)
		{
			printGoodBye();
			break;	// Exit the command listening loop
		}
		else if (strcmp(query_string, "\0") == 0)
		{
			continue;
		}
		else if (strncmp(query_string, "set ", 4) == 0 || strncmp(query_string, "SET ", 4) == 0)
		{
			char *args = query_string + 4;	// Move the pointer past "set "
			char *key = strtok(args, " ");
			char *value = strtok(NULL, " ");
			if (key != NULL && value != NULL)
			{
				store_value_by_key(table, key, value);
			}
			else
			{
				printSetUsage();
			}
		}
		else if (strncmp(query_string, "get ", 4) == 0 || strncmp(query_string, "GET ", 4) == 0)
		{
			char *key = query_string + 4;	// Move the pointer past "get "
			if (strcmp(key, " ") == 0)
			{
				printGetUsage();
			}
			else
			{
				retrieve_value_by_key(table, key);
			}
		}
		else if (strncmp(query_string, "del ", 4) == 0 || strncmp(query_string, "DEL ", 4) == 0)
		{
			char *key = query_string + 4;	// Move the pointer past "get "
			if (strcmp(key, " ") == 0)
			{
				printDelUsage();
			}
			else
			{
				delete_value_by_key(table, key);
			}
		}
		else if (strncmp(query_string, "help", 4) == 0 || strncmp(query_string, "HELP", 4) == 0)
		{
			printHelpPage();

		}
		else
		{
			printCommandNotFound(query_string);
		}
	}

	// Unmap and close the file.
	munmap(table, sizeof(struct KeyValue_Table));
	close(fd);
    // Cleanup and free memory
    //cleanupHashTable(table);
    
    return 0;
}