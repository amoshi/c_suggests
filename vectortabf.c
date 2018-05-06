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
	int64_t offset;
	size_t len;
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

int64_t writeTable ( Table *table, FILE *fd )
{
	int64_t i, j=0;
	Node *node = table->node;
	int rc = 0;
	for ( i=0; i<table->n; i++ )
	{
		Item *item;
		for ( item = node[i].info; item; item = item->next, j++ )
		{
			//printf("cur: %p, next: %p\n", item, item->next);
				//printf("key: %"PRId64", info: '%s'\n", ptr->key, str );
				printf("%p: key: %"PRId64" release: %"PRId64"\n", item, node[i].key, item->release);
				rc += fwrite(&node[i].key, sizeof(int64_t), 1, fd);
				rc += fwrite(&item->len, sizeof(int64_t), 1, fd);
				rc += fwrite(&item->offset, sizeof(int64_t), 1, fd);
		}
	}
	return j;
}

void getfilename ( char *ffile, char *ftab )
{
	printf("get filename of data: ");
	fgets(ffile, FILENAME_MAX, stdin);
	ffile[strlen(ffile)-1]='\0';
	printf("get filename of table: ");
	fgets(ftab, FILENAME_MAX, stdin);
	ftab[strlen(ftab)-1]='\0';
}

char *getInfo(char *ffile, Item *ptr)
{
	if ( ptr->len < 1 )
		return NULL;
	//printf("ptr=%p\n", ptr);
	//printf("ptr->len=%zu\n", ptr->len);
	//printf("ptr->offset=%"PRId64"\n", ptr->offset);
	FILE *fd = fopen( ffile, "r+b" );
	if ( !fd ) { puts("FFFAIL");
		return NULL; }
	char *str = calloc ( ptr->len, 1 );
	fseek(fd, ptr->offset, SEEK_SET);
	fread(str, 1, ptr->len, fd);
	fclose(fd);
	return str;
}

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
		node[ind].info = cur->next;
		free(cur);
		return 1;
	}
	else if ( cur->release == release )
	{
		free(node[ind].info);
		int64_t i;
		for (i=table->n-1; i>ind; i--)
		{
			//printf("move %"PRId64" to %"PRId64"\n", i, i-1);
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

int insfile(char *ffile, Item *item, char *info)
{
	FILE *fd = fopen(ffile, "ab");
	if (!fd)
	{
		printf("cannot openfile %s\n", ffile);
		return -1;
	}
	fseek(fd, 0L, SEEK_END);
	item->offset = ftell(fd);
	int rc = fwrite(info, 1, item->len, fd);
	fclose(fd);
	return 1;
}

void stupinsert(Table *table, int64_t key, size_t len, int64_t offset)
{
	int64_t ind;
	Node *node = table->node;
	if ( ( ind = find(table, key) ) > -1 )
	{
		Item *cur = node[ind].info;
		int64_t release = 1;
		if ( cur->next )
		{
			while ( cur->next )
			{
				cur = cur->next;
				release=cur->release;
				//printf("release: %"PRId64", crelease: %"PRId64"\n", release, cur->release);
			}
			release++;
		}
		cur->next = malloc(sizeof (Item));
		cur = cur->next;
		cur->release = release;
		cur->offset = offset;
		cur->len = len;
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
		if (node[ind].key < key)
			ind++;
		int64_t i;
		for ( i = table->n; i>ind; i-- )
		{
			//printf("move %"PRId64" to %"PRId64"\n", i-1, i);
			node[i].info = node[i-1].info;
			node[i].key = node[i-1].key;
		}
		node[i].info = malloc(sizeof(Item));
		node[i].info->len = len;
		node[i].info->release = 0;
		node[i].info->offset = offset;
		node[i].info->next = NULL;
		node[i].key = key;
		table->n += 1;
	}
	table->m += 1;
}

void insert(Table *table, int64_t key, char *string, char *ffile)
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
		cur->len = strlen(string);
		if ( !insfile(ffile, cur, string) )
		{
			printf("cannot write to file %s\n", ffile);
		}
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
		printf("%p:%p:%p\n", node, &node[ind], &key);
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
		node[i].info->len = strlen(string);
		if ( !insfile(ffile, node[i].info, string) )
		{
			printf("cannot write to file %s\n", ffile);
		}
		node[i].info->release = 0;
		node[i].info->next = NULL;
		node[i].key = key;
		table->n += 1;
	}

	table->m += 1;
}

