/* Source file of crud.h */
#include "../include/datastructure.h"
#include "../include/log.h"
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
    if (qobj && query_string && key && value && msg)
    {
        qobj->query_string = strdup(query_string);
        qobj->status_code  = status_code;
        qobj->key = strdup(key);
        qobj->val = strdup(value);
        qobj->message = strdup(msg);
    }
    else
    {
        log_message("[Error] set_query_info() failed due to assignment of null pointer.");
    }
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
    }

    // 新增一個新的節點 再串上去
    struct KeyValue *new_entry = (struct KeyValue *) malloc (sizeof(struct KeyValue));
    strcpy(new_entry->key, search_key);
    strcpy(new_entry->value, value);

    table->count_entries++;

    new_entry->next = &(table->records[index]);
    table->records[index] = *new_entry;
    sprintf(msg, "Inserted key '%s' with value '%s' at index '%d'.", search_key, value, index);
    set_query_info(qobj, query_string, 9, search_key, value, msg); 

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
            set_query_info(qobj, query_string, 11, search_key, current->value, msg);
            return qobj;
        }
        current = current->next;
    }

    sprintf(msg, "Key '%s' not found.", search_key);
    set_query_info(qobj, query_string, 11, search_key, "nil", msg);
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
            
            set_query_info(qobj, query_string, 13, search_key, current->value, msg);

            return qobj;
        }
        current = current->next;
    }

    sprintf(msg, "Key '%s' not found.", search_key);
    set_query_info(qobj, query_string, 13, search_key, "nil", msg);

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

// get_list_index()沒有找到的時候才會create_list()
int create_list(struct List_Connection *lconnection, const char *listname)
{
    int index = (lconnection->count_entries)++;
    strncpy(lconnection->list[index].name, listname, 32);
    return index;
}

struct QueryObject* list_lpush(struct List_Connection *lconnection, const char *listname, const char *value)
{

    int index = get_list_index(lconnection, listname);

    struct QueryObject *qobj = (struct QueryObject*)malloc(sizeof(struct QueryObject));
    char query_string[168], msg[200];
    sprintf(query_string, "LPUSH %s %s", lconnection->list[index].name, value);

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
        lconnection->list[index].tail = newnode;
    }
    else
    {
        // The list is not empty
        newnode->next = lconnection->list[index].head;
        lconnection->list[index].head->prev = newnode;
        lconnection->list[index].head = newnode;
    }
    // 成功push的話 在這個index的list又新增了一個新的元素
    lconnection->list[index].length++;

    sprintf(msg, "left push \"%s\" into list \"%s\"", value, lconnection->list[index].name);
    set_query_info(qobj, query_string, 15, "N/A", value, msg);

    return qobj;
}

struct QueryObject* list_lpop(struct List_Connection *lconnection, const char *listname)
{
    int index = get_list_index(lconnection, listname);

    struct QueryObject *qobj = (struct QueryObject*)malloc(sizeof(struct QueryObject));
    char query_string[168], msg[188], value[128];
    sprintf(query_string, "LPOP %s %s", lconnection->list[index].name, value);

    if (index == -1)
    {
        sprintf(msg, "[Error] List \"%s\" not exist.", listname);
        set_query_info(qobj, query_string, 16, "N/A", "N/A", msg);
        return qobj;
    }

    if (lconnection->list[index].head == NULL)
    {
        // Nothing to pop
        sprintf(msg, "[Error] Nothing to pop in List \"%s\"", listname);
        set_query_info(qobj, query_string, 16, "N/A", "N/A", msg);
    }
    else
    {
        struct Node *temp = lconnection->list[index].head;
        // Copy the poped value
        strncpy(value, temp->value, 128);

        if (temp->next != NULL) {
            // If there are more elements in the list
            lconnection->list[index].head = temp->next;
            temp->next->prev = NULL;
        } else {
            // If this was the last element in the list
            lconnection->list[index].head = NULL;
            lconnection->list[index].tail = NULL;
        }

        free(temp);

        // length減1
        lconnection->list[index].length--;

        sprintf(msg, "left pop \"%s\" from list \"%s\"", value, lconnection->list[index].name);
        set_query_info(qobj, query_string, 15, "N/A", value, msg);
    }

