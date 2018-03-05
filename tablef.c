#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <ctype.h>
#include <unistd.h>
#define MAX_LEN 10000

typedef struct Node
{
	int64_t key;
	int64_t offset;
	size_t len;
	struct Node *left;
	struct Node *right;
} Node;

const char *msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show" };
const int NMsgs = sizeof ( msgs ) / sizeof ( msgs[0] );

char *getInfo(char *ffile, Node *ptr)
{
	printf("ptr->len='%zu' ptr->key=%"PRId64"\n", ptr->len, ptr->key);
	if ( ptr->len < 1 )
		return NULL;
	printf("ptr=%p\n", ptr);
	printf("ptr->len=%zu\n", ptr->len);
	printf("ptr->offset=%"PRId64"\n", ptr->offset);
	FILE *fd = fopen( ffile, "r+b" );
	if ( !fd )
		return NULL;
	char *str = malloc ( ptr->len );
	fseek(fd, ptr->offset, SEEK_SET);
	fread(str, 1, ptr->len, fd);
	fclose(fd);
	return str;
}

Node *letoffset (Node *proot, int64_t key, int64_t changeoffset)
{
	Node *ptr = proot;
	printf("proot=%p\n", proot);
	if (!proot)
	{
		return NULL;
	}
	printf("ENTR proot key %lld\n", proot->key);

	if(proot->key > key) { printf("ENTR: change offset from %"PRId64" to %"PRId64" ( changeoffset -%"PRId64")\n", proot->offset, proot->offset-changeoffset, changeoffset );
		proot->offset -= changeoffset; }

	if ( proot->left )
		letoffset(proot->left, key, changeoffset);
	if ( proot->right )
		letoffset(proot->right, key, changeoffset);
	return proot;
}

Node *search (Node *proot, int64_t key)
{
	//printf("testing %p with key %"PRId64" (finding %"PRId64")\n", proot, proot->key, key);
	Node *ptr = proot;
	if (!proot)
	{
		/* дерево пусто */
		return NULL;
	}
	printf("1enter proot key %lld\n", proot->key);

	/* выбор нужного поддерева */
	if(proot->key == key)
		return proot;	/* элемент с таким ключом в таблице – дереве есть */
	ptr = (key < proot->key) ?  proot->left : proot->right;
	/* рекурсивный вызов функции включения нового элемента в выбранное
	* поддерево */
	return search(ptr, key);

}

int64_t file_offset_truncate(char *ffile, int64_t offset, int64_t len)
{
	FILE *fd = fopen(ffile, "r+b");
	if ( !fd )
		return -1;
	char *str = malloc(MAX_LEN);
	int64_t rc;
	int64_t bytes = 0;
	do
	{
		printf("set offset %"PRId64"\n", offset+len);
		fseek(fd, offset+len, SEEK_SET);
		rc = fread(str, 1, MAX_LEN, fd);
		printf("readed %"PRId64" bytes with text %s\n", rc, str);
		bytes += rc;
		rewind(fd);
		printf("set offset %"PRId64"\n", offset);
		fseek(fd, offset, SEEK_SET);
		fwrite(str, 1, rc, fd);
		offset += rc;
	}
	while ( rc==MAX_LEN );
	ftruncate(fileno(fd), offset-1);
	fclose(fd);
	return bytes;
}

Node* erase(Node *node, int64_t key, char *ffile, Node *table)
{
	if(node == NULL)
		return node;
 
	if(key == node->key){
		Node* tmp;
		if(node->right == NULL)
			tmp = node->left;
		else
		{
 
			Node* ptr = node->right;
			if(ptr->left == NULL)
			{
				ptr->left = node->left;
				tmp = ptr;
			}
			else
			{
 
				Node* pmin = ptr->left;
				while(pmin->left != NULL)
				{
					ptr  = pmin;
					pmin = ptr->left;
				}
				ptr->left   = pmin->right;
				pmin->left  = node->left;
				pmin->right = node->right;
				tmp = pmin;
			}
		}
 
		file_offset_truncate(ffile, node->offset, node->len);
		letoffset(table, node->key, node->offset+node->len);
		free(node);
		return tmp;
	}
	else if (key < node->key)
		node->left  = erase(node->left, key, ffile, table);
	else
		node->right = erase(node->right, key, ffile, table);
	return node;
}

