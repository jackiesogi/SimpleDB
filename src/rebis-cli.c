#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "../include/datastructure.h"
#include "../include/crud.h"
#include "../include/cli.h"
#include "../include/loadsave.h"
#include "../include/crud.h"

#include "../include/hirebis.h"

// status code is for future use
enum query_string_status
{ 
	quit,		// 0
	nextline,	// 1
	set,		// 2
	get,		// 3
	del,		// 4
	help,		// 5
	flushdb,	// 6
	invalid,	// 7
	collision,	// 8
	set_ok,		// 9
	set_use,	// 10
	get_ok,		// 11
	get_use,	// 12
	del_ok,		// 13
	del_use	// 14
};

/**********************************/
/** Entry point of the rebis-cli **/
/**********************************/
int main()
{
    printWelcomePage();

	const char* filename =  FILENAME;
	struct Connection *connection = load_data_from_file(filename);

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

		struct QueryObject* qobj = type_command(query_string, connection);

		printf("%s\n", qobj->result);
		if(qobj->status_code == 0)
		{
			break;
		}

    }

	save_data_to_file(connection);
	free_connection(connection);
    
	return 0;
}