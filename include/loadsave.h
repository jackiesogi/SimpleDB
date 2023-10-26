#ifndef LOAD_N_SAVE_H
#define LOAD_N_SAVE_H

#include "../include/datastructure.h"

// extern struct KeyValue_Table* load_data_from_file(int fd, void *table, const char* filenanme);

extern struct Connection* load_data_from_file(const char* filenanme);

extern void save_data_to_file(const struct Connection* connection);

extern void free_connection(struct Connection *connection);
// extern void leave_program(struct Connection* connection);

#endif