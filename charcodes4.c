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

void ldel(lstr_t *lstr)
{
	if ( lstr->next )
		//lstr->next=lstr->next->next;
		lstr->next->prev = lstr->prev;
	if ( lstr->prev )
		//lstr->prev=lstr->prev->prev;
		lstr->prev->next = lstr->next;
	free(lstr);
}

void erase (llstr_t *llstr)
{
	size_t n = llstr->n;
	uint64_t i;
	lstr_t *ls, *next;
	ls = llstr->ls;
	for ( i=0; i<n; i++ )
	{
		if ( ls->next)
			next = ls->next;
		ldel(ls);
		ls = next;
	}
	free(llstr);
}

void output (llstr_t *llstr)
{
	int64_t i, j;
	size_t n = llstr->n;
	for ( i=0; i<n; i++ )
	{
		lstr_t *elem = &llstr->ls[i];
		printf("\t");
		for ( j=0; elem->next; printf("%c", elem->c), j++, elem=elem->next );
		puts("");
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

	int64_t i = 0;
	int n;

	printf("enter string:\n\t");
	//for ( i=0; fgets(arg, ARG_INPUT_MAXSIZE, stdin) && i<ARG_ARRAY_MAXSIZE; i++ )
	//{
	//	printf("\t");
	//	size_t arg_len = strlen(arg);
	//	llstr->ls[i].next = NULL;
	//	llstr->ls[i].prev = NULL;
	//	lpush(&llstr->ls[i], arg, arg_len-1);
	//}
        do {
                n = scanf("%1000[^\n]", arg);
                if ( n < 0 )
                {
                        continue;
                }
                else
                        scanf("%*c");
		size_t arg_len = strlen(arg);
		llstr->ls[i].next = NULL;
		llstr->ls[i].prev = NULL;
		lpush(&llstr->ls[i], arg, arg_len-1);
                i++;
        } while ( n > 0 );



	llstr->n = i;
	return llstr;
}

void case_action (lstr_t *elem, llstr_t *nnstr, int64_t i)
{
	int64_t j;
	int isspace = 2;
	for ( j=0 ; elem->next; elem=elem->next, j++ )
	{
		if ( ( ( elem->c == ' ' ) || ( elem->c == '\t' ) ) && isspace == 0 )
		{
			isspace = 1;
		}
		//if ( ( elem->c == '1' && elem->next->c == '0' ) || ( elem->c == '0' && elem->next->c == '1' ) )
		if ( ( elem->c == '0' && elem->next->c == '1' ) )
		{
			if ( isspace == 1 )
			{
				lpush(&nnstr->ls[i], " ", 1);
			}
			isspace = 0;
			lpush(&nnstr->ls[i], &elem->c, 1);
			lpush(&nnstr->ls[i], &elem->next->c, 1);
			elem = elem->next;
		}
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
		case_action(elem, nnstr, i);
	}

	return nnstr;
}

int main()
{
	llstr_t *llstr = input();
	printf("\n\ninput vars:\n");
	output(llstr);
	llstr_t *nnstr = action(llstr);
	printf("\n\noutput vars:\n");
	output(nnstr);
	erase(nnstr);
	erase(llstr);
}
