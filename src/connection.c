#include "connection.h"


struct Connection* initConnection(struct Table_Connection *tconnection, struct List_Connection *lconnection)
{
	struct Connection *connection = (struct Connection*)malloc(sizeof(struct Connection));
	connection->tc = tconnection;
	connection->lc = lconnection;
	return connection;
}