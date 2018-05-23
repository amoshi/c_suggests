#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>

#define MAX_LEN	100000
#define	RED	1
#define	BLACK	0
#define	RIGHT	1
#define	LEFT	0

typedef struct rb_node 
{
	int color;
	int key;
	char *info;
	struct rb_node *steam[2];
	struct rb_node *parent;
	struct rb_node *ptr;
} rb_node;

typedef struct rb_tree 
{
	struct rb_node *root;
	int count;
} rb_tree;

typedef struct snode
{
	rb_node *ptr;
	struct snode *next;
	struct snode *prev;
} snode;

typedef struct shead
{
	snode *start;
	snode *end;
} shead;

void spush(shead *stack, rb_node *ptr)
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

rb_node* spop(shead *stack)
{
	if ( !stack || !(stack->start) )
	{
		return NULL;
	}
	if ( !(stack->end->prev) )
	{
		rb_node *ptr = stack->start->ptr;
		free(stack->start);
		stack->start = stack->end = NULL;
		return ptr;
	}
	rb_node *ptr = stack->end->ptr;
	snode *stnode = stack->end;
	stack->end = stnode->prev;
	stack->end->next = NULL;

	free(stnode);
	return ptr;
}

void post_order(rb_node *root, shead *stack)
{
	if ( root )
	{
		post_order(root->steam[LEFT], stack);
		post_order(root->steam[RIGHT], stack);
		rb_node *prev  = spop(stack);
		if ( prev )
			prev->ptr = root;
		spush(stack, root);
		//printf(":%d:", root->key);
		//if ( root->ptr )
		//	printf(":%d:", root->ptr->key);
		//puts("");
	}
}

int is_red ( rb_node *node )
{
	return node != NULL && node->color == RED;
}

rb_node *rb_single ( rb_node *root, int dir )
{
	rb_node *save = root->steam[!dir];
 
	root->steam[!dir] = save->steam[dir];
	save->steam[dir] = root;
 
	root->color = RED;
	save->color = BLACK;
 
	return save;
}
 
rb_node *rb_double ( rb_node *root, int dir )
{
	root->steam[!dir] = rb_single ( root->steam[!dir], !dir );
	return rb_single ( root, dir );
}

rb_node *make_node ( int key, char *field, rb_node *parent )
{
	rb_node *rn = (rb_node*)malloc ( sizeof *rn );
  
	if ( rn != NULL ) {
		rn->key = key;
		rn->color = RED;
		rn->steam[LEFT] = NULL;
		rn->steam[RIGHT] = NULL;
		rn->info = field;
		rn->parent = parent;
	}
	return rn;
}

int rb_insert ( rb_tree *tree, int key, char *field )
{
	int excode = 1;
	if ( tree->root == NULL )
	{
		tree->count ++ ;
	        tree->root = make_node ( key, field, NULL );
	        if ( tree->root == NULL )
	       		return 0;
	}
	else
	{
		rb_node head = {0};
		rb_node *g, *t;
		rb_node *p, *q;
		int dir = 0, last;
	
		t = &head;
		g = p = NULL;
		q = t->steam[RIGHT] = tree->root;
	
		for (;;) 
		{
			int flag = 1;
			if ( q == NULL )
			{
				flag = 0;
				p->steam[dir] = q = make_node ( key, field, p );
				tree->count ++ ;
				if ( q == NULL )
					return 0;
			}
			else if ( is_red ( q->steam[LEFT] ) && is_red ( q->steam[RIGHT] ) ) 
			{
				q->color = RED;
				q->steam[LEFT]->color = BLACK;
				q->steam[RIGHT]->color = BLACK;
			}

			if ( is_red ( q ) && is_red ( p ) ) 
			{
				//rb_node *p = q->parent;
				int dir2 = t->steam[RIGHT] == g;
				if ( q == p->steam[last] )
					t->steam[dir2] = rb_single ( g, !last );
				else
					t->steam[dir2] = rb_double ( g, !last );
			}
	
			if ( q->key == key )
			{
				if ( flag )
				{
					excode=0;
					printf("%u already exists\n", key);
				}
				break;
			}
	
			last = dir;
			dir = q->key < key;
	
			if ( g != NULL )
				t = g;
			g = p, p = q;
			q = q->steam[dir];
		}
		tree->root = head.steam[RIGHT];
	}
	tree->root->color = BLACK;
	shead *stack = (shead*)calloc(sizeof(shead),1);
	post_order(tree->root, stack);
	tree->root->ptr = NULL;
	free(stack);
	return excode;
}

