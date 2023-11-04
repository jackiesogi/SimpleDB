/* Source file of crud.h */
#include "../include/datastructure.h"
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

void set_query_info(struct QueryObject *qobj,
                    const char *query_string,
                    const int  status_code,
                    const char *key,
                    const char *value,
                    const char *msg)
{
    qobj->query_string = strdup(query_string);
    qobj->status_code  = status_code;
    qobj->key = strdup(key);
    qobj->val = strdup(value);
    qobj->message = strdup(msg);
}

struct QueryObject* table_set(struct KeyValue_Table *table, const char *search_key, const char *value)
{
    unsigned int index = hash_function(search_key, table->max_size);
    struct QueryObject *qobj = (struct QueryObject*)malloc(sizeof(struct QueryObject));

    // query_string 存 當初query的指令，msg存query後要顯示的結果
    char query_string[100], msg[256];
    sprintf(query_string, "SET %s %s", search_key, value);

    if( table->records[index].key[0] != '\0' )
    {
        set_query_info(qobj, query_string, 8, search_key, value, msg);
        // qobj->query_string = strdup(query_string);
        // qobj->status_code = 8;
        // qobj->key = strdup(search_key);
        // qobj->val = strdup(value);
        // qobj->message = strdup("[Error] Collision occured!");    // strdup 會allocate新空間然後回傳新指標給string literal(in read-only memory section)
        // return qobj; // 發生碰撞的返回值要再想想
    }

    // 新增一個新的節點 再串上去
    struct KeyValue *new_entry = (struct KeyValue *) malloc (sizeof(struct KeyValue));
    strcpy(new_entry->key, search_key);
    strcpy(new_entry->value, value);

    table->count_entries++;

    new_entry->next = &(table->records[index]);
    table->records[index] = *new_entry;
    
    sprintf(msg, "Inserted key '%s' with value '%s' at index '%d'.", search_key, value, index);
    qobj->query_string = strdup(query_string);
    qobj->status_code = 9;
    qobj->key = strdup(search_key);
    qobj->val = strdup(value);
    qobj->message = strdup(msg); 

    return qobj;
}

// Todo : change the snprintf() to sprintf(), and add other attribute of
struct QueryObject* table_get(struct KeyValue_Table *table, const char *search_key)
{
	unsigned int index = hash_function(search_key, table->max_size);
    struct QueryObject *qobj = (struct QueryObject*)malloc(sizeof(struct QueryObject));
    struct KeyValue *current = &(table->records[index]);

    char query_string[100], msg[256];
    sprintf(query_string, "GET %s", search_key);

    while (current != NULL && current->key[0] != '\0')
    {
        if (strcmp(current->key, search_key) == 0)
        {
            sprintf(msg, "Found key '%s' with value '%s' at index '%d'.", search_key, current->value, index);
            
            qobj->query_string = strdup(query_string);
            qobj->status_code = 11;
            qobj->key = strdup(search_key);
            qobj->val = strdup(current->value);
            qobj->message = strdup(msg);
            return qobj;
        }
        current = current->next;
    }

    sprintf(msg, "Key '%s' not found.", search_key);
    qobj->query_string = strdup(query_string);
    qobj->status_code = 11;
    qobj->key = strdup(search_key);
    qobj->val = strdup("nil");
    qobj->message = strdup(msg);
    
    return qobj; // Key not found
}

struct QueryObject* table_del(struct KeyValue_Table *table, const char *search_key)
{
	unsigned int index = hash_function(search_key, table->max_size);
    struct QueryObject *qobj = (struct QueryObject*)malloc(sizeof(struct QueryObject));
    struct KeyValue *current = &(table->records[index]);

    char msg[256], query_string[100];

