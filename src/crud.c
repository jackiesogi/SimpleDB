/* Source file of crud.h */
#include "../include/datastructure.h"
#include "../include/log.h"
// #include ""
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

unsigned int hash_function(const char *str, int max_entries)
{
    unsigned int hash = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        hash = (hash * 31) + (unsigned int)str[i];
    }
    return hash % max_entries;
}

struct QueryObject* store_value_by_key(struct KeyValue_Table *table, const char *search_key, const char *value)
{
    unsigned int index = hash_function(search_key, table->max_size);
    struct QueryObject *qobj = (struct QueryObject*)malloc(sizeof(struct QueryObject));

    if( table->records[index].key[0] != '\0' )
    {
        qobj->status_code = 8;
        qobj->result = strdup("[Error] Collision occured!");    // strdup 會allocate新空間然後回傳新指標給string literal(in read-only memory section)
    }

    struct KeyValue *new_entry = (struct KeyValue *) malloc (sizeof(struct KeyValue));
    strcpy(new_entry->key, search_key);
    strcpy(new_entry->value, value);

    table->count_entries++;

    new_entry->next = &(table->records[index]);
    table->records[index] = *new_entry;
    
    char buffer[100];
    int written = snprintf(buffer, sizeof(buffer), "Inserted key '%s' with value '%s' at index '%d'.", search_key, value, index);
    if (written >= 0 && written < (int)sizeof(buffer)) {
        qobj->status_code = 9;
        qobj->result = strdup(buffer);
    }

    return qobj;
}

struct QueryObject* retrieve_value_by_key(struct KeyValue_Table *table, const char *search_key)
{
	unsigned int index = hash_function(search_key, table->max_size);
    struct QueryObject *qobj = (struct QueryObject*)malloc(sizeof(struct QueryObject));
    struct KeyValue *current = &(table->records[index]);

    while (current != NULL && current->key[0] != '\0')
    {
        if (strcmp(current->key, search_key) == 0)
        {
            char buffer[100];
            int written = snprintf(buffer, sizeof(buffer), "Found key '%s' with value '%s' at index '%d'.", search_key, current->value, index);
            if (written >= 0 && written < (int)sizeof(buffer))
            {
                qobj->status_code = 11;
                qobj->result = strdup(buffer);
            }
            return qobj;
        }
        current = current->next;
    }

    char buffer[100];
    int written = snprintf(buffer, sizeof(buffer), "Key '%s' not found.", search_key);
    if (written >= 0 && written < (int)sizeof(buffer))
    {
        qobj->status_code = 11;
        qobj->result = strdup(buffer);
    }
    return qobj; // Key not found
}

// struct QueryObject* delete_value_by_key(struct KeyValue_Table *table, const char *search_key)
// {
// 	unsigned int size = table->count_entries;
// 	for(unsigned int i = 0; i < size; i++)
// 	{
// 		if (strcmp(table->records[i].key, search_key) == 0)
// 		{
// 			table->records[i].value[0] = '\0';
// 			table->records[i].key[0] = '\0';
// 			puts("OK");
// 			return;
// 		}
// 	}
// 	puts("(nil)");
// }

struct QueryObject* delete_value_by_key(struct KeyValue_Table *table, const char *search_key)
{
	unsigned int index = hash_function(search_key, table->max_size);
    struct QueryObject *qobj = (struct QueryObject*)malloc(sizeof(struct QueryObject));
    struct KeyValue *current = &(table->records[index]);

    while (current != NULL && current->key[0] != '\0')
    {
        if (strcmp(current->key, search_key) == 0)
        {

            char buffer[100];
            int written = snprintf(buffer, sizeof(buffer), "Key '%s' with value '%s' at index '%d' deleted.", search_key, current->value, index);
            
            // delete key and value
            current->key[0] = '\0';
            current->value[0] = '\0';
            table->count_entries--;

            if (written >= 0 && written < (int)sizeof(buffer))
            {
                qobj->status_code = 13;
                qobj->result = strdup(buffer);
            }
            return qobj;
        }
        current = current->next;
    }

