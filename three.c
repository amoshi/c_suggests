#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_LEN 10000

typedef struct Node
{
	int key;
	char *info;
	struct Node *left;
	struct Node *right;
	struct Node *ptr;
} Node;

const char *msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show", "5. Bypass" };
const int NMsgs = sizeof ( msgs ) / sizeof ( msgs[0] );

Node *search (Node *proot, int key)
{
	Node *ptr = proot;
	if (!proot)
	{
		return NULL;
	}
	printf("enter proot key %d\n", proot->key);

	if(proot->key == key)
		return proot;
	ptr = (key < proot->key) ?  proot->left : proot->right;
	return search(ptr, key);

}

Node* erase(Node *node, int key)
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

Node *get_rev_next(Node *proot)
{
	return NULL;
}

Node *instree (Node **proot, Node *newnode)
{
	Node **ptr = proot;
	if(!*proot){
		*proot = newnode;
		return newnode;
	}

	if((*proot)->key == newnode->key)
		return NULL;
	ptr = (newnode->key < (*proot)->key) ?  &(*proot)->left : &(*proot)->right;
	
	return instree(ptr, newnode);

}

void rev_normalize(Node *proot, Node *next, Node *par)
{
	Node *left = NULL;
	int flag = 0;
	if ( proot->left )
	{
		puts("10");
		if ( proot->right )
			rev_normalize(proot->left, proot->right, proot);
		else
			rev_normalize(proot->left, proot, proot);
	}
	else if ( proot->right )
	{
		left = proot->left;
		flag = 1;
	}
	if ( proot->right && !flag )
	{
		printf("trying %d\n", proot->key);
		rev_normalize(proot->right, proot, proot);
	}
	else if (proot->right && flag )
		rev_normalize(proot->right, par->left, proot);
	if ( next )
		printf("set thread for %d to %d\n", proot->key, next->key);
	else
		printf("passing %d\n", proot->key);
	proot->ptr = next;
}

void linker(Node *proot)
{
	if (!proot || !(proot->left) || !(proot->right) )
		return;
	Node *ptr = proot->left;

	Node *rptr = proot->right;
	while ( (rptr->right) || (rptr->left) )
	{
		if (rptr->left)
			rptr = rptr->left;
		else if (rptr->right)
			rptr = rptr->right;
	}

	printf("linker set %d to %d\n", ptr->key, rptr->key);
	ptr->ptr = rptr;
}

void rrev_normalize(Node *proot, Node *next)
{
	if ( proot->left )
	{
		puts("1");
		if ( proot->right )
			rev_normalize(proot->left, proot->right, proot);
		else
			rev_normalize(proot->left, proot, proot);
	}
	linker(proot);
	if ( proot->right )
	{
		puts("1");
		
		rev_normalize(proot->right, proot, proot);
	}
	if ( next )
		printf("\tset thread for %d to %d\n", proot->key, next->key);
	else
		printf("\tpassing %d\n", proot->key);
	proot->ptr = next;
}

Node *insert(Node **proot, int k, char *in)
{
	Node *cur, *ptr;
	size_t inl = strlen(in);
	cur = (Node*)malloc(sizeof(Node));
	cur->info = (char*)malloc(inl+1);
	if(!cur)
		return NULL;
	cur->key = k;
	strncpy(cur->info,in,inl);
	cur->info[inl]=0;
	cur->left = cur->right = NULL;
	if(!(ptr = instree(proot, cur)))
	{
		puts("this key already in used");
		free(cur->info);
		free(cur);
	}
	else
	{
		//cur->ptr = get_rev_next(*proot, cur->key);
		rrev_normalize(proot[0], NULL);
	}
	return ptr;
}


