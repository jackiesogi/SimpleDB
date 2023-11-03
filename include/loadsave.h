#ifndef LOAD_N_SAVE_H
#define LOAD_N_SAVE_H

#include "../include/datastructure.h"

// extern struct KeyValue_Table* load_table_from_file(int fd, void *table, const char* filenanme);

extern struct Table_Connection* load_table_from_file(const char* filenanme, int32_t table_size);

extern void save_table_to_file(const struct Table_Connection* connection);

extern void free_connection(struct Connection *connection);

extern struct List_Connection* load_list_from_file(const char *filenanme);

extern void save_list_to_file(const struct List_Connection *connection);

// extern void leave_program(struct Connection* connection);

#endif