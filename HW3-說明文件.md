## 使用的資料結構
- 定義在[datastructure.h](./include/datastructure.h)
- 沿用KeyValue結構，但新增了一個KeyValue *prev
```c
struct KeyValue
{
	char key[32];
	char value[128];
	struct KeyValue *next;
	struct KeyValue *prev;	// for doubly linked list
};
```
- 新增了一個Node_List，紀錄doubly linked list的頭尾
```c
struct Node_List
{
	struct KeyValue *head;
	struct KeyValue *tail;
};
```