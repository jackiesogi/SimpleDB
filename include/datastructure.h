#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#define FILENAME "./data/kvstore.dat"
#define MAX_ENTRIES 100
#define QUERY_STRING_MAX 100

struct KeyValue
{
	char key[32];
	char value[128];
};

struct KeyValue_Table
{
	int count_entries;
	struct KeyValue records[MAX_ENTRIES];
};

#endif