int d_add(Node *a)
{
	char field[MAX_LEN];
	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int key = atoll(field);

	//printf("key=%d\n", key);
	printf("info: ");
	fgets(field, MAX_LEN, stdin);
	//printf("info=%s\n", field);
	insert(&(a->left), key, field);
	return 1;
}
int d_find(Node *a)
{
	Node *res = NULL;
	char field[MAX_LEN];
	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int key = atoll(field);
	
	if ( a->left )
		res = search(a->left, key);
	if ( a->right )
		res = search(a->right, key);
	printf("\n\nResults:\n-------------\n");
	if ( res )
		printf("key: %d, info: '%s'\n", res->key, res->info);
	printf("\n------\n\n");
	
	return 1;
}
int d_delete(Node *a)
{
	Node *temp = NULL;
	char field[MAX_LEN];
	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int key = atoll(field);
	
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
	//printf("\n\nResults:\n-------------\n");
	//if ( temp )
		//printf("delete key: success");
	//else
		//printf("delete key: no key");
	//printf("\n------\n\n");
	
	return 1;
}

int show ( Node *ptr, int n )
{
	n++;
	if ( ptr->right )
	{
		n = show(ptr->right, n);
	}
	if ( ptr )
	{
		int i;
		for ( i=0; i<n; i++, printf("\t") );
		if ( ptr->ptr )
			printf("key: %d, info: '%s', ptr: %d\n", ptr->key, ptr->info, ptr->ptr->key );
		else
			printf("key: %d, info: '%s'\n", ptr->key, ptr->info );
	}
	if ( ptr->left )
	{
		n = show(ptr->left, n);
	}
	n--;
	return n;
}
int d_show(Node *a)
{
	int i;
	if ( a->left )
		show(a->left, 0);
	if ( a->right )
		show(a->right, 0);
	return 1;
}

int d_bypass(Node *a)
{
	int i;
	Node *ptr = a;
	while (ptr->left || ptr->right)
	{
		if ( ptr->left )
		{
			ptr = ptr->left;
		}
		else if ( ptr->right )
		{
			ptr = ptr->right;
		}
	}

	while ( ptr )
	{
		printf("key = %d, info='%s'\n",ptr->key, ptr->info);
		ptr = ptr->ptr;
	}
	return 1;
}

void freetab(Node *proot)
{
	Node *ptr = proot;
	if (!proot)
	{
		return;
	}
	printf("deleting key from memory %d\n", proot->key);

	freetab(proot->left);
	freetab(proot->right);
	free(proot->info);
	free(proot);
}

int delTable(Node *a)
{
	if ( a->left )
		freetab(a->left);
	if ( a->right )
		freetab(a->right);
	return 0;
}

int (*fptr[])(Node *) = {NULL, d_add, d_find, d_delete, d_show, d_bypass};

int dialog ( const char *msgs[], int argc)
{
	int i;
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

void file_input(Node *tree, char *file)
{

	FILE *fd = fopen(file, "r");
	if ( !fd ) 
		return;

	char field[MAX_LEN];
	unsigned int i;
	int key;
	for ( i=0; fgets(field, MAX_LEN, fd); i++ )
	{
		//printf("-------\nfield = '%s'\n", field);
		int key = atoll(field);
		//printf("key %"d64" from %s\n", key, field);
		fgets(field, MAX_LEN, fd);
		size_t len = strlen(field);
		field[len-1] = 0;
		//printf("key %"d64", info: '%s'\n", key, field);
		char *buf = (char*)malloc ( len );
		strncpy(buf, field, len);
		buf[len] = 0;
		insert ( &(tree->left), key, buf );
	}
	printf("%u keys loaded from file %s\n", i, file);

	fclose(fd);
}

int main(int argc, char **argv)
{
	Node a;
	a.right=NULL;
	a.left=NULL;
	if ( argc > 1 )
		file_input(&a, argv[1]);
	int rc;
	while ( (rc = dialog(msgs, NMsgs)) )
		if ( fptr[rc])
			if ( !fptr[rc](&a) )
				break;
	printf("OK\n");
	delTable(&a);
}
