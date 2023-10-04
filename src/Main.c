#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
//#include "../include/crud.h"

#define FILENAME "./data/recordsstore.dat"
#define MAX_ENTRIES 100
#define QUERY_STRING_MAX 100

struct KeyValue
{
	char key[32];
	char value[128];
};

struct KeyValue_Table
{
	int count_entries;
	struct KeyValue records[MAX_ENTRIES];
};

void store_key_value(struct KeyValue_Table *table, const char *key, const char *value)
{
	if (table->count_entries >= MAX_ENTRIES)
	{
		puts("[Error] Maximum number of entries reached.");
		return;
	}
	
	strcpy(table->records[table->count_entries].key, key);
	strcpy(table->records[table->count_entries].value, value);
	(table->count_entries) ++;
	puts("OK");
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

int main()
{
	puts("######################################################");
	puts("######################################################");
	puts("##                                                  ##");
	puts("##              SimpleDB by jackiesogi              ##");
	puts("##    (Sorry, it's not listening on any port :D)    ##");
	puts("##                                                  ##");
	puts("######################################################");
	puts("######################################################");

	int fd;
	struct KeyValue_Table *table;

	fd = open(FILENAME, O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		// File doesn't exist, create it.
		fd = open(FILENAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		if (fd == -1)
		{
			perror("open");
			exit(1);
		}

		// Initialize the file with zeros
		ftruncate(fd, sizeof(struct KeyValue_Table));
		
	}

	// Map the file into memory.
	table = (struct KeyValue_Table *) mmap(NULL, sizeof(struct KeyValue_Table), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (table == MAP_FAILED)
	{
		perror("mmap");
		exit(1);
	}
	

	// Assuming the maximum length of query_string is 99 characters plus null terminator
	char query_string[QUERY_STRING_MAX];

	while (1)
	{
		// Infinite loop
		printf("127.0.0.1:6379 > ");
		fgets(query_string, sizeof(query_string), stdin);

		// Remove newline character added by fgets
		size_t len = strlen(query_string);
		if (len > 0 && query_string[len - 1] == '\n')
		{
			query_string[len - 1] = '\0';
		}

		if (strcmp(query_string, "exit") == 0)
		{
			puts("Bye!");
			break;	// Exit the loop
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
				store_key_value(table, key, value);
			}
			else
			{
				puts("[Usage] set <key> <value>");
			}
		}
		else if (strncmp(query_string, "get ", 4) == 0 || strncmp(query_string, "GET ", 4) == 0)
		{
			char *key = query_string + 4;	// Move the pointer past "get "
			if (strcmp(key, " ") == 0)
			{
				puts("[Usage] get <key>");
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
				puts("[Usage] del <key>");
			}
			else
			{
				delete_value_by_key(table, key);
			}
		}
		else
		{
			printf("[Error] Command %s\n not found.\n", query_string);
		}
	}

	// Unmap and close the file.
	munmap(table, sizeof(struct KeyValue_Table));
	close(fd);

	return 0;
}
