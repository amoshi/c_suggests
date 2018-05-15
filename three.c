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

const char *msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show", "5. Bypass", "6. Generate" };
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
		return NULL;
	ptr = (newnode->key < (*proot)->key) ?  &(*proot)->left : &(*proot)->right;
	
	return instree(ptr, newnode);

}

// функция нормализации обратной прошивки
void rev_normalize(Node *proot, Node *next, Node *par)
{
	Node *left = NULL;
	int flag = 0;
	if ( proot->left )
	{
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
		//printf("trying %d\n", proot->key);
		rev_normalize(proot->right, proot, proot);
	}
	else if (proot->right && flag )
		rev_normalize(proot->right, par->left, proot);
	//if ( next )
		//printf("set thread for %d to %d\n", proot->key, next->key);
	//else
	//	printf("passing %d\n", proot->key);
	proot->ptr = next;
}

// установка связей между левым и правым деревом от корня
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

// fixup threads tree
void rrev_normalize(Node *proot, Node *next)
{
	if ( proot->left )
	{
		if ( proot->right )
			rev_normalize(proot->left, proot->right, proot);
		else
			rev_normalize(proot->left, proot, proot);
	}
	linker(proot); // установка связей левого и правого поддерева от корня
	if ( proot->right )
	{
		rev_normalize(proot->right, proot, proot);
	}
	//if ( next )
	//	printf("\tset thread for %d to %d\n", proot->key, next->key);
	//else
	//	printf("\tpassing %d\n", proot->key);
	proot->ptr = next;
}

// функция вставки в дерево
Node *insert(Node **proot, int k, char *in)
{
	Node *cur, *ptr;
	size_t inl = strlen(in); // length info
	cur = (Node*)malloc(sizeof(Node));	// allocate node
	cur->info = (char*)malloc(inl+1);	// allocate info
	if(!cur)
		return NULL;
	cur->key = k;				// set key
	strncpy(cur->info,in,inl);		// copy info to node
	cur->info[inl]='\0';
	cur->left = cur->right = NULL;		// set leaf
	if(!(ptr = instree(proot, cur)))	// worker insert to tree
	{
		puts("this key already in used");
		free(cur->info);
		free(cur);
	}
	else
	{
		rrev_normalize(proot[0], NULL);	// set threads
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

	printf("info: ");
	fgets(field, MAX_LEN, stdin);
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

void generate(Node *tree, int range, int count)
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
		while (!insert(&tree, rnd, buf));
	}
}
int d_gen(Node *tree)
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

int (*fptr[])(Node *) = {NULL, d_add, d_find, d_delete, d_show, d_bypass, d_gen};

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
		int key = atoll(field);
		fgets(field, MAX_LEN, fd);
		size_t len = strlen(field);
		field[len-1] = 0;
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
