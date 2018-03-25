#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#define MAX_LEN	100000
#define u64	PRIu64
#define d64	PRId64

typedef struct rb_node 
{
    int red;
    int64_t data;
    struct rb_node *link[2];
} rb_node;

typedef struct rb_tree 
{
    struct rb_node *root; // указатель на корневой узел
    int count; // количество узлов в дереве
} rb_tree;

int is_red ( rb_node *node )
{
    return node != NULL && node->red == 1;
}

/* функция для однократного поворота узла */
rb_node *rb_single ( rb_node *root, int dir )
{
    rb_node *save = root->link[!dir];
 
    root->link[!dir] = save->link[dir];
    save->link[dir] = root;
 
    root->red = 1;
    save->red = 0;
 
    return save;
}
 
/* функция для двукратного поворота узла */
rb_node *rb_double ( rb_node *root, int dir )
{
    root->link[!dir] = rb_single ( root->link[!dir], !dir );
    return rb_single ( root, dir );
}

rb_node *make_node ( int64_t data )
{
    rb_node *rn = malloc ( sizeof *rn );
  
    if ( rn != NULL ) {
        rn->data = data;
        rn->red = 1; /* –инициализация красным цветом */
        rn->link[0] = NULL;
        rn->link[1] = NULL;
    }
    return rn;
}

int rb_insert ( rb_tree *tree, int64_t data )
{
	/* если добавляемый элемент оказывается первым – то ничего делать не нужно*/
	if ( tree->root == NULL )
	{
	        tree->root = make_node ( data );
	        if ( tree->root == NULL )
	       		return 0;
	}
	else
	{
		rb_node head = {0}; /* временный корень дерева*/
		rb_node *g, *t;		 /* дедушка и родитель */
		rb_node *p, *q;		 /* родитель и итератор */
		int dir = 0, last;
	
		/* вспомогательные переменные */
		t = &head;
		g = p = NULL;
		q = t->link[1] = tree->root;
	
		/* основной цикл прохода по дереву */
		for (;;) 
		{
			int flag = 1;
			if ( q == NULL )
			{
				flag = 0;
			        /* вставка ноды */
			        p->link[dir] = q = make_node ( data );
			        tree->count ++ ;
			        if ( q == NULL )
			       		return 0;
			}
			else if ( is_red ( q->link[0] ) && is_red ( q->link[1] ) ) 
			{
			        /* смена цвета */
			        q->red = 1;
			        q->link[0]->red = 0;
			        q->link[1]->red = 0;
			}
			       /* совпадение 2-х красных цветов */
			if ( is_red ( q ) && is_red ( p ) ) 
			{
			        int dir2 = t->link[1] == g;
	
			        if ( q == p->link[last] )
			       		t->link[dir2] = rb_single ( g, !last );
			        else
			       		t->link[dir2] = rb_double ( g, !last );
			}
	
			/* такой узел в дереве уже есть	- выход из функции*/
			if ( q->data == data )
			{
				if ( flag )
					printf("%"u64" already exists\n", data);
				break;
			}
	
			last = dir;
			dir = q->data < data;
	
			if ( g != NULL )
			        t = g;
			g = p, p = q;
			q = q->link[dir];
		}
		/* обновить указатель на корень дерева*/
		tree->root = head.link[1];
	}
	/* сделать корень дерева черным */
	tree->root->red = 0;
	return 1;
}

