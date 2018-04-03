#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif
#define MAX_LEN	100000
#define u64	PRIu64
#define d64	PRId64
#define	RED	1
#define	BLACK	0
#define	RIGHT	1
#define	LEFT	0

typedef struct r_time
{
	int sec;
	int nsec;
} r_time;

typedef struct rb_node 
{
	int color;
	int64_t key;
	char *info;
	struct rb_node *steam[2];
} rb_node;

typedef struct rb_tree 
{
	struct rb_node *root;
	int64_t count;
} rb_tree;

r_time setrtime()
{
#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t timer1;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &timer1);
	mach_port_deallocate(mach_task_self(), cclock);

#else
	struct timespec timer1;
	clock_gettime(CLOCK_REALTIME, &timer1);
#endif
	r_time rt;
	rt.sec=timer1.tv_sec;
	rt.nsec=timer1.tv_nsec;
	return rt;
}

void getrtime(r_time t1, r_time t2)
{
	printf("complete for: %u.%09d sec\n",t2.sec-t1.sec,t2.nsec-t1.nsec);
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

rb_node *make_node ( int64_t key, char *field )
{
	rb_node *rn = malloc ( sizeof *rn );
  
	if ( rn != NULL ) {
		rn->key = key;
		rn->color = RED;
		rn->steam[LEFT] = NULL;
		rn->steam[RIGHT] = NULL;
		rn->info = field;
	}
	return rn;
}

int rb_insert ( rb_tree *tree, int64_t key, char *field )
{
	if ( tree->root == NULL )
	{
	        tree->root = make_node ( key, field );
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
				p->steam[dir] = q = make_node ( key, field );
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
				int dir2 = t->steam[RIGHT] == g;
	
				if ( q == p->steam[last] )
			       		t->steam[dir2] = rb_single ( g, !last );
				else
			       		t->steam[dir2] = rb_double ( g, !last );
			}
	
			if ( q->key == key )
			{
				if ( flag )
					printf("%"u64" already exists\n", key);
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
	return 1;
}

int rb_delete ( rb_tree *tree, int64_t key )
{
	if ( tree->root != NULL ) 
	{
		rb_node head = {0}; /* временный указатель на корень дерева */
		rb_node *q, *p, *g; /* вспомогательные переменные */
		rb_node *f = NULL;	/* узел, подлежащий удалению */
		int dir = 1;
 
		/* инициализация вспомогательных переменных */
		q = &head;
		g = p = NULL;
		q->steam[RIGHT] = tree->root;
 
		/* поиск и удаление */
		while ( q->steam[dir] != NULL )
		{
			int last = dir;
 
			/* сохранение иерархии узлов во временные переменные */
			g = p, p = q;
			q = q->steam[dir];
			dir = q->key < key;
 
			/* сохранение найденного узла */
			if ( q->key == key )
				f = q;
 
			if ( !is_red ( q ) && !is_red ( q->steam[dir] ) ) {
				if ( is_red ( q->steam[!dir] ) )
					p = p->steam[last] = rb_single ( q, dir );
				else if ( !is_red ( q->steam[!dir] ) ) {
					rb_node *s = p->steam[!last];
 

					if ( s != NULL ) {
						if ( !is_red ( s->steam[!last] ) && !is_red ( s->steam[last] ) ) {
							/* смена цвета узлов */
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
 
							/* корректировка цвета узлов */
							q->color = g->steam[dir2]->color = RED;
							g->steam[dir2]->steam[LEFT]->color = BLACK;
							g->steam[dir2]->steam[RIGHT]->color = BLACK;
						}
					}
				}
			}
		}
 
		/* удаление найденного узла */
		if ( f != NULL ) {
			f->key = q->key;
			p->steam[p->steam[RIGHT] == q] =
				q->steam[q->steam[LEFT] == NULL];
			free ( q->info );
			free ( q );
		}
 
		/* обновление указателя на корень дерева */
		tree->root = head.steam[RIGHT];
		if ( tree->root != NULL )
			tree->root->color = BLACK;
	}
 
	return 1;
}

void tree_show(rb_node *x)
{
	printf("%"u64": '%s'\n",x->key, x->info);
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
	printf("freeing %"u64": '%s'\n",x->key, x->info);
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

uint64_t tree_build(rb_node *x, uint64_t l)
{
	l++;

	if ( x->steam[LEFT] )
		l = tree_build(x->steam[LEFT], l++);

	uint64_t i;
	for ( i=0; i<l; i++)
		printf("\t");
	printf("%"u64" (%s)\n",x->key, x->info);

	if ( x->steam[RIGHT] )
		l = tree_build(x->steam[RIGHT], l++);
	l--;

	return l;
}

void rb_build ( rb_tree *tree )
{
	if ( tree && tree->root )
		tree_build(tree->root, 0);
}

rb_tree* tree_get ( rb_node *x, int64_t key )
{
	if (  x && x->key < key )
		return tree_get(x->steam[LEFT], key);
	else if ( x && x->key > key )
		return tree_get(x->steam[RIGHT], key);
	else if ( x && x->key == key )
	{
		printf("finded: %"u64"\n", x->steam[LEFT]->key);
		return tree_get(x->steam[RIGHT], key);
	}
	else
		return NULL;
}

rb_node* rb_find ( rb_tree *tree, int64_t key )
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
			printf("finded: %"u64"\n", x->key);
			return x;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}

void rb_getmax(rb_tree *tree)
{
	if ( !tree || !(tree->root) )
		return;
	rb_node *x = tree->root;
	while ( x )
	{
		printf("trying %"u64"\n", x->key);
		if ( x->steam[RIGHT] && x->steam[RIGHT]->key > x->key )
			x = x->steam[RIGHT];
		else if ( x->steam[LEFT] && x->steam[LEFT]->key > x->key )
			x = x->steam[RIGHT];
		else
		{
			printf("max: %"u64"\n", x->key);
			return;
		}
	}
}

const char *msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show", "5. Build tree", "6. Get max" };
const int NMsgs = sizeof ( msgs ) / sizeof ( msgs[0] );

int64_t d_add(rb_tree *tree)
{
	char field[MAX_LEN];
	field[0]='a';
	size_t len;
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int64_t key = atoll(field);

	printf("info: ");
	fgets(field, MAX_LEN, stdin);
	len = strlen(field);
	char *buf = malloc ( len );
	strlcpy(buf, field, len);
	buf[len] = 0;
	r_time t1 = setrtime();
	rb_insert ( tree, key, buf );
	r_time t2 = setrtime();
	getrtime(t1, t2);
	return 1;
}
int64_t d_find(rb_tree *tree)
{
	char field[MAX_LEN];
	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int64_t key = atoll(field);
	
	r_time t1 = setrtime();
	rb_node *res = rb_find ( tree, key );
	r_time t2 = setrtime();
	getrtime(t1, t2);
	printf("\n\nResults:\n-------------\n");
	if ( res )
		printf("key: %"d64", info: '%s'\n", res->key, res->info);
	printf("\n------\n\n");
	
	return 1;
}
int64_t d_delete(rb_tree *tree)
{
	char field[MAX_LEN];
	field[0]='a';
	while ( !isdigit(*field) )
	{
		printf("key: ");
		fgets(field, MAX_LEN, stdin);
	}
	int64_t key = atoll(field);
	r_time t1 = setrtime();
	rb_delete(tree,key);
	r_time t2 = setrtime();
	getrtime(t1, t2);
	
	return 1;
}
int64_t d_show(rb_tree *tree)
{
	r_time t1 = setrtime();
	rb_show(tree);
	r_time t2 = setrtime();
	getrtime(t1, t2);
	return 1;
}

int64_t d_build(rb_tree *tree)
{
	r_time t1 = setrtime();
	rb_build(tree);
	r_time t2 = setrtime();
	getrtime(t1, t2);
	return 1;
}

int64_t d_max(rb_tree *tree)
{
	r_time t1 = setrtime();
	rb_getmax(tree);
	r_time t2 = setrtime();
	getrtime(t1, t2);
	return 1;
}

int64_t (*fptr[])(rb_tree *) = {NULL, d_add, d_find, d_delete, d_show, d_build, d_max, NULL};

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

void file_input(rb_tree *tree, char *file)
{

	r_time t1 = setrtime();

	FILE *fd = fopen(file, "r");
	if ( !fd ) 
		return;

	char field[MAX_LEN];
	uint64_t i;
	int64_t key;
	for ( i=0; fgets(field, MAX_LEN, fd); i++ )
	{
		//printf("-------\nfield = '%s'\n", field);
		int64_t key = atoll(field);
		//printf("key %"d64" from %s\n", key, field);
		fgets(field, MAX_LEN, fd);
		size_t len = strlen(field);
		field[len-1] = 0;
		//printf("key %"d64", info: '%s'\n", key, field);
		char *buf = malloc ( len );
		strlcpy(buf, field, len);
		buf[len] = 0;
		rb_insert ( tree, key, buf );
	}
	printf("%"u64" keys loaded from file %s\n", i, file);

	r_time t2 = setrtime();
	getrtime(t1, t2);
	fclose(fd);
}

int main(int argc, char **argv)
{
	rb_tree *tree = calloc(1,sizeof(rb_tree));
	int i;

	if ( argc > 1 )
	{
		file_input(tree, argv[1]);
	}

	int rc;
        while ( (rc = dialog(msgs, NMsgs)) )
                if ( fptr[rc])
                        if ( !fptr[rc](tree) )
                                break;
        printf("OK\n");
	rb_free(tree);
}
