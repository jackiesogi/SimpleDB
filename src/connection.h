#ifndef CONNECTION_H
#define CONNECTION_H
struct Connection
{
	struct Table_Connection *tc;
	struct List_Connection  *lc;
};

extern void initConfigTable(const char* dbPath);

extern struct Connection* initConnection(struct Table_Connection*, struct List_Connection*);

#endif