Node *instree (Node **node, Node *newnode)
{
	printf("add key='%"PRId64"', len='%zu', offset=%"PRId64"\n", newnode->key, newnode->len, newnode->offset);
	if(!(*node)){
		/* дерево пусто */
		printf("add newnode with address %p: key=%"PRId64" len=%zu, offset=%"PRId64"\n", newnode, newnode->key, newnode->len, newnode->offset);
		*node = newnode;
		return newnode;
	}

	/* выбор нужного поддерева */
	if((*node)->key == newnode->key)
		return NULL;	/* элемент с таким ключом в таблице – дереве есть */
	Node **ptr;
	ptr = (newnode->key < (*node)->key) ?
	&(*node)->left : &(*node)->right;
	/* рекурсивный вызов функции включения нового элемента в выбранное
	* поддерево */
	return instree(ptr, newnode);

}

/* Функция включения нового элемента в таблицу. Результат –NULL, если
* элемент не может быть включен в таблицу, и указатель на новый элемент
* в противном случае
*/

int insfile(char *ffile, Node *node, char *info)
{
	FILE *fd = fopen(ffile, "ab");
	if (!fd)
	{
		printf("cannot openfile %s\n", ffile);
		return -1;
	}
	fseek(fd, 0L, SEEK_END);
	node->offset = ftell(fd);
	int rc = fwrite(info, 1, node->len, fd);
	printf("put %s with size %zu to file %s, rc = %d\n", info, node->len, ffile, rc);
	fclose(fd);
	return 1;
}

Node *insert(Node **node, int64_t k, char *in, char *ffile)
{
	Node *cur, *ptr;
	size_t inl = strlen(in);
	cur = malloc(sizeof(Node));
	if(!cur)
		return NULL;
	cur->key = k;
	cur->left = cur->right = NULL;
	cur->len = strlen(in);
	if(!(ptr = instree(node, cur)))
	{
		puts("this key already in used");
	}
	if ( !insfile(ffile, ptr, in) )
	{
		printf("cannot write to file %s\n", ffile);
	}
	return ptr;
}


Node *stupinsert(Node **node, Node *newnode)
{
	printf("ADD: %p\n", *node);
	if(!newnode)
		return NULL;
	newnode->left = newnode->right = NULL;
	Node *ptr;
	if(!(ptr = instree(node, newnode)))
	{
		puts("this key already in used");
	}
	return ptr;
}


int64_t d_add(Node *a, char *ffile)
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
	field[strlen(field)-1]=0;
	insert(&(a), key, field, ffile);
	return 1;
}
int64_t d_find(Node *a, char *ffile)
{
	Node *res = NULL;
	char field[MAX_LEN];
	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int64_t key = atoll(field);
	
	res = search(a, key);
	printf("\n\nResults:\n-------------\n");
	if ( res )
	{
		char *str = getInfo(ffile, res);
		if ( !str )
			return -1;
		printf("key: %"PRId64", info: '%s'\n", res->key, str);
		free(str);
	}
	printf("\n------\n\n");
	
	return 1;
}
int64_t d_delete(Node *a, char *ffile)
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
	
	if ( a->left )
	{
		a->left = erase(a->left, key, ffile, a->left);
		temp = a->left;
	}
	if ( a->right )
	{
		a->right = erase(a->right, key, ffile, a->right);
		temp = a->right;
	}
	printf("\n\nResults:\n-------------\n");
	if ( temp )
		printf("delete key: success");
	else
		printf("delete key: no key");
	printf("\n------\n\n");
	
	return 1;
}

int64_t writeTable ( Node *ptr, FILE *fd )
{
	int64_t i = 0;
	if ( ptr )
	{
		printf("write key=%"PRId64", len=%zu, offset=%"PRId64" to file\n", ptr->key, ptr->len, ptr->offset);
		int rc = 0;
		rc += fwrite(&ptr->key, sizeof(int64_t), 1, fd);
		rc += fwrite(&ptr->len, sizeof(int64_t), 1, fd);
		rc += fwrite(&ptr->offset, sizeof(int64_t), 1, fd);
		printf("writed %d bytes\n", rc);
		i++;
	}
	if ( ptr->left )
	{
		i += writeTable(ptr->left, fd);
	}
	if ( ptr->right )
	{
		i += writeTable(ptr->right, fd);
	}
	return i;
}


