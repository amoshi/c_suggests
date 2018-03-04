#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <ctype.h>
#define MAX_LEN 10000

typedef struct Node
{
	int64_t key;
	char *info;
	struct Node *left;
	struct Node *right;
} Node;

typedef struct Table
{
	struct Node *left;
	struct Node *right;
	size_t n;
} Table;

const char *msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show" };
const int NMsgs = sizeof ( msgs ) / sizeof ( msgs[0] );

Node *search (Node *proot, int64_t key)
{
	Node *ptr = proot;
	if (!proot)
	{
		/* дерево пусто */
		return NULL;
	}
	printf("enter proot key %lld\n", proot->key);

	/* выбор нужного поддерева */
	if(proot->key == key)
		return proot;	/* элемент с таким ключом в таблице – дереве есть */
	ptr = (key < proot->key) ?  proot->left : proot->right;
	/* рекурсивный вызов функции включения нового элемента в выбранное
	* поддерево */
	return search(ptr, key);

}

Node *parentsearch (Node *proot, int64_t key)
{
	Node *ptr = proot;
	if (!proot)
	{
		return NULL;
	}
	printf("enter proot key %lld\n", proot->key);

	/* выбор нужного поддерева */
	if( (proot->left->key == key) || (proot->right->key == key) )
		return proot;
	ptr = (key < proot->key) ?  proot->left : proot->right;
	/* рекурсивный вызов функции включения нового элемента в выбранное
	* поддерево */
	return search(ptr, key);

}




Node* erase(Node *node, int64_t key)
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
 
		free(node->info);
		free(node);
		return tmp;
	}
	else if (key < node->key)
		node->left  = erase(node->left, key);
	else
		node->right = erase(node->right, key);
	return node;
}

Node *instree (Node **proot, Node *newnode)
{
	Node **ptr = proot;
	if(!*proot){
		/* дерево пусто */
		*proot = newnode;
		return newnode;
	}

	/* выбор нужного поддерева */
	if((*proot)->key == newnode->key)
		return NULL;	/* элемент с таким ключом в таблице – дереве есть */
	ptr = (newnode->key < (*proot)->key) ?
	&(*proot)->left : &(*proot)->right;
	/* рекурсивный вызов функции включения нового элемента в выбранное
	* поддерево */
	return instree(ptr, newnode);

}

/* Функция включения нового элемента в таблицу. Результат –NULL, если
* элемент не может быть включен в таблицу, и указатель на новый элемент
* в противном случае
*/

Node *insert(Node **proot, int64_t k, char *in)
{
	Node *cur, *ptr;
	size_t inl = strlen(in);
	/* создание нового элемента таблицы – дерева */
	cur = malloc(sizeof(Node));
	cur->info = malloc(inl+1);
	if(!cur)
		return NULL;	/* нет свободной памяти */
	cur->key = k;
	//cur->info = dupl(in);
	strlcpy(cur->info,in,inl);
	cur->left = cur->right = NULL;
	/* включение нового элемента в дерево */
	if(!(ptr = instree(proot, cur)))
	{
		puts("this key already in used");
		free(cur->info);
		free(cur);
	}
	return ptr;
}


int64_t d_add(Table *a)
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
	insert(&(a->left), key, field);
	return 1;
}
int64_t d_find(Table *a)
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
	
	if ( a->left )
		res = search(a->left, key);
	if ( a->right )
		res = search(a->right, key);
	printf("\n\nResults:\n-------------\n");
	if ( res )
		printf("key: %"PRId64", info: '%s'\n", res->key, res->info);
	printf("\n------\n\n");
	
	return 1;
}
int64_t d_delete(Table *a)
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
		a->left = erase(a->left, key);
		temp = a->left;
	}
	if ( a->right )
	{
		a->right = erase(a->right, key);
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

int show ( Node *ptr )
{
	if ( ptr )
	{
		printf("key: %"PRId64", info: '%s'\n", ptr->key, ptr->info );
	}
	if ( ptr->left )
	{
		show(ptr->left);
	}
	if ( ptr->right )
	{
		show(ptr->right);
	}
	return 1;
}
int64_t d_show(Table *a)
{
	int64_t i;
	if ( a->left )
		show(a->left);
	if ( a->right )
		show(a->right);
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
	free(proot->info);
	free(proot);
	/* рекурсивный вызов функции включения нового элемента в выбранное
	* поддерево */
}

int64_t delTable(Table *a)
{
	if ( a->left )
		freetab(a->left);
	if ( a->right )
		freetab(a->right);
	return 0;
}

int64_t (*fptr[])(Table *) = {NULL, d_add, d_find, d_delete, d_show};

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

int main()
{
	Table a;
	int rc;
	while ( (rc = dialog(msgs, NMsgs)) )
		if ( !fptr[rc](&a) )
			break;
	printf("OK\n");
	delTable(&a);
}
