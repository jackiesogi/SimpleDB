#ifndef CRUD_H
#define CRUD_H

// store the key value pair into structure KeyValue
void store_key_value(struct KeyValue **kv, int *num_entries, const char *key, const char *value);

// retrieve value by key
char* retrieve_value_by_key(struct KeyValue **kv, int num_entries, const char *search_key);

#include "crud.c"
#endif
