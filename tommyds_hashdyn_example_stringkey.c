#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tommy.h"
typedef struct object {
	tommy_node node;
	char *key;
} object;

int compare(const void* arg, const void* obj)
{
	//return *(const int*)arg != ((const struct object*)obj)->value;
	char *s1 = (char*)arg;
	char *s2 = ((object*)obj)->key;
	return strcmp(s1, s2);
}

void foreach(void* arg)
{
	object *obj = arg;
	printf("for element %s\n", obj->key);
}

int main()
{
	// init
	tommy_hashdyn hashtable;
	tommy_hashdyn_init(&hashtable);

	object *new = malloc(sizeof(*new));
	new->key = strdup("mykey");
	

	// insert
	tommy_hashdyn_insert(&hashtable, &new->node, new, tommy_strhash_u32(0, new->key));

	// get count
	printf("count is %d\n", tommy_hashdyn_count(&hashtable));

	// foreach
	tommy_hashdyn_foreach(&hashtable, foreach);

	// search
	char* value_to_find = strdup("mykey");
	object* obj = tommy_hashdyn_search(&hashtable, &compare, value_to_find, tommy_strhash_u32(0, value_to_find));
	if (!obj)
	{
		printf("not found\n");
	}
	else
	{
		printf("found\n");
	}

	// freeing
	obj = tommy_hashdyn_remove(&hashtable, compare, value_to_find, tommy_strhash_u32(0, value_to_find));
	if (obj)
	{
		free(obj); // frees the object allocated memory
	}
	tommy_hashdyn_done(&hashtable);
	return EXIT_SUCCESS;
}