    return qobj;
}

struct QueryObject* list_rpush(struct List_Connection *lconnection, const char *listname, const char *value)
{

    int index = get_list_index(lconnection, listname);

    struct QueryObject *qobj = (struct QueryObject*)malloc(sizeof(struct QueryObject));
    char query_string[168], msg[200];
    sprintf(query_string, "RPUSH %s %s", lconnection->list[index].name, value);

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
        lconnection->list[index].tail = newnode;
    }
    else
    {
        // The list is not empty
        newnode->prev = lconnection->list[index].tail;
        lconnection->list[index].tail->next = newnode;
        lconnection->list[index].tail = newnode;
    }
    // 成功push的話 在這個index的list又新增了一個新的元素
    lconnection->list[index].length++;

    sprintf(msg, "right push \"%s\" into list \"%s\"", value, lconnection->list[index].name);
    set_query_info(qobj, query_string, 17, "N/A", value, msg);

    return qobj;
}

struct QueryObject* list_rpop(struct List_Connection *lconnection, const char *listname)
{
    int index = get_list_index(lconnection, listname);

    struct QueryObject *qobj = (struct QueryObject*)malloc(sizeof(struct QueryObject));
    char query_string[168], msg[188], value[128];
    sprintf(query_string, "RPOP %s %s", lconnection->list[index].name, value);

    if (index == -1)
    {
        sprintf(msg, "[Error] List \"%s\" not exist.", listname);
        set_query_info(qobj, query_string, 18, "N/A", "N/A", msg);
        return qobj;
    }

    if (lconnection->list[index].head == NULL)
    {
        // Nothing to pop
        sprintf(msg, "[Error] Nothing to pop in List \"%s\"", listname);
        set_query_info(qobj, query_string, 18, "N/A", "N/A", msg);
    }
    else
    {
        struct Node *temp = lconnection->list[index].tail;
        // Copy the poped value
        strncpy(value, temp->value, 128);

        if (temp->prev != NULL) {
            // If there are more elements in the list
            lconnection->list[index].tail = temp->prev;
            temp->prev->next = NULL;
        } else {
            // If this was the last element in the list
            lconnection->list[index].head = NULL;
            lconnection->list[index].tail = NULL;
        }

        // free(head->prev)
        free(temp);

        // length減1
        lconnection->list[index].length--;

        sprintf(msg, "right pop \"%s\" from list \"%s\"", value, lconnection->list[index].name);
        set_query_info(qobj, query_string, 18, "N/A", value, msg);
    }

    return qobj;
}

struct QueryObject* list_length(struct List_Connection *lconnection, const char *listname)
{
    int index = get_list_index(lconnection, listname);

    struct QueryObject *qobj = (struct QueryObject*)malloc(sizeof(struct QueryObject));
    char query_string[188], msg[200];
    sprintf(query_string, "LLEN %s", listname);

    if (index == -1)
    {
        sprintf(msg, "[Error] List \"%s\" not exist.", listname);
        set_query_info(qobj, query_string, 19, "N/A", "N/A", msg);
        return qobj;
    }

    sprintf(msg, "Got length of list \"%s\" : %d", listname, lconnection->list[index].length);
    set_query_info(qobj, query_string, 19, "N/A", "N/A", msg);
    return qobj;
}

struct QueryObject* list_at_index(struct List_Connection *lconnection, const char *listname, int index)
{
    int count = 0;
    struct QueryObject *qobj =  (struct QueryObject*)malloc(sizeof(struct QueryObject));
    char query_string[188], msg[200];
    sprintf(query_string, "LINDEX %s", listname);

