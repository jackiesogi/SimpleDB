#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
//#include "../include/crud.h"

#define FILENAME "./data/kvstore.dat"
#define MAX_ENTRIES 100
#define QUERY_STRING_MAX 100

struct KeyValue {
    char key[32];
    char value[128];
};

void store_key_value(struct KeyValue *kv, int *num_entries, const char *key, const char *value) {
    if (*num_entries >= MAX_ENTRIES) {
        printf("Error: Maximum number of entries reached.\n");
        return;
    }

    strcpy(kv[*num_entries].key, key);
    strcpy(kv[*num_entries].value, value);
    (*num_entries)++;
    //printf("Key-Value pair set: %s -> %s\n", key, value);
    puts("OK");
}

char* retrieve_value_by_key(struct KeyValue *kv, int num_entries, const char *search_key){
	// Currently it is a linear search, hash function to be added.
    for (int i = 0; i < num_entries; i++) {
        if (strcmp(kv[i].key, search_key) == 0) {
            printf("\"%s\"\n", kv[i].value);
            return kv[i].value;
        }
    }
    puts("(nil)");
    return NULL;
}

int main() {

	puts("##############################################");	
	puts("##############################################");	
	puts("##                                          ##");
	puts("##          SimpleDB by jackiesogi          ##");
	puts("##                                          ##");	
	puts("##############################################");	
	puts("##############################################");	
	
	
    int fd;
    struct KeyValue *kv;
    int num_entries = 0;

    fd = open(FILENAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // Truncate the file to the size of the struct KeyValue array.
    ftruncate(fd, MAX_ENTRIES * sizeof(struct KeyValue));

    // Map the file into memory.
    kv = (struct KeyValue *)mmap(NULL, MAX_ENTRIES * sizeof(struct KeyValue), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (kv == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Assuming the maximum length of query_string is 99 characters plus null terminator
    char query_string[QUERY_STRING_MAX];

    while (1) {  // Infinite loop
        printf("127.0.0.1 > ");
        fgets(query_string, sizeof(query_string), stdin);

        // Remove newline character added by fgets
        size_t len = strlen(query_string);
        if (len > 0 && query_string[len - 1] == '\n') {
            query_string[len - 1] = '\0';
        }

        if (strcmp(query_string, "exit") == 0) {
        	puts("Bye!");
            break;  // Exit the loop
        }
        else if(strcmp(query_string, "\0") == 0){
			continue;        
        }
        else if (strncmp(query_string, "set ", 4) == 0) {
            char* args = query_string + 4;  // Move the pointer past "set "
            char* key = strtok(args, " ");
            char* value = strtok(NULL, " ");
            if (key != NULL && value != NULL) {
                store_key_value(kv, &num_entries, key, value);
            }
            else {
                printf("Invalid set command format.\n");
            }
        }
        else if (strncmp(query_string, "get ", 4) == 0) {
            char* key = query_string + 4;  // Move the pointer past "get "
            retrieve_value_by_key(kv, num_entries, key);
        }
        else {
            printf("Invalid command: %s\n", query_string);
        }
    }

    // Unmap and close the file.
    munmap(kv, MAX_ENTRIES * sizeof(struct KeyValue));
    close(fd);

    return 0;
}

