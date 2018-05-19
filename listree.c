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
	int release;
	struct Node *next;
} Node;

const char *msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show", "5. Generate" };
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
Node* erase(Node *node, int key, int release)
{
	// пустой элемент - завершить функцию, вернуть NULL
	if(node == NULL)
		return node;
 
	if(key == node->key && release == 0)
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
	else if (key == node->key && release > 0)
	{
		Node *cur = node;
		Node *prev = NULL;
		while (cur)
		{
			if ( cur->release == release )
			{
				prev->next = cur->next;
				free(cur->info);
				free(cur);

				return node;
				printf("key %d, release %d deleted\n", key, release);
			}
			prev = cur;
			cur = cur->next;
		}
		printf("key %d, release %d not found\n", key, release);
	}
	// рекурсивный обход дерева до нахождения удаляемого элемента
	else if (key < node->key)
		node->left  = erase(node->left, key, release);
	else
		node->right = erase(node->right, key, release);
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
		puts("1");
		Node *elem = (*proot);
		int i;
		for (i=1; elem->next; i++)
		{
			elem = elem->next;
		}
		elem->next = newnode;
		newnode->release = i;
		return newnode;
		//return NULL;
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
	cur->release = 0;
	cur->next = NULL;
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
		while ( res )
		{
			printf("key: %d, info: '%s', release: %d\n", res->key, res->info, res->release);
			res = res->next;
		}
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

	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("release: ");
		fgets(field, MAX_LEN, stdin);
	}
	int release = atoll(field);
	
	erase(a,key,release);
	
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
		Node *cur = ptr;
		while ( cur )
		{
			printf("key: %d, info: '%s' release: %d\t| ", cur->key, cur->info, cur->release);
			cur = cur->next;
		}
		puts("");
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
	show(a,-1);
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

void freetab(Node *proot)
{
	if (!proot)
	{
		return;
	}

	freetab(proot->left);
	freetab(proot->right);

	Node *ptr = proot;
	while ( ptr )
	{
		printf("deleting key from memory %d, release %d\n", ptr->key, ptr->release);
		Node *prev = ptr;
		free(ptr->info);
		free(ptr);
		ptr = ptr->next;
	}
	//free(proot->info);
	//free(proot);
}

int delTable(Node *a)
{
	if (a)
		freetab(a);
	return 0;
}

int (*fptr[])(Node **) = {NULL, d_add, d_find, d_delete, d_show, d_gen};

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
