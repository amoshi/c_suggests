#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <ctype.h>
#define MAX_LEN 10000
#define TAB_SIZE 1000LL

typedef struct Item
{
	int64_t release;
	char *string;
	struct Item *next;
} Item;

typedef struct Node
{
	int64_t key;
	Item *info;
} Node;

typedef struct Table
{
	size_t n;
	size_t m;
	Node *node;
} Table;

int64_t getcell(Table *table, int64_t key)
{
	Node *node = table->node;
	int64_t n = table->n;
	int64_t i = 0;
	int64_t m = n-1;
	int64_t j = 0;
	while ( i<=m )
	{
		j = (i+m)/2;
		if ( node[j].key > key )
			m = j - 1;
		else if ( node[j].key < key )
			i = j + 1;
		else
			break;
	}
	return j;
}

int64_t find(Table *table, int64_t key)
{
	Node *node = table->node;
	int64_t n = table->n;
	int64_t i = 0;
	int64_t m = n-1;
	int64_t j = 0;
	while ( i<=m )
	{
		j = (i+m)/2;
		if ( node[j].key > key )
			m = j - 1;
		else if ( node[j].key < key )
			i = j + 1;
		else
		{
			return j;
			//break;
		}
	}
	return -1;
}

Item* find_release(Table *table, int64_t key, int64_t release)
{
	int64_t ind = find (table, key);
	if ( ind < -1 )
	{
		return NULL;
	}
	Node *node = table->node;
	Item *item = node[ind].info;
	while ( item )
	{
		if ( item->release == release )
			break;
		item = item->next;
	}
	return item;
}

int delitem(Table *table, int64_t key, int64_t release)
{
	int64_t ind;
	Node *node = table->node;
	if ( ( ind = find(table, key) ) < -1 )
	{
		printf("key %"PRId64" not found\n", key);
		return 0;
	}
	Item *cur = node[ind].info;
	if ( cur->next && cur->release != release)
	{
		while ( cur->next && cur->release != release )
			cur = cur->next;
		if ( cur->release != release )
		{
			printf("key %"PRId64" release %"PRId64" not found\n", key, release);
			return 0;
		}
		free(cur->next->string);
		Item *bkp = cur->next;
		if ( cur->next->next )
			cur->next = cur->next->next;
		else
			cur->next = NULL;
		free(bkp);
		return 0;
	}
	else if ( cur->next && cur->release == release )
	{
		free(cur->string);
		node[ind].info = cur->next;
		free(cur);
		return 1;
	}
	else if ( cur->release == release )
	{
		free(cur->string);
		free(node[ind].info);
		int64_t i;
		printf("i=%zu ind=%"PRId64"\n", table->n, ind);
		for (i=table->n-1; i>ind; i--)
		{
			printf("move %"PRId64" to %"PRId64"\n", i, i-1);
			node[i-1].info = node[i].info;
			node[i-1].key = node[i].key;
		}
		table->n -= 1;
		return 1;
	}
	else
	{
		printf("Key %"PRId64" release %"PRId64" not found\n", key, release);
		return 0;
	}
}

void insert(Table *table, int64_t key, char *string)
{
	int64_t ind;
	Node *node = table->node;
	if ( ( ind = find(table, key) ) > -1 )
	{
		Item *cur = node[ind].info;
		int64_t release = 1;
		if ( cur->next ) {
		while ( cur->next )
		{
			cur = cur->next;
			release=cur->release;
			printf("release: %"PRId64", crelease: %"PRId64"\n", release, cur->release);
		}
		release++; }
		cur->next = malloc(sizeof (Item));
		cur = cur->next;
		size_t len = strlen(string);
		cur->string = malloc (len);
		strlcpy(cur->string,string,len);
		cur->release = release;
		cur->next = NULL;
	}
	else
	{
		if ( table->n+1 >= TAB_SIZE )
		{
			printf("Table is full: %zu/%lld cells used\n", table->n, TAB_SIZE);
			return;
		}
		ind = getcell (table, key);
		printf("ind=%"PRId64"\n", ind);
		if (node[ind].key < key)
			ind++;
		int64_t i;
		for ( i = table->n; i>ind; i-- )
		{
			printf("move %"PRId64" to %"PRId64"\n", i-1, i);
			node[i].info = node[i-1].info;
			node[i].key = node[i-1].key;
		}
		node[i].info = malloc(sizeof(Item));
		size_t len = strlen(string);
		node[i].info->string = malloc (len);
		strlcpy(node[i].info->string,string,len);
		node[i].info->release = 0;
		node[i].info->next = NULL;
		node[i].key = key;
		table->n += 1;
	}

	table->m += 1;
	//(*table->n)++;
}