    int id;
    if ( (id = get_list_index(lconnection, listname)) == -1)
    {
        sprintf(msg, "[Error] List \"%s\" not found.", listname);
        set_query_info(qobj, query_string, 888, "N/A", "N/A", msg);
        return qobj;
    }

    for (struct Node *current = lconnection->list[id].head; current != NULL; current = current->next)
    {
        if (count == index)
        {
            sprintf(msg, "Found value \"%s\" at index %d", current->value, count);
            set_query_info(qobj, query_string, 888, "N/A", current->value, msg);
            return qobj;
        }
        count++;
    }
    sprintf(msg, "[Error] Index %d not reachable in list \"%s\"", index, listname);
    set_query_info(qobj, query_string, 888, "N/A", "N/A", msg);
    return qobj;
}

struct QueryObject* list_range(struct List_Connection *lconnection, const char *listname, int start, int stop)
{
    struct QueryObject *qobj = (struct QueryObject*)malloc(sizeof(struct QueryObject));
    char query_string[188], value[1024], msg[1024]; // msg might be long
    sprintf(query_string, "LRANGE %s %d %d", listname, start, stop);
    value[0] = '\0';
    msg[0] = '\0';

    // Todo : support circular index query
    if (stop < start)
    {
        set_query_info(qobj, query_string, 20, "N/A", "N/A", "[Error] LRANGE: Currently only support stop > start");
        return NULL;
    }

    for (int i = start; i <= stop; i++)
    {
        struct QueryObject *temp = list_at_index(lconnection, listname, i);
        strncat(value, temp->val, 128);
        strncat(msg, temp->message, 200);
        // value[strlen(value)] = '\n';
        // msg[strlen(msg)] = '\n';
        strncat(value, "\n", 2);
        strncat(msg, "\n", 2);
        free(temp);
    }
    value[strlen(value)] = '\0';
    msg[strlen(msg)] = '\0';

    set_query_info(qobj, query_string, 20, "N/A", value, msg);
    return qobj;
}

struct QueryObject* type_command(char *query_string, struct Connection* connection)
{   
    struct QueryObject *queryobject = (struct QueryObject*)malloc(sizeof(struct QueryObject));

