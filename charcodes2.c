#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#define ARG_INPUT_MAXSIZE 1024000
#define ARG_ARRAY_MAXSIZE 100000

typedef struct lstr_t
{
	char c;
	struct lstr_t *next;
	struct lstr_t *prev;
} lstr_t;

typedef struct llstr_t
{
	lstr_t *ls;
	size_t n;
} llstr_t;

void erase (llstr_t *llstr)
{
	free(llstr);
}

void output (llstr_t *llstr)
{
	int64_t i, j;
	size_t n = llstr->n;
	for ( i=0; i<n; i++ )
	{
		lstr_t *elem = &llstr->ls[i];
		printf("\t'");
		for ( j=0; elem->next; j++, elem=elem->next )
		{
			//printf("[%"PRIu64"][%"PRIu64"] : '%c'\n", i, j, elem->c);
			printf("'%c'", elem->c);
		}
		puts("'");
	}
}

void lpush(lstr_t *lstr, char *str, size_t n)
{
	int64_t i;
	lstr_t *elem = lstr;
	while ( elem->next ) { /*puts("follow");*/ elem=elem->next; }
	for (i=0; i<n; i++)
	{
		elem->c = str[i];
		//printf("put %c\n", elem->c);
		elem->next = malloc(sizeof(lstr_t));
		elem->next->next = NULL;
		elem->next->prev = elem;
		elem = elem->next;
	}
}

llstr_t* input ()
{
	char *arg = malloc( ARG_INPUT_MAXSIZE );
	llstr_t *llstr = malloc(sizeof(llstr));
	llstr->ls = malloc(sizeof(lstr_t)*ARG_ARRAY_MAXSIZE);

	int64_t i;

	printf("enter string:\n\t");
	for ( i=0; fgets(arg, ARG_INPUT_MAXSIZE, stdin) && i<ARG_ARRAY_MAXSIZE; i++ )
	{
		printf("\t");
		size_t arg_len = strlen(arg);
		llstr->ls[i].next = NULL;
		llstr->ls[i].prev = NULL;
		lpush(&llstr->ls[i], arg, arg_len-1);
	}
	llstr->n = i;
	return llstr;
}

void normalize (llstr_t *llstr)
{
	size_t n = llstr->n;
	int64_t i, j;
	for ( i=0; i<n; i++ )
	{
		
	}
}

llstr_t* action (llstr_t *llstr)
{
	size_t n = llstr->n;
	llstr_t *nnstr = malloc(sizeof(llstr_t));
	nnstr->ls = malloc(sizeof(lstr_t)*n);
	nnstr->n=n;
	int64_t i, j;
	for ( i=0; i<n; i++ )
	{
		lstr_t *elem = &llstr->ls[i];
		nnstr->ls[i].next=NULL;
		nnstr->ls[i].prev=NULL;
		int isspace = 1;
		for ( j=0 ; elem->next; elem=elem->next, j++ )
		{
			printf("1elem->next is %p\n", elem->next->next);
			if ( !(elem->next) )
				break;
			if ( ( elem->c == ' ' ) || ( elem->c == '\t' ) )
			{
				if ( isspace == 0 )
				{
					if ( elem->next->next != NULL )
					{
						printf("j=%lld\n", j);
						isspace = 1;
						lpush(&nnstr->ls[i], " ", 1);
					}
				}
			}
			if ( ( elem->c == '1' && elem->next->c == '0' ) || ( elem->c == '0' && elem->next->c == '1' ) )
			{
				lpush(&nnstr->ls[i], &elem->c, 1);
				lpush(&nnstr->ls[i], &elem->next->c, 1);
				elem = elem->next;
				isspace = 0;
				printf("2elem->next is %p\n", elem->next);
				if ( elem->next == NULL )
					break;
					//isspace = 1;
			}
		}
	}



	return nnstr;
}

int main()
{
	llstr_t *llstr = input();
	printf("\n\ninput vars:\n");
	output(llstr);
	llstr_t *nnstr = action(llstr);
	normalize(nnstr);
	printf("\n\noutput vars:\n");
	output(nnstr);
	erase(nnstr);
	erase(llstr);
}
