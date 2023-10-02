#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define FILENAME "kvstore.dat"
#define MAX_ENTRIES 100

struct KeyValue {
    char key[32];
    char value[128];
};

void store_key_value(struct KeyValue *kv, int num_entries, const char *key, const char *value) {
    if (num_entries >= MAX_ENTRIES) {
        printf("Error: Maximum number of entries reached.\n");
        return;
    }

    strcpy(kv[num_entries].key, key);
    strcpy(kv[num_entries].value, value);
}

int main() {
    int fd;
    struct KeyValue *kv;
    int num_entries = 0;

    // Create or open the memory-mapped file.
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

    // Example: Store some key-value pairs in memory.
    store_key_value(kv, num_entries++, "name", "John");
    store_key_value(kv, num_entries++, "age", "30");
    store_key_value(kv, num_entries++, "city", "New York");

    // Example: Retrieve a value by key.
    const char *search_key = "age";
    for (int i = 0; i < num_entries; i++) {
        if (strcmp(kv[i].key, search_key) == 0) {
            printf("Value for key '%s' is '%s'\n", search_key, kv[i].value);
            break;
        }
    }

    // Unmap and close the file.
    munmap(kv, MAX_ENTRIES * sizeof(struct KeyValue));
    close(fd);

    return 0;
}