const char *msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show", "5. Replace" };
const int NMsgs = sizeof ( msgs ) / sizeof ( msgs[0] );
int64_t d_add(Table *table, char *ffile)
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
	field[strlen(field)-1]=0;
	//printf("info=%s\n", field);
	insert(table, key, field, ffile);
	return 1;
}
int64_t d_find(Table *table, char *ffile)
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
		char *str = getInfo(ffile, info);
		if ( !str )
			return -1;
		//printf("key: %"PRId64", info: '%s'\n", res->key, str);
		printf("key: %"PRId64", release: %"PRId64" info: '%s'\n", key, info->release, str);
		free(str);
	}
	printf("\n------\n\n");
	
	return 1;
}

int64_t d_replace(Table *table, char *ffile)
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
		char *str = getInfo(ffile, info);

		printf("info: ");
		fgets(field, MAX_LEN, stdin);
		field[strlen(field)-1]=0;
		printf("- key: %"PRId64", release: %"PRId64" info: '%s'\n", key, info->release, str);
		free(str);
		info->len = strlen(field);
		if ( !insfile(ffile, info, field) )
		{
			printf("cannot write to file %s\n", ffile);
		}

		str = getInfo(ffile, info);
		printf("+ key: %"PRId64", release: %"PRId64" info: '%s'\n", key, info->release, str);
		free(str);
	}
	else
	{
		printf("key: %"PRId64", release: %"PRId64" not found\n", key, release);
	}
	printf("\n------\n\n");
	
	return 1;
}

int64_t d_delete(Table *table, char *ffile)
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

int64_t show ( Table *table, char *ffile )
{
	int64_t i, j=0;
	Node *node = table->node;
	for ( i=0; i<table->n; i++ )
	{
		Item *item;
		for ( item = node[i].info; item; item = item->next, j++ )
		{
			char *str = getInfo(ffile, item);
			if ( str )
			{
				printf("%p: key: %"PRId64" release: %"PRId64" string: %s\n", item, node[i].key, item->release, str);
				free(str);
			}
		}
	}
	return j;
}
int64_t d_show(Table *table, char *ffile)
{
	show (table, ffile);
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
			free(bkp);
			bkp = item;
		}
		free(item);
	}
	return j;
}

int64_t (*fptr[])(Table *, char *) = {NULL, d_add, d_find, d_delete, d_show, d_replace};

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

Table* startup(char *ftab)
{
	Table *table = calloc(sizeof(Table), 1);
	table->node = calloc(sizeof(Node), TAB_SIZE);
	size_t size;
	FILE *fd = fopen(ftab, "r+b");
	if (!fd)
	{
		printf("Cannot open file %s\n", ftab);
		return table;
	}
	//printf("startup: position: %ld\n", ftell(fd));
	fread(&size, sizeof(int64_t),1,fd);
	//table->n = size;
	while ( size-- != 0 )
	{
		int64_t key;
		int64_t offset;
		size_t len;
		//printf("startup: position: %ld\n", ftell(fd));
		fread(&key, sizeof(int64_t),1,fd);
		//printf("key=%"PRId64"\n", key);
		//printf("startup: position: %ld\n", ftell(fd));
		fread(&len, sizeof(int64_t),1,fd);
		//printf("len=%zu\n", len);
		//printf("startup: position: %ld\n", ftell(fd));
		fread(&offset, sizeof(int64_t),1,fd);
		//printf("offset=%"PRId64"\n", offset);
		printf("trying loading key=%"PRId64", len=%zu, offset=%"PRId64"\n", key, len, offset);
		stupinsert(table, key, len, offset );
	}
	printf("returning struct with addr %p", table);
	return table;
}

int64_t shutdown(char *ftab, Table *table)
{
	size_t size = 0;
	FILE *fd = fopen(ftab, "w+b");
	if (!fd)
	{
		printf("cannot write table to file %s\n", ftab);
		return -1;
	}
	fwrite(&size, sizeof(int64_t),1,fd);
	size += writeTable( table, fd );
	fseek(fd, 0, SEEK_SET);
	fwrite(&size, sizeof(int64_t),1,fd);
	printf("closing file %s\n", ftab);
	fclose(fd);
	return size;
}

int main()
{
	char *ffile = malloc (FILENAME_MAX);
	char *ftab = malloc (FILENAME_MAX);
	getfilename(ffile, ftab);
	Table *table = startup(ftab);
	//Table *table = calloc(sizeof(Table), 1);
	//table->node = calloc(sizeof(Node), TAB_SIZE);
	int rc;
	while ( (rc = dialog(msgs, NMsgs)) )
		if ( fptr[rc])
			if ( !fptr[rc](table, ffile) )
				break;
	printf("OK\n");
	shutdown(ftab, table);
	deltab(table);
	free(table);
}