int rb_delete ( rb_tree *tree, int key )
{
	if ( tree->root != NULL ) 
	{
		rb_node head = {0};
		rb_node *q, *p, *g;
		rb_node *f = NULL;
		int dir = 1;
 
		q = &head;
		g = p = NULL;
		q->steam[RIGHT] = tree->root;
 
		while ( q->steam[dir] != NULL )
		{
			int last = dir;
 
			g = p, p = q;
			q = q->steam[dir];
			dir = q->key < key;
 
			if ( q->key == key )
				f = q;
 
			if ( !is_red ( q ) && !is_red ( q->steam[dir] ) )
			{
				if ( is_red ( q->steam[!dir] ) )
					p = p->steam[last] = rb_single ( q, dir );
				else if ( !is_red ( q->steam[!dir] ) )
				{
					rb_node *s = p->steam[!last];

					if ( s != NULL )
					{
						if ( !is_red ( s->steam[!last] ) && !is_red ( s->steam[last] ) )
						{
							p->color = BLACK;
							s->color = RED;
							q->color = RED;
						}
						else {
							int dir2 = g->steam[RIGHT] == p;
 
							if ( is_red ( s->steam[last] ) )
								g->steam[dir2] = rb_double ( p, last );
							else if ( is_red ( s->steam[!last] ) )
								g->steam[dir2] = rb_single ( p, last );
 
							q->color = g->steam[dir2]->color = RED;
							g->steam[dir2]->steam[LEFT]->color = BLACK;
							g->steam[dir2]->steam[RIGHT]->color = BLACK;
						}
					}
				}
			}
		}
 
		if ( f != NULL )
		{
			f->key = q->key;
			p->steam[p->steam[RIGHT] == q] =
				q->steam[q->steam[LEFT] == NULL];
			free ( q->info );
			free ( q );
		}
 
		tree->root = head.steam[RIGHT];
		if ( tree->root != NULL )
			tree->root->color = BLACK;
	}
 
	shead *stack = (shead*)calloc(sizeof(shead),1);
	post_order(tree->root, stack);
	tree->root->ptr = NULL;
	free(stack);
	return 1;
}

void tree_show(rb_node *x)
{
	char *color = x->color ? (char*)"red" : (char*)"black";
	printf("%u: '%s' (%s)\n",x->key, x->info, color);
	if ( x->steam[LEFT] )
		tree_show(x->steam[LEFT]);
	if ( x->steam[RIGHT] )
		tree_show(x->steam[RIGHT]);
}

void rb_show ( rb_tree *tree )
{
	if ( tree && tree->root )
		tree_show(tree->root);
}

void tree_free(rb_node *x)
{
	printf("freeing %u: '%s'\n",x->key, x->info);
	if ( x->steam[LEFT] )
		tree_free(x->steam[LEFT]);
	if ( x->steam[RIGHT] )
		tree_free(x->steam[RIGHT]);
	free (x->info);
	free (x);
}

void rb_free ( rb_tree *tree )
{
	if ( tree && tree->root )
		tree_free(tree->root);
}

unsigned int tree_build(rb_node *x, unsigned int l)
{
	l++;

	char *color = x->color ? (char*)"red" : (char*)"black";
	if ( x->steam[RIGHT] )
		l = tree_build(x->steam[RIGHT], l++);

	unsigned int i;
	for ( i=0; i<l; i++)
		printf("\t");
	if ( x->ptr )
		printf("%u (%s:%s) ptr: %d\n",x->key, x->info, color, x->ptr->key);
	else
		printf("%u (%s:%s)\n",x->key, x->info, color);

	if ( x->steam[LEFT] )
		l = tree_build(x->steam[LEFT], l++);
	l--;

	return l;
}

void rb_build ( rb_tree *tree )
{
	if ( tree && tree->root )
		tree_build(tree->root, -1);
}

rb_tree* tree_get ( rb_node *x, int key )
{
	if (  x && x->key < key )
		return tree_get(x->steam[LEFT], key);
	else if ( x && x->key > key )
		return tree_get(x->steam[RIGHT], key);
	else if ( x && x->key == key )
	{
		printf("finded: %u\n", x->steam[LEFT]->key);
		return tree_get(x->steam[RIGHT], key);
	}
	else
		return NULL;
}

rb_node* rb_find ( rb_tree *tree, int key )
{
	if ( !tree || !(tree->root) )
		return NULL;
	rb_node *x = tree->root;
	while ( x )
	{
		if ( x->key > key )
			x = x->steam[LEFT];
		else if ( x->key < key )
			x = x->steam[RIGHT];
		else if ( x->key == key )
		{
			printf("finded: %u\n", x->key);
			return x;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}

const char *msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show", "5. Generate", "6. Bypass", "7. Smartdelete", "8. Load from file" };
const int NMsgs = sizeof ( msgs ) / sizeof ( msgs[0] );

int d_add(rb_tree *tree)
{
	char field[MAX_LEN];
	field[0]='a';
	size_t len;
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int key = atoll(field);

	printf("info: ");
	fgets(field, MAX_LEN, stdin);
	len = strlen(field);
	char *buf = (char*)malloc ( len );
	strncpy(buf, field, len);
	buf[len] = 0;
	rb_insert ( tree, key, buf );
	rb_build (tree);
	return 1;
}
int d_find(rb_tree *tree)
{
	char field[MAX_LEN];
	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int key = atoll(field);
	
	rb_node *res = rb_find ( tree, key );
	printf("\n\nResults:\n-------------\n");
	if ( res )
		printf("key: %d, info: '%s'\n", res->key, res->info);
	printf("\n------\n\n");
	
	return 1;
}
int d_delete(rb_tree *tree)
{
	char field[MAX_LEN];
	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int key = atoll(field);
	rb_delete(tree,key);
	return 1;
}
void rb_generate(rb_tree *tree, int range, int count)
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
		while (!rb_insert(tree, rnd, buf));
	}
}
int d_gen(rb_tree *tree)
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
	rb_generate(tree, range, count);
	
	return 1;
}
int d_show(rb_tree *tree)
{
	rb_show(tree);
	return 1;
}

