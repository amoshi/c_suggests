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
	struct rb_node *link[2];
} rb_node;

typedef struct rb_tree 
{
	struct rb_node *root;
	int count;
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
	rb_node *save = root->link[!dir];
 
	root->link[!dir] = save->link[dir];
	save->link[dir] = root;
 
	root->color = RED;
	save->color = BLACK;
 
	return save;
}
 
rb_node *rb_double ( rb_node *root, int dir )
{
	root->link[!dir] = rb_single ( root->link[!dir], !dir );
	return rb_single ( root, dir );
}

rb_node *make_node ( int64_t key, char *field )
{
	rb_node *rn = malloc ( sizeof *rn );
  
	if ( rn != NULL ) {
		rn->key = key;
		rn->color = RED;
		rn->link[LEFT] = NULL;
		rn->link[RIGHT] = NULL;
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
		q = t->link[RIGHT] = tree->root;
	
		for (;;) 
		{
			int flag = 1;
			if ( q == NULL )
			{
				flag = 0;
			        p->link[dir] = q = make_node ( key, field );
			        tree->count ++ ;
			        if ( q == NULL )
			       		return 0;
			}
			else if ( is_red ( q->link[LEFT] ) && is_red ( q->link[RIGHT] ) ) 
			{
			        q->color = RED;
			        q->link[LEFT]->color = BLACK;
			        q->link[RIGHT]->color = BLACK;
			}
			if ( is_red ( q ) && is_red ( p ) ) 
			{
			        int dir2 = t->link[RIGHT] == g;
	
			        if ( q == p->link[last] )
			       		t->link[dir2] = rb_single ( g, !last );
			        else
			       		t->link[dir2] = rb_double ( g, !last );
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
			q = q->link[dir];
		}
		tree->root = head.link[RIGHT];
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
		q->link[RIGHT] = tree->root;
 
		/* поиск и удаление */
		while ( q->link[dir] != NULL )
		{
			int last = dir;
 
			/* сохранение иерархии узлов во временные переменные */
			g = p, p = q;
			q = q->link[dir];
			dir = q->key < key;
 
			/* сохранение найденного узла */
			if ( q->key == key )
				f = q;
 
			if ( !is_red ( q ) && !is_red ( q->link[dir] ) ) {
				if ( is_red ( q->link[!dir] ) )
					p = p->link[last] = rb_single ( q, dir );
				else if ( !is_red ( q->link[!dir] ) ) {
					rb_node *s = p->link[!last];
 

					if ( s != NULL ) {
						if ( !is_red ( s->link[!last] ) && !is_red ( s->link[last] ) ) {
							/* смена цвета узлов */
							p->color = BLACK;
							s->color = RED;
							q->color = RED;
						}
						else {
							int dir2 = g->link[RIGHT] == p;
 
							if ( is_red ( s->link[last] ) )
								g->link[dir2] = rb_double ( p, last );
							else if ( is_red ( s->link[!last] ) )
								g->link[dir2] = rb_single ( p, last );
 
							/* корректировка цвета узлов */
							q->color = g->link[dir2]->color = RED;
							g->link[dir2]->link[LEFT]->color = BLACK;
							g->link[dir2]->link[RIGHT]->color = BLACK;
						}
					}
				}
			}
		}
 
		/* удаление найденного узла */
		if ( f != NULL ) {
			f->key = q->key;
			p->link[p->link[RIGHT] == q] =
				q->link[q->link[LEFT] == NULL];
			free ( q->info );
			free ( q );
		}
 
		/* обновление указателя на корень дерева */
		tree->root = head.link[RIGHT];
		if ( tree->root != NULL )
			tree->root->color = BLACK;
	}
 
	return 1;
}

void tree_show(rb_node *x)
{
	printf("%"u64": '%s'\n",x->key, x->info);
	if ( x->link[LEFT] )
		tree_show(x->link[LEFT]);
	if ( x->link[RIGHT] )
		tree_show(x->link[RIGHT]);
}

void rb_show ( rb_tree *tree )
{
	if ( tree && tree->root )
		tree_show(tree->root);
}

uint64_t tree_build(rb_node *x, uint64_t l)
{
	l++;

	if ( x->link[LEFT] )
		l = tree_build(x->link[LEFT], l++);

	uint64_t i;
	for ( i=0; i<l; i++)
		printf("\t");
	printf("%"u64" (%s)\n",x->key, x->info);

	if ( x->link[RIGHT] )
		l = tree_build(x->link[RIGHT], l++);
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
		return tree_get(x->link[LEFT], key);
	else if ( x && x->key > key )
		return tree_get(x->link[RIGHT], key);
	else if ( x && x->key == key )
	{
		printf("finded: %"u64"\n", x->link[LEFT]->key);
		return tree_get(x->link[RIGHT], key);
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
			x = x->link[LEFT];
		else if ( x->key < key )
			x = x->link[RIGHT];
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
		if ( x->link[RIGHT] && x->link[RIGHT]->key > x->key )
			x = x->link[RIGHT];
		else if ( x->link[LEFT] && x->link[LEFT]->key > x->key )
			x = x->link[RIGHT];
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
		printf("key: %"PRId64", info: '%s'\n", res->key, res->info);
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
	//if ( argc<4 )
	//	return 1;
	int i;
	//for ( i=0; i<argc-2; i++ )
	//{
	//	rb_insert ( tree, atoi(argv[i]) );
	//}
	//rb_delete(tree, atoi(argv[i]));
	//printf("show:\n");
	//rb_show ( tree );
	//printf("build:\n");
	//rb_build ( tree );
	//printf("get:\n");
	//rb_find(tree, atoi(argv[i+1]));
	//printf("max:\n");
	//rb_getmax(tree);

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
}