const char *msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show", "5. Replace" };
const int NMsgs = sizeof ( msgs ) / sizeof ( msgs[0] );
int64_t d_add(Table *table)
{
	char field[MAX_LEN];
	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int64_t key = atoll(field);

	//printf("key=%"PRId64"\n", key);
	printf("info: ");
	fgets(field, MAX_LEN, stdin);
	//printf("info=%s\n", field);
	insert(table, key, field);
	return 1;
}
int64_t d_find(Table *table)
{
	int64_t ind;
	char field[MAX_LEN];

	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int64_t key = atoll(field);

	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("release: ");
		fgets(field, MAX_LEN, stdin);
	}
	int64_t release = atoll(field);
	
	Item *info = find_release(table,key,release);
	printf("\n\nResults:\n-------------\n");
	if ( info )
		printf("key: %"PRId64", release: %"PRId64" info: '%s'\n", key, info->release, info->string);
	printf("\n------\n\n");
	
	return 1;
}

int64_t d_replace(Table *table)
{
	int64_t ind;
	char field[MAX_LEN];

	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int64_t key = atoll(field);

	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("release: ");
		fgets(field, MAX_LEN, stdin);
	}
	int64_t release = atoll(field);
	
	Item *info = find_release(table,key,release);
	printf("\n\nResults:\n-------------\n");
	if ( info )
	{
		printf("info: ");
		fgets(field, MAX_LEN, stdin);
		printf("- key: %"PRId64", release: %"PRId64" info: '%s'\n", key, info->release, info->string);
		size_t len = strlen(field);
		if ( strlen(info->string)<len)
		{
			free(info->string);
			info->string = malloc(len);
		}
		strlcpy(info->string,field,len);
		printf("+ key: %"PRId64", release: %"PRId64" info: '%s'\n", key, info->release, info->string);
	}
	else
	{
		printf("key: %"PRId64", release: %"PRId64" not found\n", key, release);
	}
	printf("\n------\n\n");
	
	return 1;
}

int64_t d_delete(Table *table)
{
	Node *temp = NULL;
	char field[MAX_LEN];

	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int64_t key = atoll(field);

	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("release: ");
		fgets(field, MAX_LEN, stdin);
	}
	int64_t release = atoll(field);

	delitem(table, key, release);
	
	return 1;
}

int64_t show ( Table *table )
{
	int64_t i, j=0;
	Node *node = table->node;
	for ( i=0; i<table->n; i++ )
	{
		Item *item;
		for ( item = node[i].info; item; item = item->next, j++ )
			printf("key: %"PRId64" release: %"PRId64" string: %s\n", node[i].key, item->release, item->string);
	}
	return j;
}
int64_t d_show(Table *table)
{
	show (table);
	return 1;
}


int64_t deltab ( Table *table )
{
	int64_t i, j=0;
	Node *node = table->node;
	Item *bkp = NULL;
	for ( i=0; i<table->n; i++ )
	{
		Item *item;
		for ( item = node[i].info; item; item = item->next, j++ )
		{
			printf("key: %"PRId64" removed\n", node[i].key);
			free(item->string);
			free(bkp);
			bkp = item;
		}
		free(item);
	}
	return j;
}

int64_t (*fptr[])(Table *) = {NULL, d_add, d_find, d_delete, d_show, d_replace};

int dialog ( const char *msgs[], int argc)
{
	int64_t i;
	puts("============");
	for ( i=0; i<argc; i++ )
	{
		printf("| ");
		puts(msgs[i]);
	}
	puts("============");
	char field[MAX_LEN];
	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("choise: ");
		fgets(field, MAX_LEN, stdin);
	}
	i = atoll(field);
	puts("============");
	return i;
}

int main()
{
	Table *table = calloc(sizeof(Table), 1);
	table->node = calloc(sizeof(Node), TAB_SIZE);
	int rc;
	while ( (rc = dialog(msgs, NMsgs)) )
		if ( fptr[rc])
			if ( !fptr[rc](table) )
				break;
	printf("OK\n");
	deltab(table);
	free(table);
}