    while (current != NULL && current->key[0] != '\0')
    {
        if (strcmp(current->key, search_key) == 0)
        {
            sprintf(msg, "Deleted key '%s' with value '%s' at index '%d'.", search_key, current->value, index);

            // delete key and value
            current->key[0] = '\0';
            current->value[0] = '\0';
            table->count_entries--;

            qobj->query_string = strdup(query_string);
            qobj->status_code = 13;
            qobj->key = strdup(search_key);
            qobj->val = strdup(current->value);
            qobj->message = strdup(msg);
            
            return qobj;
        }
        current = current->next;
    }

    sprintf(msg, "Key '%s' not found.", search_key);
    qobj->query_string = strdup(query_string);
    qobj->status_code = 13;
    qobj->key = strdup(search_key);
    qobj->val = strdup("nil");
    qobj->message = strdup(msg);

    return qobj; // Key not found
}

int get_list_index(struct List_Connection *lconnection, const char* listname)
{
    int len = lconnection->count_entries;
    for(int i = 0; i < len; i++)
    {
        if (strncmp(lconnection->list[i].name, listname, 32) == 0)
        {
            return i;
        }
    }
    return -1;
}

int create_list(struct List_Connection *lconnection, const char *listname)
{
    int index = (lconnection->count_entries)++;
    strncpy(lconnection->list[index].name, listname, 32);
    return index;
}

struct QueryObject* list_lpush(struct List_Connection *lconnection, const char *listname, const char *value)
{
    int index = get_list_index(lconnection, listname);

    if (index == -1)
    {
        index = create_list(lconnection, listname);
    }

    struct Node *newnode = (struct Node*)calloc(1, sizeof(struct Node));
    strncpy(newnode->value, value, 128);

    if (lconnection->list[index].head == NULL)
    {
        // The list is initially empty, so the new node becomes the head
        lconnection->list[index].head = newnode;
    }
    else
    {
        // The list is not empty
        newnode->next = lconnection->list[index].head;
        lconnection->list[index].head->prev = newnode;
        lconnection->list[index].head = newnode;
    }

    char query_string[100], msg[100];
    sprintf(query_string, "LPUSH %s %s", lconnection->list[index].name, value);
    sprintf(msg, "left push %s into list %s", value, lconnection->list[index].name);

    struct QueryObject *qobj = (struct QueryObject*)malloc(sizeof(struct QueryObject));
    qobj->query_string = strdup(query_string);
    qobj->key = strdup("N/A");
    qobj->val = strdup(value);
    qobj->status_code = 15;
    qobj->message = strdup(msg);

    return qobj;
}
// struct QueryObject* list_rpush(struct List_Connection *lconnection, const char *listname, const char *value)
// {
//     int index = get_list_index(lconnection, listname);

//     if (index == -1)
//     {
//         create_list(lconnection, listname);
//     }

//     struct Node *newnode = (struct Node*)calloc(1, sizeof(struct Node));
//     strncpy(newnode->value, value, 128);

//     if (lconnection->list[index].tail == NULL)
//     {
//         lconnection->list[index].tail = newnode;
//     }
//     else
//     {
//         newnode->prev = lconnection->list[index].tail;
//         lconnection->list[index].tail->next = newnode;
//         lconnection->list[index].tail = newnode;
//     }

//     char query_string[100], msg[100];
//     sprintf(query_string, "RPUSH %s %s", listname, value);
//     sprintf(msg, "right push element \"%s\" into list \"%s\"", value, listname);

//     struct QueryObject qobj = (struct QueryObject*)malloc(sizeof(struct QueryObject));
//     qobj.query_string = strdup(query_string);
//     qobj.message = strdup(msg);
//     qobj.val = value;
// }
// struct QueryObject* list_lpop();
// struct QueryObject* list_rpop();
// struct QueryObject* list_range();
// struct QueryObject* list_length();

struct QueryObject* type_command(char *query_string, struct Connection* connection)
{   
    struct QueryObject *queryobject = (struct QueryObject*)malloc(sizeof(struct QueryObject));