int rb_delete ( rb_tree *tree, int64_t data )
 {
   if ( tree->root != NULL ) 
   {
     rb_node head = {0}; /* временный указатель на корень дерева */
     rb_node *q, *p, *g; /* вспомогательные переменные */
     rb_node *f = NULL;  /* узел, подлежащий удалению */
     int dir = 1;
  
     /* инициализация вспомогательных переменных */
     q = &head;
     g = p = NULL;
     q->link[1] = tree->root;
  
     /* поиск и удаление */
     while ( q->link[dir] != NULL ) {
       int last = dir;
  
       /* сохранение иерархии узлов во временные переменные */
       g = p, p = q;
       q = q->link[dir];
       dir = q->data < data;
  
       /* сохранение найденного узла */
       if ( q->data == data )
         f = q;
  
       if ( !is_red ( q ) && !is_red ( q->link[dir] ) ) {
         if ( is_red ( q->link[!dir] ) )
           p = p->link[last] = rb_single ( q, dir );
         else if ( !is_red ( q->link[!dir] ) ) {
           rb_node *s = p->link[!last];
  
 
           if ( s != NULL ) {
             if ( !is_red ( s->link[!last] ) && !is_red ( s->link[last] ) ) {
               /* смена цвета узлов */
               p->red = 0;
               s->red = 1;
               q->red = 1;
             }
             else {
               int dir2 = g->link[1] == p;
  
               if ( is_red ( s->link[last] ) )
                 g->link[dir2] = rb_double ( p, last );
               else if ( is_red ( s->link[!last] ) )
                 g->link[dir2] = rb_single ( p, last );
  
               /* корректировка цвета узлов */
               q->red = g->link[dir2]->red = 1;
               g->link[dir2]->link[0]->red = 0;
               g->link[dir2]->link[1]->red = 0;
             }
           }
         }
       }
     }
  
     /* удаление найденного узла */
     if ( f != NULL ) {
       f->data = q->data;
       p->link[p->link[1] == q] =
         q->link[q->link[0] == NULL];
       free ( q );
     }
  
     /* обновление указателя на корень дерева */
     tree->root = head.link[1];
     if ( tree->root != NULL )
       tree->root->red = 0;
   }
  
   return 1;
 }

uint64_t tree_show(rb_node *x, uint64_t l)
{
	uint64_t i;
	for ( i=0; i<l; i++)
		printf(" ");
	printf("%"u64" (%d)\n",x->data, x->red);
	l++;
	if ( x->link[0] )
		l = tree_show(x->link[0], l++);
	if ( x->link[1] )
		l = tree_show(x->link[1], l++);
	l--;
	return l;
}

void rb_show ( rb_tree *tree )
{
	if ( tree && tree->root )
		tree_show(tree->root, 0);
}

uint64_t tree_build(rb_node *x, uint64_t l)
{
	l++;

	if ( x->link[0] )
		l = tree_build(x->link[0], l++);

	if ( x->link[1] )
		l = tree_build(x->link[1], l++);
	l--;

	uint64_t i;
	for ( i=0; i<l; i++)
		printf(" ");
	printf("%"u64" (%d)\n",x->data, x->red);
	return l;
}

void rb_build ( rb_tree *tree )
{
	if ( tree && tree->root )
		tree_build(tree->root, 0);
}

rb_tree* tree_get ( rb_node *x, int64_t key )
{
	if (  x && x->data < key )
		return tree_get(x->link[0], key);
	else if ( x && x->data > key )
		return tree_get(x->link[1], key);
	else if ( x && x->data == key )
	{
		printf("finded: %"u64"\n", x->link[0]->data);
		return tree_get(x->link[1], key);
	}
	else
		return NULL;
}

int rb_find ( rb_tree *tree, int64_t key )
{
	if ( !tree || !(tree->root) )
		return 0;
	rb_node *x = tree->root;
	while ( x )
	{
		if ( x->data > key )
			x = x->link[0];
		else if ( x->data < key )
			x = x->link[1];
		else if ( x->data == key )
		{
			printf("finded: %"u64"\n", x->data);
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

void rb_getmax(rb_tree *tree)
{
	if ( !tree || !(tree->root) )
		return;
	rb_node *x = tree->root;
	while ( x )
	{
		printf("trying %"u64"\n", x->data);
		if ( x->link[1] && x->link[1]->data > x->data )
			x = x->link[1];
		else if ( x->link[0] && x->link[0]->data > x->data )
			x = x->link[0];
		else
		{
			printf("max: %"u64"\n", x->data);
			return;
		}
	}
}

int main(int argc, char **argv)
{
	if ( argc<4 )
		return 1;
	rb_tree *tree = calloc(1,sizeof(rb_tree));
	int i;
	for ( i=0; i<argc-2; i++ )
	{
		rb_insert ( tree, atoi(argv[i]) );
	}
	rb_delete(tree, atoi(argv[i]));
	printf("show:\n");
	rb_show ( tree );
	printf("build:\n");
	rb_build ( tree );
	printf("get:\n");
	rb_find(tree, atoi(argv[i+1]));
	printf("max:\n");
	rb_getmax(tree);
}