int show ( Node *ptr, char *ffile )
{
	printf("showing tree with adddress %p\n", ptr);
	if ( ptr )
	{
		char *str = getInfo(ffile, ptr);
		if ( str )
		{
			printf("key: %"PRId64", info: '%s'\n", ptr->key, str );
			free(str);
		}
	}
	if ( ptr->left )
	{
		show(ptr->left, ffile);
	}
	if ( ptr->right )
	{
		show(ptr->right, ffile);
	}
	return 1;
}
int64_t d_show(Node *a, char *ffile)
{
	int64_t i;
	show(a, ffile);
	return 1;
}

void freetab(Node *proot)
{
	Node *ptr = proot;
	if (!proot)
	{
		/* дерево пусто */
		return;
	}
	printf("deleting key %lld\n", proot->key);

	freetab(proot->left);
	freetab(proot->right);
	free(proot);
	/* рекурсивный вызов функции включения нового элемента в выбранное
	* поддерево */
}

int64_t delTable(Node *a)
{
	if ( a->left )
		freetab(a->left);
	if ( a->right )
		freetab(a->right);
	return 0;
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

int64_t (*fptr[])(Node *,  char *) = {NULL, d_add, d_find, d_delete, d_show};

int dialog ( const char *msgs[], int argc, char *ffile)
{
	int64_t i;
	puts("============");
	for ( i=0; i<argc; i++ )
	{
		printf("| ");
		puts(msgs[i]);
	}
	puts("============");
	//scanf("%d", &i);
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

Node* startup(char *ftab)
{
	size_t size;
	FILE *fd = fopen(ftab, "r+b");
	if (!fd)
	{
		printf("Cannot open file %s\n", ftab);
		return malloc(sizeof(Node));
	}
	printf("startup: position: %ld\n", ftell(fd));
	fread(&size, sizeof(int64_t),1,fd);
	printf("size=%zu\n", size);
	Node *root = NULL;
	//root = calloc(1, sizeof(Node));
	while ( size-- != 0 )
	{
		Node *node;
		node = calloc(1, sizeof(Node));
		printf("startup: position: %ld\n", ftell(fd));
		fread(&node->key, sizeof(int64_t),1,fd);
		printf("key=%"PRId64"\n", node->key);
		printf("startup: position: %ld\n", ftell(fd));
		fread(&node->len, sizeof(int64_t),1,fd);
		printf("len=%zu\n", node->len);
		printf("startup: position: %ld\n", ftell(fd));
		fread(&node->offset, sizeof(int64_t),1,fd);
		printf("offset=%"PRId64"\n", node->offset);
		printf("trying loading key=%"PRId64", len=%zu, offset=%"PRId64"\n", node->key, node->len, node->offset);
		stupinsert(&root, node);
	}
	printf("returning struct with addr %p", root);
	return root;
}

int64_t shutdown(char *ftab, Node *node)
{
	size_t size = 0;
	Node *ptr;
	FILE *fd = fopen(ftab, "w+b");
	if (!fd)
	{
		printf("cannot write table to file %s\n", ftab);
		return -1;
	}
	fwrite(&size, sizeof(int64_t),1,fd);
	size += writeTable( node, fd );
	fseek(fd, 0, SEEK_SET);
	fwrite(&size, sizeof(int64_t),1,fd);
	printf("closing file %s\n", ftab);
	fclose(fd);
	return size;
}

int main()
{
	int rc;
	char *ffile = malloc (FILENAME_MAX);
	char *ftab = malloc (FILENAME_MAX);
	getfilename(ffile, ftab);
	Node *a = startup(ftab);
	printf("\n\n\n\n\n\n\n\n");
	//show(a,"1");
	printf("address of node is %p\n", a);
	while ( (rc = dialog(msgs, NMsgs, ffile)) )
		if (fptr[rc])
			if ( !fptr[rc](a, ffile) )
				break;
	shutdown(ftab, a);
	printf("OK\n");
	delTable(a);
}