    if (strcmp(query_string, "exit") == 0 || strcmp(query_string, "EXIT") == 0)
    {
        // Exit
        queryobject->status_code = 0;
        queryobject->message = strdup("Bye!");
    }
    else if (strcmp(query_string, "\0") == 0)
    {
        // Nextline
        queryobject->status_code = 1;
        queryobject->message = strdup("127.0.0.1:8888 > ");
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
            return table_set(connection->tc->table, key, value);
        }
        else
        {
            // set_usage
            queryobject->status_code = 10;
            queryobject->message = strdup("[Usage] SET <key> <value>");
        }
    }
    else if (strncmp(query_string, "get ", 4) == 0 || strncmp(query_string, "GET ", 4) == 0)
    {
        char *key = query_string + 4; // Move the pointer past "get "
        if (strcmp(key, " ") == 0)
        {
            // get_usage
            queryobject->status_code = 12;
            queryobject->message = strdup("[Usage] GET <key>");
        }
        else
        {
            // get_ok
            free(queryobject);
            return table_get(connection->tc->table, key);
        }
    }
    else if (strncmp(query_string, "del ", 4) == 0 || strncmp(query_string, "DEL ", 4) == 0)
    {
        char *key = query_string + 4; // Move the pointer past "get "
        if (strcmp(key, " ") == 0)
        {   
            // del_usage
            queryobject->status_code = 14;
            queryobject->message = strdup("[Usage] DEL <key>");
        }
        else
        {
            // del_ok
            free(queryobject);
            return table_del(connection->tc->table, key);
        }
    }
    else if (strncmp(query_string, "help", 4) == 0 || strncmp(query_string, "HELP", 4) == 0)
    {
        // help
        queryobject->status_code = 5;
        queryobject->message = strdup("\nSET <key> <value>\n    example : SET name Caroline\n    output  : OK\n    example : SET height 168\n    output  : OK\n\nGET <key>\n    example : GET name\n    output  : \"Caroline\"\n    example : GET weight\n    output  : (nil)\n\nDEL <key>\n    example : DEL height\n    output  : OK\n    example : DEL helloworld\n    output  : (nil)\n\nEXIT\n    example : EXIT\n    output  : Bye!\n");
    }
	else if (strncmp(query_string, "flushdb", 7) == 0 || strncmp(query_string, "FLUSHDB", 7) == 0)
	{
        if ( access(connection->tc->filename, F_OK) == -1 || access(connection->lc->filename, F_OK) == -1)
        {
            queryobject->status_code = 6;
            queryobject->message = strdup("[Error] No data file was found.");
        }
		else if (unlink(connection->tc->filename) == 0 && unlink(connection->lc->filename) == 0)
		{
            char msg[105];
            sprintf(msg, "Data file '%s', '%s' deleted successfully.\nPlease restart rebis-cli to begin with new session.", connection->tc->filename, connection->lc->filename);
            queryobject->status_code = 6;
            queryobject->message = strdup(msg);
    	}
		else
		{
        	perror("unlink");
    	}
	}
    else if (strncmp(query_string, "lpush", 5) == 0 || strncmp(query_string, "LPUSH", 5) == 0 )
    {
        char *args = query_string + 6; // Move the pointer past "lpush "
        char *listname = strtok(args, " ");
        char *value = strtok(NULL, " ");
        
        if (listname != NULL && value != NULL)
        {
            list_lpush(connection->lc, listname, value);
            
            char msg[100];
            sprintf(msg, "left push element \"%s\" to list \"%s\"", value, listname);

            queryobject->status_code = 15;
            queryobject->message = strdup(msg);
        }
        else
        {
            queryobject->status_code = 15;
            queryobject->message = strdup("[Usage] LPUSH <list name> <value>");
        }

    }
    else
    {
        char msg[100];
        sprintf(msg, "[Error] Command \"%s\" not found.", query_string);
        queryobject->status_code = 7;
        queryobject->message = strdup(msg);
    }

    return queryobject;
}