int d_build(rb_tree *tree)
{
	rb_build(tree);
	return 1;
}

int d_bypass(rb_tree *tree)
{
	rb_node *a = tree->root;
	if ( !a )
		return 1;
	rb_node *ptr = a;
	while (ptr->steam[LEFT] || ptr->steam[RIGHT])
	{
		if ( ptr->steam[LEFT] )
		{
			ptr = ptr->steam[LEFT];
		}
		else if ( ptr->steam[RIGHT] )
		{
			ptr = ptr->steam[RIGHT];
		}
	}

	while ( ptr )
	{
		ptr = ptr->ptr;
	}

	return 1;
}

int smartdelete(rb_tree *tree, int key)
{
	rb_node *a = tree->root;
	rb_node *ptr = a;
	rb_node *pp = NULL;
	rb_node *p = NULL;
	while (ptr->steam[LEFT] || ptr->steam[RIGHT])
	{
		//printf ("1 key %d\n", ptr->key);
		if ( ptr->steam[LEFT] )
		{
			ptr = ptr->steam[LEFT];
		}
		else if ( ptr->steam[RIGHT] )
		{
			ptr = ptr->steam[RIGHT];
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
		rb_delete(tree,p->key);
	}
	if ( pp )
	{
		printf("key = %d, info='%s'\n",pp->key, pp->info);
		rb_delete(tree,pp->key);
	}

	if (ptr->ptr && ptr->ptr->ptr )
	{
		printf("key = %d, info='%s'\n",ptr->ptr->ptr->key, ptr->ptr->ptr->info);
		rb_delete(tree,ptr->ptr->ptr->key);
	}
	if ( ptr->ptr )
	{
		printf("key = %d, info='%s'\n",ptr->ptr->key, ptr->ptr->info);
		rb_delete(tree,ptr->ptr->key);
	}

	puts("===============");
	puts("noramlize");
	shead *stack = (shead*)calloc(sizeof(shead),1);
	post_order(tree->root, stack);
	tree->root->ptr = NULL;
	free(stack);
	return 1;
}

int d_smartdelete(rb_tree *tree)
{
	rb_node *a = tree->root;
	char field[MAX_LEN];
	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int key = atoll(field);
	
	smartdelete(tree, key);
	printf("\n------\n\n");
	
	return 1;
}


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

void file_input(rb_tree *tree, char *file)
{


	FILE *fd = fopen(file, "r");
	if ( !fd ) 
	{
		printf("trying open file '%s'\n", file);
		perror("cannot open file: ");
		return;
	}

	char field[MAX_LEN];
	unsigned int i;
	int key;
	for ( i=0; fgets(field, MAX_LEN, fd); i++ )
	{
		//printf("-------\nfield = '%s'\n", field);
		int key = atoll(field);
		//printf("key %d from %s\n", key, field);
		fgets(field, MAX_LEN, fd);
		size_t len = strlen(field);
		field[len-1] = 0;
		//printf("key %d, info: '%s'\n", key, field);
		char *buf = (char*)malloc ( len );
		strncpy(buf, field, len);
		buf[len] = 0;
		rb_insert ( tree, key, buf );
	}
	printf("%u keys loaded from file %s\n", i, file);

	fclose(fd);
}
int d_fload(rb_tree *tree)
{
	char field[MAX_LEN];
	printf("Filename: ");
	fgets(field, MAX_LEN, stdin);
	field[strlen(field)-1]='\0';
	
	file_input(tree, field);
	printf("\n------\n\n");
	
	return 1;
}
int (*fptr[])(rb_tree *) = {NULL, d_add, d_find, d_delete, d_build, d_gen, d_bypass, d_smartdelete, d_fload };

int main()
{
	rb_tree *tree = (shead*)calloc(1,sizeof(rb_tree));

	int rc;
	while ( (rc = dialog(msgs, NMsgs)) )
		if ( fptr[rc])
			if ( !fptr[rc](tree) )
				break;
	printf("OK\n");
	rb_free(tree);
}
