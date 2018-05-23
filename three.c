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
	struct Node *ptr; // нить
} Node;

typedef struct snode
{
	Node *ptr;
	struct snode *next;
	struct snode *prev;
} snode;

typedef struct shead
{
	snode *start;
	snode *end;
} shead;

void spush(shead *stack, Node *ptr)
{
	if ( stack->start == NULL )
	{
		stack->start = (snode*)calloc(sizeof(snode),1);
		stack->start->next = NULL;
		stack->start->prev = NULL;
		stack->start->ptr = ptr;
		stack->end = stack->start;
	}
	else
	{
		stack->end->next = (snode*)calloc(sizeof(snode),1);
		stack->end->next->next = NULL;
		stack->end->next->prev = stack->end;
		stack->end = stack->end->next;
		stack->end->ptr = ptr;
	}
}

Node* spop(shead *stack)
{
	if ( !stack || !(stack->start) )
	{
		return NULL;
	}
	if ( !(stack->end->prev) )
	{
		Node *ptr = stack->start->ptr;
		free(stack->start);
		stack->start = stack->end = NULL;
		return ptr;
	}
	Node *ptr = stack->end->ptr;
	snode *stnode = stack->end;
	stack->end = stnode->prev;
	stack->end->next = NULL;

	free(stnode);
	return ptr;
}

const char *msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show", "5. Bypass", "6. Generate", "7. Smartdelete" };
const int NMsgs = sizeof ( msgs ) / sizeof ( msgs[0] );

// рекурсивный поиск элемента
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
		return NULL;
	}
	ptr = (newnode->key < (*proot)->key) ?  &(*proot)->left : &(*proot)->right;
	return instree(ptr, newnode);

}

void post_order(Node *root, shead *stack)
{
	if ( root )
	{
		post_order(root->left, stack);
		post_order(root->right, stack);
		Node *prev  = spop(stack);
		if ( prev )
			prev->ptr = root;
		spush(stack, root);
		//printf(":%d:", root->key);
		//if ( root->ptr )
		//	printf(":%d:", root->ptr->key);
		//puts("");
	}
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
	cur->ptr = NULL;
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
	else
	{
		shead *stack = (shead*)calloc(sizeof(shead),1);
		post_order(proot[0], stack);
		free(stack);
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
	
	return 1;
}

int show ( Node *ptr, int n )
{
	n++;
	if ( !ptr )
		return 0;
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
int d_show(Node **node)
{
	Node *a = *node;
	int i;
	//if ( a->left )
	//	show(a->left, 0);
	//if ( a->right )
	//	show(a->right, 0);
	show(a,-1);
	return 1;
}

int d_bypass(Node **node)
{
	Node *a = *node;
	int i;
	if ( !a )
		return 1;
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
		printf("debug (%p) key = %d, info='%s'\n", ptr, ptr->key, ptr->info);
		ptr = ptr->ptr;
	}

	return 1;
}

int smartdelete(Node *a, int key)
{
	int i;
	Node *ptr = a;
	Node *pp = NULL;
	Node *p = NULL;
	while (ptr->left || ptr->right)
	{
		//printf ("1 key %d\n", ptr->key);
		if ( ptr->left )
		{
			ptr = ptr->left;
		}
		else if ( ptr->right )
		{
			ptr = ptr->right;
		}
	}

	while ( ptr && ptr->key != key )
	{
		//printf ("2 key %d\n", ptr->key);
		pp = p;
		p = ptr;
		ptr = ptr->ptr;
	}

	if ( p )
	{
		printf("key = %d, info='%s'\n",p->key, p->info);
		erase(a, p->key);
	}
	if ( pp )
	{
		printf("key = %d, info='%s'\n",pp->key, pp->info);
		erase(a, pp->key);
	}

	if (ptr->ptr && ptr->ptr->ptr )
	{
		printf("key = %d, info='%s'\n",ptr->ptr->ptr->key, ptr->ptr->ptr->info);
		erase(a, ptr->ptr->ptr->key);
	}
	if ( ptr->ptr )
	{
		printf("key = %d, info='%s'\n",ptr->ptr->key, ptr->ptr->info);
		erase(a, ptr->ptr->key);
	}

	puts("===============");
	puts("noramlize");
	shead *stack = (shead*)calloc(sizeof(shead),1);
	post_order(a, stack);
	free(stack);
	return 1;
}

void generate(Node **tree, int range, int count)
{
	int i;
	time_t seconds;
	time(&seconds);
	srand((unsigned int) seconds);
	for ( i=0; i<count; i++ )
	{
		char *buf = (char*)malloc(2);
		int rnd = rand() % range;
		do
		{
			rnd = rand() % range;
			snprintf(buf, 2, "%d", rnd % 30);
		}
		while (!insert(tree, rnd, buf));
	}
}
int d_gen(Node **tree)
{
	char field[MAX_LEN];
	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("range for generate (0 to n): ");
		fgets(field, MAX_LEN, stdin);
	}
	int range = atoll(field);
	field[0]=0;
	while ( !isdigit(*field) )
	{
		printf("element count: ");
		fgets(field, MAX_LEN, stdin);
	}
	int count = atoll(field);
	if ( count > range )
	{
		puts("error, count great than range");
		return 1;
	}
	generate(tree, range, count);
	
	return 1;
}

int d_smartdelete(Node **node)
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
	
	smartdelete(a, key);
	printf("\n------\n\n");
	
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
	//if ( a->left )
	//	freetab(a->left);
	//if ( a->right )
	//	freetab(a->right);
	if (a)
		freetab(a);
	return 0;
}

int (*fptr[])(Node **) = {NULL, d_add, d_find, d_delete, d_show, d_bypass, d_gen, d_smartdelete};

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
