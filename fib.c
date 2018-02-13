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

typedef struct sseq
{
	seq *seq;
	size_t n;
} sseq;

typedef struct uintstruct
{
	int64_t *fib;
	size_t n;
} uintstruct;

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
		cur += strcspn(cur, " \t\n\0") + 1;
	}
	cur = str;
	seq = malloc (sizeof(int64_t)*i);
	for ( j=0; j<i; j++ )
	{
		cur += strcspn(cur, "0123456789");
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
	
	fgets(arg, ARG_INPUT_MAXSIZE, stdin);
	string_to_int( arg, strlen(arg), st[0] );
}

void erase_seq (seq *st)
{
	free( st->s );
	free( st );
}

void erase_sseq ( sseq *sst )
{
	uint64_t i;
	for (i=0; i<sst->n; i++)
		free( sst->seq[i].s );
	free(sst);
}


void erase_fib ( uintstruct *fs )
{
	free(fs->fib);
	free(fs);
}
void output(seq *st)
{
	int64_t i;
	for ( i=0; i<st->m; i++ )
	{
		printf("%5"PRId64" ", st->s[i]);
	}
	puts("");
}

void soutput(sseq *sst, seq *st)
{
	int64_t i;
	for ( i=0; i < sst->n; i++ )
	{
		printf("%6"PRId64": ", st->s[i]);
		output(sst->seq+i);
	}
}

int64_t max(seq *st)
{
	uint64_t i;
	int64_t max = st->s[0];
	for ( i=1; i<st->m; i++ )
	{
		if ( max < st->s[i] )
			max=st->s[i];
	}
	return max;
}

uintstruct* gen_fib(int64_t max)
{
	uint64_t i, j;
	int64_t tmp0 = 0, tmp1 = 1, tmp2 = 0, tmpcur = 0;
	for ( i=2; tmp1<max; tmpcur=tmp0 + tmp1, tmp0=tmp1, tmp1=tmpcur, i++ /*, printf("tmp=%"PRId64"/%"PRId64"\n", tmpcur, max)*/ );
	int64_t *fib = malloc(sizeof(int64_t)*i);
	fib[0]=0;
	fib[1]=1;
	for ( j=2; j<i; j++ )
	{
		fib[j]=fib[j-1]+fib[j-2];
	}
	uintstruct *fs = malloc ( sizeof(uintstruct) );
	fs->fib = fib;
	fs->n = j;
	return fs;
}

sseq* make_matrix (seq *st, uintstruct *fs)
{
	int64_t i, j;
	sseq *sst;
	sst = malloc( sizeof(sseq) );
	sst->n = st->m;
	sst->seq = malloc ( sizeof(seq) * st->m );
	for ( j=0; j<st->m; j++ )
	{
		int64_t count = st->s[j];
		uint64_t k;
		for ( i=fs->n-1, k=0; count>=0 && i>0; i-- )
		{
			if ( count - fs->fib[i] >= 0 )
			{
				count -= fs->fib[i];
				k++;
			}
		}

		sst->seq[j].s = malloc (sizeof(int64_t) * k);
		count = st->s[j];
		sst->seq[j].m = k;
		for ( i=fs->n-1, k=0; count>=0 && i>0; i-- )
		{
			if ( count - fs->fib[i] >= 0 )
			{
				count -= fs->fib[i];
				sst->seq[j].s[k] = fs->fib[i];
				k++;
			}
		}
	}
	return sst;
}

void action(seq *st, sseq **sst)
{
	int64_t mmax = max(st);
	uintstruct *fs = gen_fib(mmax);
	sst[0] = make_matrix(st, fs);
	erase_fib (fs);
}

int main()
{
	seq *st = NULL;
	sseq *sst = NULL;
	input (&st);
	action (st, &sst);
	printf("input numbers\n");
	output (st);
	printf("output matrix\n");
	soutput (sst, st);
	erase_seq (st);
	erase_sseq (sst);
}
