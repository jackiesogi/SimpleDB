#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
//#include "../include/crud.h"

#define FILENAME "./data/kvstore.dat"
#define MAX_ENTRIES 100
#define QUERY_STRING_MAX 100
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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

/************************************************************************/
/************************************************************************/  
/**  This function store a key-value pair into a given table           **/
/**                                                                    **/
/**  Parameters:                                                       **/
/**    - table   : The target table you want to store key and value.   **/
/**    - key     : The key you want to store.                          **/
/**    - value   : The value you want to store.                        **/
/**                                                                    **/
/**  Returns:                                                          **/
/**    - if succeeded it will return true.                             **/
/**    - if failed it will return false.                               **/
/************************************************************************/
/************************************************************************/
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

/*************************************************************************/
/*************************************************************************/  
/**  This function retrieve a value from given table by a given key.    **/
/**                                                                     **/
/**  Parameters:                                                        **/
/**    - table      : The target table you want to retrieve value from. **/
/**    - search_key : The key of the value you want to retrieve.        **/
/**                                                                     **/
/**  Returns:                                                           **/
/**    - if succeeded it will return the value in string.               **/
/**    - if failed it will return NULL.                                 **/
/*************************************************************************/
/*************************************************************************/
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

/*************************************************************************/
/*************************************************************************/  
/**  This function delete the value from given table by a given key.    **/
/**                                                                     **/
/**  Parameters:                                                        **/
/**    - table      : The target table you want to delete value from.   **/
/**    - search_key : The key of the value you want to delete.          **/
/**                                                                     **/
/**  Returns:                                                           **/
/**    (no return value)                                                **/
/*************************************************************************/
/*************************************************************************/
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


/********************************/
/** Entry point of the program **/
/********************************/
int main()
{
	printf(ANSI_COLOR_CYAN "###################################################### \n");
	printf(ANSI_COLOR_CYAN "###################################################### \n");
	printf(ANSI_COLOR_CYAN "##                                                  ## \n");
	printf(ANSI_COLOR_CYAN "##              SimpleDB by jackiesogi              ## \n");
	printf(ANSI_COLOR_CYAN "##    (Sorry, it's not listening on any port :D)    ## \n");
	printf(ANSI_COLOR_CYAN "##                                                  ## \n");
	printf(ANSI_COLOR_CYAN "###################################################### \n");
	printf(ANSI_COLOR_CYAN "###################################################### \n");
	printf(ANSI_COLOR_RESET);

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
	

	// the maximum length of query_string is 99 characters plus a terminator
	char query_string[QUERY_STRING_MAX];

	while (1)
	{
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
			printf("[Error] Command \"%s\" not found.\n", query_string);
		}
	}

	// Unmap and close the file.
	munmap(table, sizeof(struct KeyValue_Table));
	close(fd);

	return 0;
}
