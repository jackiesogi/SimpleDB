#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "../include/datastructure.h"
#include "../include/crud.h"
#include "../include/cli.h"


/********************************/
/** Entry point of the program **/
/********************************/
int main(int argc, char *argv[])
{
	printWelcomePage();

	int fd;
	struct KeyValue_Table *table = NULL;

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

		table = initKeyValueTable();
		// Initialize the file with zeros
		ftruncate(fd, sizeof(struct KeyValue_Table));
		
	}
	else
	{
		// Map the file into memory.
		table = (struct KeyValue_Table *) mmap(NULL, sizeof(struct KeyValue_Table), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (table == MAP_FAILED)
		{
			perror("mmap");
			exit(1);
		}
		if (table->max_size == 0)
		{
			const char *errorMessage = "\nTable initialization failed: max_size == 0\nforce execute please type \"./rebis-cli --force\"\nBut you might not expect operations to run correctly\n";
			// print message to stderr
			fprintf(stderr, "%s\n", errorMessage);
			//perror(errorMessage);
			if ( argc == 1 )
			{
				return -1;
				// if (strcmp(argv[1], "--force") == 0)
				// {
				// 	table->max_size = 50;
				// }
			}
			else
			{
				return -1;
			}
		}
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
	if (msync(table, sizeof(struct KeyValue_Table), MS_SYNC) == -1)
	{
		//perror("msync");
		// You can check errno to get more details about the error
		int msync_error = errno;
		fprintf(stderr, "msync error: %s\n", strerror(msync_error));
	}

	if (munmap(table, sizeof(struct KeyValue_Table)) == -1)
	{
		//perror("munmap");
		// You can check errno to get more details about the error
		int munmap_error = errno;
		fprintf(stderr, "munmap error: %s\n", strerror(munmap_error));
	}


	freeKeyValueTable(table);
	close(fd);

	return 0;
}
