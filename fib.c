#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#define ARG_INPUT_MAXSIZE 1024000

typedef struct seq
{
	int64_t *s;
	size_t m;
} seq;

size_t string_to_int( char *str, size_t l, seq *st )
{
	uint64_t i, j, k;
	char *cur = str;
	char *curswp = cur;
	int64_t *seq;
	for ( i=0; cur < str+l; i++ )
	{
		curswp = cur;
		cur += strcspn(cur, "0123456789");
		if ( cur == str+l )
			break;
		printf("%"PRIu64"/%zu: %p/%p/%p: %s\n", i, l, cur, curswp, str+l, cur);
		cur += strcspn(cur, " \t\n\0") + 1;
	}
	cur = str;
	seq = malloc (sizeof(int64_t)*i);
	for ( j=0; j<i; j++ )
	{
		cur += strcspn(cur, "0123456789");
		printf("%lld\n", atoll(cur));
		seq[j]=atoll(cur);
		cur += strcspn(cur, " \t\n\0") + 1;
	}
	st->s = seq;
	st->m=i;
	return i;
}

void input (seq **st)
{
	char *arg = malloc( ARG_INPUT_MAXSIZE );
	st[0] = malloc(sizeof(seq));
	
//	int res = scanf("%s", arg);
//	printf("%d, %s\n", res, arg);
//	res = scanf("%s", arg);
//	printf("%d, %s\n", res, arg);
	fgets(arg, ARG_INPUT_MAXSIZE, stdin);
	printf("%s\n", arg);
	string_to_int( arg, strlen(arg), st[0] );
}

void erase (seq **st)
{
	uint64_t i;
	free( st[0]->s );
	free( st[0] );
	st = NULL;
}

void output(seq *st)
{
	int64_t i;
	for ( i=0; i<st->m; i++ )
	{
		printf(" %"PRId64" ", st->s[i]);
	}
}

void action(seq *st)
{
}

int main()
{
	seq *st = NULL;
	input (&st);
	puts("out");
	output (st);
	action (st);
	output (st);
	//erase (&st);
}
