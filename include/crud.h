#ifndef CRUD_H
#define CRUD_H
#include "./datastructure.h"

extern unsigned int hash_function(const char *str, int max_entries);

/************************************************************************/
/************************************************************************/  
/**  This function store a key-value pair into a given table           **/
/**                                                                    **/
/**  Parameters:                                                       **/
/**    - table   : The target table you want to store key and value.   **/
/**    - key     : The key you want to store.                          **/
/**    - value   : The value you want to store.                        **/
/**                                                                    **/
/**  Returns:                                                          **/
/**    - if succeeded it will return true.                             **/
/**    - if failed it will return false.                               **/
/************************************************************************/
/************************************************************************/
extern void store_value_by_key(struct KeyValue_Table *table, const char *search_key, const char *value);


/*************************************************************************/
/*************************************************************************/  
/**  This function retrieve a value from given table by a given key.    **/
/**                                                                     **/
/**  Parameters:                                                        **/
/**    - table      : The target table you want to retrieve value from. **/
/**    - search_key : The key of the value you want to retrieve.        **/
/**                                                                     **/
/**  Returns:                                                           **/
/**    - if succeeded it will return the value in string.               **/
/**    - if failed it will return NULL.                                 **/
/*************************************************************************/
/*************************************************************************/
extern const char *retrieve_value_by_key(struct KeyValue_Table *table, const char *search_key);


/*************************************************************************/
/*************************************************************************/  
/**  This function delete the value from given table by a given key.    **/
/**                                                                     **/
/**  Parameters:                                                        **/
/**    - table      : The target table you want to delete value from.   **/
/**    - search_key : The key of the value you want to delete.          **/
/**                                                                     **/
/**  Returns:                                                           **/
/**    (no return value)                                                **/
/*************************************************************************/
/*************************************************************************/
extern void delete_value_by_key(struct KeyValue_Table *table, const char *search_key);

// #include "../src/crud.c"
#endif
