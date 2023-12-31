#ifndef CRUD_H
#define CRUD_H

#include "./datastructure.h"

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
extern struct QueryObject* table_set(struct KeyValue_Table *table, const char *search_key, const char *value);


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
extern struct QueryObject* table_get(struct KeyValue_Table *table, const char *search_key);


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
extern struct QueryObject* table_del(struct KeyValue_Table *table, const char *search_key);

extern struct QueryObject* type_command(char *query_string, struct Connection* connection);

extern void set_query_info(struct QueryObject *qobj,
                    const char *query_string,
                    const int  status_code,
                    const char *key,
                    const char *value,
                    const char *msg);

#endif
