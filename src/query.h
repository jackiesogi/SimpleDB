#ifndef QUERY_H
#define QUERY_H

#define QUERY_STRING_MAX 100

// QueryObject
struct QueryObject
{
	const char *query_string;
	int status_code;
	const char *key;
	const char *val;
	const char *message;
};

#endif