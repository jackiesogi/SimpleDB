#include "list.h"

// Todo : 改善 bad naming 和 refactor Node_List 結構
struct List_Connection* initNodeListConnection()
{
	struct List_Connection *list = (struct List_Connection*)calloc(1, sizeof(struct List_Connection));
	return list;
}