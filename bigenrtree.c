#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#define MAX_LEN 10000

typedef struct Node
{
	int key; // ключ
	char *info; // информация
	struct Node *left; // левая ветка
	struct Node *right; // правая ветка
} Node;

const char *msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show", "5. Min", "6. Reverse", "7. Show tree" };
const int NMsgs = sizeof ( msgs ) / sizeof ( msgs[0] );

// рекурсивный поиск элемента
Node *search (Node *proot, int key)
{
	Node *ptr = proot;
	if (!proot)
	{
		return NULL;
	}
	//printf("enter proot key %d\n", proot->key);

	if(proot->key == key)
		return proot;
	ptr = (key < proot->key) ?  proot->left : proot->right;
	return search(ptr, key);

}

// поиск и удаление элемента по ключу
Node* erase(Node *node, int key)
{
	// пустой элемент - завершить функцию, вернуть NULL
	if(node == NULL)
		return node;
 
	if(key == node->key)
	{
		Node* tmp;
		// если есть только левый дочерний элемент - ставим его вместо удаляемого
		if(node->right == NULL)
			tmp = node->left;
		else
		{
			Node* ptr = node->right;
			// если есть только правый дочерний элемент - ставим его вместо удаляемого
			if(ptr->left == NULL)
			{
				ptr->left = node->left;
				tmp = ptr;
			}
			else
			{
				// проходим по левым элементам рекурсивно по дереву, пока не упремся в NULL
				Node* pmin = ptr->left;
				while(pmin->left != NULL)
				{
					ptr  = pmin;
					pmin = ptr->left;
				}

				// левому крылу удаляемого элемента присваиваем правый узел конечного элемента, а левый и правый узел удаляемого элемента переводим в левый и првый элемент конечного элемента левого поддерева
				ptr->left   = pmin->right;
				pmin->left  = node->left;
				pmin->right = node->right;
				tmp = pmin;
			}
		}
 
		// высвобождаем память
		free(node->info);
		free(node);
		return tmp;
	}
	// рекурсивный обход дерева до нахождения удаляемого элемента
	else if (key < node->key)
		node->left  = erase(node->left, key);
	else
		node->right = erase(node->right, key);
	return node;
}

// рекурсивная функция вставки
Node *instree (Node **proot, Node *newnode)
{
	Node **ptr = proot;
	if(!*proot){
		*proot = newnode;
		return newnode;
	}

	if((*proot)->key == newnode->key)
	{
		printf("key %d already in used\n", newnode->key);
		return NULL;
	}
	ptr = (newnode->key < (*proot)->key) ?  &(*proot)->left : &(*proot)->right;
	return instree(ptr, newnode);

}

// функция вставки в дерево
Node *insert(Node **proot, int k, char *in)
{
	Node *cur, *ptr;
	size_t inl = strlen(in);		// length info
	cur = (Node*)malloc(sizeof(Node));	// allocate node
	cur->info = (char*)malloc(inl+1);	// allocate info
	if(!cur)
		return NULL;
	cur->key = k;				// set key
	strncpy(cur->info,in,inl);		// copy info to node
	cur->info[inl]='\0';
	cur->left = cur->right = NULL;		// set leaf
	if (!(proot[0]))
	{
		proot[0] = cur;
		return cur;
	}
	if(!(ptr = instree(proot, cur)))	// worker insert to tree
	{
		free(cur->info);
		free(cur);
	}
	return ptr;
}


int d_add(Node **a)
{
	char field[MAX_LEN];
	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int key = atoll(field);

	printf("info: ");
	fgets(field, MAX_LEN, stdin);
	field[strlen(field)-1]=0;
	insert(a, key, field);
	return 1;
}
int d_find(Node **node)
{
	Node *a = *node;
	Node *res = NULL;
	char field[MAX_LEN];
	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int key = atoll(field);
	
	res = search(a,key);
	printf("\n\nResults:\n-------------\n");
	if ( res )
	{
		printf("key: %d, info: '%s'\n", res->key, res->info);
	}
	printf("\n------\n\n");
	
	return 1;
}
int d_delete(Node **node)
{
	Node *a = *node;
	Node *temp = NULL;
	char field[MAX_LEN];
	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int key = atoll(field);

	erase(a,key);
	
	return 1;
}

void rb_getmin(Node *x)
{
	while ( x )
	{
		if ( x->left && x->left->key < x->key )
			x = x->left;
		else if ( x->right && x->right->key < x->key )
			x = x->right;
		else
		{
			printf("min: %d, info: '%s'\n", x->key, x->info);
			return;
		}
	}
}

void show ( Node *ptr )
{
	if ( !ptr )
		return;
	if ( ptr->right )
	{
		show(ptr->right);
	}
	if ( ptr )
	{
		int i;
		printf("key: %d, info: '%s'\n", ptr->key, ptr->info);
	}
	if ( ptr->left )
	{
		show(ptr->left);
	}
}
int d_show(Node **node)
{
	show(*node);
	return 1;
}

int showtree ( Node *ptr, int n )
{
	n++;
	if ( !ptr )
		return 0;
	if ( ptr->right )
	{
		n = showtree(ptr->right, n);
	}
	if ( ptr )
	{
		int i;
		for ( i=0; i<n; i++, printf("\t") );
		printf("key: %d, info: '%s'\n", ptr->key, ptr->info);
	}
	if ( ptr->left )
	{
		n = showtree(ptr->left, n);
	}
	n--;
	return n;
}
int d_showtree(Node **node)
{
	Node *a = *node;
	int i;
	//if ( a->left )
	//	showtree(a->left, 0);
	//if ( a->right )
	//	showtree(a->right, 0);
	showtree(a,-1);
	return 1;
}

void rev_show ( Node *ptr )
{
	if ( !ptr )
		return;
	rev_show(ptr->left);
	rev_show(ptr->right);

	printf("key: %d, info: '%s'\n", ptr->key, ptr->info);
}
int d_rev_show(Node **node)
{
	Node *a = *node;
	rev_show(a);
	return 1;
}

int d_min(Node **node)
{
	rb_getmin(*node);
	return 1;
}

void freetab(Node *proot)
{
	if (!proot)
	{
		return;
	}

	freetab(proot->left);
	freetab(proot->right);

	printf("deleting key from memory %d\n", proot->key);
	free(proot->info);
	free(proot);
}

int delTable(Node *a)
{
	if (a)
		freetab(a);
	return 0;
}

int (*fptr[])(Node **) = {NULL, d_add, d_find, d_delete, d_show, d_min, d_rev_show, d_showtree};

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

void file_input(Node **tree, char *file)
{

	FILE *fd = fopen(file, "r");
	if ( !fd ) 
		return;

	char field[MAX_LEN];
	unsigned int i;
	int key;
	for ( i=0; fgets(field, MAX_LEN, fd); i++ )
	{
		int key = atoll(field);
		fgets(field, MAX_LEN, fd);
		size_t len = strlen(field);
		field[len-1] = 0;
		char *buf = (char*)malloc ( len );
		strncpy(buf, field, len);
		buf[len] = 0;
		insert ( tree, key, buf );
	}
	printf("%u keys loaded from file %s\n", i, file);

	fclose(fd);
}

int main(int argc, char **argv)
{
	Node *a = NULL;
	//a.right=NULL;
	//a.left=NULL;
	if ( argc > 1 )
		file_input(&a, argv[1]);
	int rc;
	while ( (rc = dialog(msgs, NMsgs)) )
		if ( fptr[rc])
			if ( !fptr[rc](&a) )
				break;
	printf("OK\n");
	delTable(a);
}