    if (strcmp(query_string, "exit") == 0 || strcmp(query_string, "EXIT") == 0)
    {
        // Exit
        set_query_info(queryobject, "EXIT", 0, "N/A", "N/A", "Bye!");
    }
    else if (strcmp(query_string, "\0") == 0)
    {
        // Nextline
        set_query_info(queryobject, "NEXTLINE", 1, "N/A", "N/A", "127.0.0.1:8888 > ");
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
            set_query_info(queryobject, query_string, 10, key, "N/A", "[Usage] SET <key> <value>");
        }
    }
    else if (strncmp(query_string, "get ", 4) == 0 || strncmp(query_string, "GET ", 4) == 0)
    {
        char *key = query_string + 4; // Move the pointer past "get "
        if (strcmp(key, " ") == 0)
        {
            // get_usage
            set_query_info(queryobject, query_string, 12, key, "N/A", "[Usage] GET <key>");
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
            set_query_info(queryobject, query_string, 14, key, "N/A", "[Usage] DEL <key>");
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
        set_query_info(queryobject, query_string, 5, "N/A", "N/A", "\nSET <key> <value>\n    example : SET name Caroline\n    output  : OK\n    example : SET height 168\n    output  : OK\n\nGET <key>\n    example : GET name\n    output  : \"Caroline\"\n    example : GET weight\n    output  : (nil)\n\nDEL <key>\n    example : DEL height\n    output  : OK\n    example : DEL helloworld\n    output  : (nil)\n\nEXIT\n    example : EXIT\n    output  : Bye!\n");
    }
	else if (strncmp(query_string, "flushdb", 7) == 0 || strncmp(query_string, "FLUSHDB", 7) == 0)
	{
        if ( access(connection->tc->filename, F_OK) == -1 || access(connection->lc->filename, F_OK) == -1)
        {
            set_query_info(queryobject, query_string, 6, "N/A", "N/A", "[Error] No data file was found.");
        }
		else if (unlink(connection->tc->filename) == 0 && unlink(connection->lc->filename) == 0)
		{
            char msg[105];
            sprintf(msg, "Data file '%s', '%s' deleted successfully.\nPlease restart rebis-cli to begin with new session.", connection->tc->filename, connection->lc->filename);
            set_query_info(queryobject, query_string, 6, "N/A", "N/A", msg);
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
        
        // if(strlen(listname) > 32 || strlen(value) > 128)
        // {
        //     set_query_info(queryobject, query_string, 15, "N/A", "N/A", "[Error] listname length must be under 32 bytes, value length must be under 128 bytes");
        //     return queryobject;
        // }
        
        if (listname != NULL && value != NULL)
        {
            return list_lpush(connection->lc, listname, value);
        }
        else
        {
            set_query_info(queryobject, query_string, 15, "N/A", "N/A", "[Usage] LPUSH <list name> <value>");
        }

    }
    else if (strncmp(query_string, "lpop", 4) == 0 || strncmp(query_string, "LPOP", 4) == 0 )
    {
        char *args = query_string + 5; // Move the pointer past "lpush "
        char *listname = strtok(args, " ");
        
        if (listname != NULL)
        {
            return list_lpop(connection->lc, listname);
        }
        else
        {
            set_query_info(queryobject, query_string, 16, "N/A", "N/A", "[Usage] LPOP <list name>");
        }

    }
    else if (strncmp(query_string, "rpush", 5) == 0 || strncmp(query_string, "RPUSH", 5) == 0 )
    {
        char *args = query_string + 6; // Move the pointer past "lpush "
        char *listname = strtok(args, " ");
        char *value = strtok(NULL, " ");
        
        if (listname != NULL && value != NULL)
        {
            return list_rpush(connection->lc, listname, value);
        }
        else
        {
            set_query_info(queryobject, query_string, 17, "N/A", "N/A", "[Usage] RPUSH <list name> <value>");
        }
    }
    else if (strncmp(query_string, "rpop", 4) == 0 || strncmp(query_string, "RPOP", 4) == 0 )
    {
        char *args = query_string + 5; // Move the pointer past "lpush "
        char *listname = strtok(args, " ");
        
        if (listname != NULL)
        {
            return list_rpop(connection->lc, listname);
        }
        else
        {
            set_query_info(queryobject, query_string, 18, "N/A", "N/A", "[Usage] RPOP <list name>");
        }
    }
    else if (strncmp(query_string, "llen", 4) == 0 || strncmp(query_string, "LLEN", 4) == 0 )
    {
        char *args = query_string + 5; // Move the pointer past "lpush "
        char *listname = strtok(args, " ");
        
        if (listname != NULL)
        {
            return list_length(connection->lc, listname);
        }
        else
        {
            set_query_info(queryobject, query_string, 19, "N/A", "N/A", "[Usage] LLEN <list name>");
        }
    }
    else if (strncmp(query_string, "lrange", 6) == 0 || strncmp(query_string, "LRANGE", 6) == 0 )
    {
        char *args = query_string + 7; // Move the pointer past "lpush "
        char *listname = strtok(args, " ");
        
        if (listname != NULL)
        {
            char *start_str = strtok(NULL, " ");
            char *stop_str = strtok(NULL, " ");

            if (start_str != NULL && stop_str != NULL)
            {
                int start = atoi(start_str);
                int stop  = atoi(stop_str);
                return list_range(connection->lc, listname, start, stop);
            }
        }
        set_query_info(queryobject, query_string, 20, "N/A", "N/A", "[Usage] LRANGE <list name> <start idx> <stop idx>");
        return queryobject;
    }
    else
    {
        char msg[100];
        sprintf(msg, "[Error] Command \"%s\" not found.", query_string);
        set_query_info(queryobject, query_string, 7, "N/A", "N/A", msg);
    }

    return queryobject;
}