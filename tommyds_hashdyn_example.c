#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tommy.h"
typedef struct object {
	tommy_node node;
	int value;
} object;

int compare(const void* arg, const void* obj)
{
	return *(const int*)arg != ((const struct object*)obj)->value;
}

void foreach(void* arg)
{
	object *obj = arg;
	printf("for element %d\n", obj->value);
}

int main()
{
	// init
	tommy_hashdyn hashtable;
	tommy_hashdyn_init(&hashtable);

	object *new = malloc(sizeof(*new));
	new->value = 1;
	

	// insert
	tommy_hashdyn_insert(&hashtable, &new->node, new, tommy_inthash_u32(new->value));

	// get count
	printf("count is %d\n", tommy_hashdyn_count(&hashtable));

	// foreach
	tommy_hashdyn_foreach(&hashtable, foreach);

	// search
	int value_to_find = 1;
	object* obj = tommy_hashdyn_search(&hashtable, &compare, &value_to_find, tommy_inthash_u32(value_to_find));
	if (!obj)
	{
		printf("not found\n");
	}
	else
	{
		printf("found\n");
	}

	// freeing
	obj = tommy_hashdyn_remove(&hashtable, compare, &value_to_find, tommy_inthash_u32(value_to_find));
	if (obj)
	{
		free(obj); // frees the object allocated memory
	}
	tommy_hashdyn_done(&hashtable);
	return EXIT_SUCCESS;
}
