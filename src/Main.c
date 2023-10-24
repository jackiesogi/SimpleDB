#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "../include/crud.h"
#include "../include/cli.h"
#include "../include/datastructure.h"


/********************************/
/** Entry point of the program **/
/********************************/
int main()
{
	printWelcomePage();

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

	return 0;
}
