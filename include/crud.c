/* Source file of crud.h */

void store_key_value(struct KeyValue **kv, int *num_entries, const char *key, const char *value) {
    if (*num_entries >= MAX_ENTRIES) {
        printf("Error: Maximum number of entries reached.\n");
        return;
    }

    strcpy(kv[*num_entries]->key, key);
    strcpy(kv[*num_entries]->value, value);
    (*num_entries)++;
}

char* retrieve_value_by_key(struct KeyValue **kv, int num_entries, const char *search_key){
	// Currently it is a linear search, hash function to be added.
    for (int i = 0; i < num_entries; i++) {
        if (strcmp(kv[i]->key, search_key) == 0) {
            printf("Value for key '%s' is '%s'\n", search_key, kv[i]->value);
            return kv[i]->value;
        }
    }
    return NULL;
}
