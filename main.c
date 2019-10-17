#include "chaining_hash_table.h"

//-------Example--------

int main()
{
	Chaining_hashtable* table = CH_create(5,10,100);
	CH_add(table,"first","hello");
	CH_add(table,"second","world");
	CH_add(table,"third","everyonone");
	CH_delete_elem(table,"third");
	printf("%s\n",CH_find(table,"first"));
	printf("%s\n",CH_find(table,"second"));
	printf("%s\n",CH_find(table,"third"));
	CH_delete(table);
	return 0;
}