    char buffer[100];
    int written = snprintf(buffer, sizeof(buffer), "Key '%s' not found.", search_key);
    if (written >= 0 && written < (int)sizeof(buffer))
    {
        qobj->status_code = 13;
        qobj->result = strdup(buffer);
    }
    return qobj; // Key not found
}

// 兩層QueryObject不知道會不會浪費效能
struct QueryObject* type_command(char *query_string, struct Connection* connection)
{   
    struct QueryObject *queryobject = (struct QueryObject*)malloc(sizeof(struct QueryObject));

    if (strcmp(query_string, "exit") == 0 || strcmp(query_string, "EXIT") == 0)
    {
        // Exit
        queryobject->status_code = 0;
        queryobject->result = strdup("Bye!");
    }
    else if (strcmp(query_string, "\0") == 0)
    {
        // Nextline
        queryobject->status_code = 1;
        queryobject->result = strdup("127.0.0.1:8888 > ");
    }
    else if (strncmp(query_string, "set ", 4) == 0 || strncmp(query_string, "SET ", 4) == 0)
    {
        char *args = query_string + 4; // Move the pointer past "set "
        char *key = strtok(args, " ");
        char *value = strtok(NULL, " ");
        if (key != NULL && value != NULL)
        {
            // set_ok
            free(queryobject);
            return store_value_by_key(connection->table, key, value);
        }
        else
        {
            // set_usage
            queryobject->status_code = 10;
            queryobject->result = strdup("[Usage] SET <key> <value>");
        }
    }
    else if (strncmp(query_string, "get ", 4) == 0 || strncmp(query_string, "GET ", 4) == 0)
    {
        char *key = query_string + 4; // Move the pointer past "get "
        if (strcmp(key, " ") == 0)
        {
            // get_usage
            queryobject->status_code = 12;
            queryobject->result = strdup("[Usage] GET <key>");
        }
        else
        {
            // get_ok
            free(queryobject);
            return retrieve_value_by_key(connection->table, key);
        }
    }
    else if (strncmp(query_string, "del ", 4) == 0 || strncmp(query_string, "DEL ", 4) == 0)
    {
        char *key = query_string + 4; // Move the pointer past "get "
        if (strcmp(key, " ") == 0)
        {   
            // del_usage
            queryobject->status_code = 14;
            queryobject->result = strdup("[Usage] DEL <key>");
        }
        else
        {
            // del_ok
            free(queryobject);
            return delete_value_by_key(connection->table, key);
        }
    }
    else if (strncmp(query_string, "help", 4) == 0 || strncmp(query_string, "HELP", 4) == 0)
    {
        // help
        queryobject->status_code = 5;
        queryobject->result = strdup("\nSET <key> <value>\n    example : SET name Caroline\n    output  : OK\n    example : SET height 168\n    output  : OK\n\nGET <key>\n    example : GET name\n    output  : \"Caroline\"\n    example : GET weight\n    output  : (nil)\n\nDEL <key>\n    example : DEL height\n    output  : OK\n    example : DEL helloworld\n    output  : (nil)\n\nEXIT\n    example : EXIT\n    output  : Bye!\n");
    }
	else if (strncmp(query_string, "flushdb", 7) == 0 || strncmp(query_string, "FLUSHDB", 7) == 0)
	{
		if (unlink(connection->filename) == 0)
		{
            char buffer[100];
            int written = snprintf(buffer, sizeof(buffer), "File '%s' deleted successfully.", connection->filename);

            if (written >= 0 && written < (int)sizeof(buffer))
            {
                queryobject->status_code = 6;
                queryobject->result = strdup(buffer);
            }
    	}
		else
		{
        	perror("unlink");
    	}
	}
    else
    {
        char buffer[100];
        int written = snprintf(buffer, sizeof(buffer), "[Error] Command \"%s\" not found.", query_string);

        if (written >= 0 && written < (int)sizeof(buffer))
        {
            queryobject->status_code = 7;
            queryobject->result = strdup(buffer); 
        }
    }

    return queryobject;
}