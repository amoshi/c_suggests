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

void output (llstr_t *llstr)
{
	int64_t i, j;
	size_t n = llstr->n;
	for ( i=0; i<n; i++ )
	{
		lstr_t *elem = &llstr->ls[i];
		printf("\t");
		for ( j=0; elem->next; j++, elem=elem->next )
		{
			//printf("[%"PRIu64"][%"PRIu64"] : '%c'\n", i, j, elem->c);
			printf("%c", elem->c);
		}
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

llstr_t* action (llstr_t *llstr)
{
	size_t n = llstr->n;
	llstr_t *nnstr = malloc(sizeof(llstr_t));
	nnstr->ls = malloc(sizeof(lstr_t)*n);
	nnstr->n=n;
	int64_t i;
	for ( i=0; i<n; i++ )
	{
		lstr_t *elem = &llstr->ls[i];
		nnstr->ls[i].next=NULL;
		nnstr->ls[i].prev=NULL;
		for ( ; elem->next; elem=elem->next )
		{
			if ( !(elem->next->next) )
				break;
			//printf("[%"PRIu64"][%"PRIu64"] : '%c'\n", i, j, elem->c);
			if ( ( elem->c == '1' && elem->next->c == '0' ) || ( elem->c == '0' && elem->next->c == '1' ) )
			{
				lpush(&nnstr->ls[i], &elem->c, 1);
				lpush(&nnstr->ls[i], &elem->next->c, 1);
				elem = elem->next;
				//printf("(put=%c)\n", elem->c);
				//printf("(put=%c)\n", elem->next->c);
			}
		}
	}



////	for ( i=0; i<n; i++ )
////	{
////		size_t sn = strlen(mstr_i->s[i]);
////		newmstr->s[i] = malloc(sn+1);
////		for ( j=0, k=0; j<sn+1; j++ )
////		{
////			if ( ( mstr_i->s[i][j] == '1' && mstr_i->s[i][j+1] == '0' ) || ( mstr_i->s[i][j] == '0' && mstr_i->s[i][j+1] == '1' ) )
////			{
////				//printf("%% %c\n", mstr_i->s[i][j]);
////				//printf("%% %c\n", mstr_i->s[i][j+1]);
////				newmstr->s[i][k] = mstr_i->s[i][j];
////				newmstr->s[i][k+1] = mstr_i->s[i][j+1];
////				//printf("newmstr->s[%"PRIu64"][%"PRIu64"]=%c\n", i, k, newmstr->s[i][k]);
////				//printf("newmstr->s[%"PRIu64"][%"PRIu64"]=%c\n", i, k+1, newmstr->s[i][k+1]);
////				k+=2;
////				j++;
////			}
////		}
////		newmstr->s[i][k] = '\0';
////	}
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
}
