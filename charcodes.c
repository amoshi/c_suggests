#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#define ARG_INPUT_MAXSIZE 1024000
#define ARG_ARRAY_MAXSIZE 100000

typedef struct mstring_t
{
	char **s;
	size_t n;
} mstring_t;

void output (mstring_t *mstr_i)
{
	size_t n = mstr_i->n;
	int64_t i;
	for ( i=0; i<n; i++ )
	{
		printf("[%"PRIu64"] : '%s'\n", i, mstr_i->s[i]);
	}
}

void erase (mstring_t *mstr_i)
{
	size_t n = mstr_i->n;
	int64_t i;
	for ( i=0; i<n; i++ )
	{
		free(mstr_i->s[i]);
	}
}

size_t input (char ***istr)
{
	char *arg = malloc( ARG_INPUT_MAXSIZE );

	istr[0] = malloc( (sizeof(char*))*ARG_ARRAY_MAXSIZE );
	int64_t i;

	printf("enter string:\n\t");
	for ( i=0; fgets(arg, ARG_INPUT_MAXSIZE, stdin); i++ )
	{
		printf("\t");
		size_t arg_len = strlen(arg);
		malloc(arg_len);
		istr[0][i] = malloc(arg_len);
		strlcpy(istr[0][i],arg,arg_len);
	}
	return i;
}

void case_action (char *s, char *sz, size_t sn) 
{
	int64_t k, j;
	for ( j=0, k=0; j<sn+1; j++ )
	{
		int64_t a = strspn(s+j," \t");
		if ( a>0 )
		{
			j += a-1;
		}
		if ( ( !strncmp(s+j, " ", 1) ) || ( !strncmp(s+j, "\t", 1) ) )
		{
			if ( ( sz[k-1] != ' ' ) && ( k != 0 ) )
			{
				sz[k]=' ';
				k++;
			}
		}
		if ( ( !strncmp(s+j, "1", 1) && !strncmp(s+j+1, "0", 1) ) || ( !strncmp(s+j, "0", 1) && !strncmp(s+j+1, "1", 1) ) )
		{
			sz[k] = s[j];
			sz[k+1] = s[j+1];
			k+=2;
			j++;
		}
	}
	sz[k] = '\0';
	if ( sz[k-1] == ' ' )
		sz[k-1]='\0';
	
}

mstring_t* action (mstring_t *mstr_i)
{
	size_t n = mstr_i->n;
	mstring_t *newmstr = malloc(sizeof(mstring_t));
	newmstr->s = malloc(sizeof(char*)*n);
	newmstr->n=n;
	int64_t i, j, k;
	for ( i=0; i<n; i++ )
	{
		size_t sn = strlen(mstr_i->s[i]);
		newmstr->s[i] = malloc(sn+1);
		case_action (mstr_i->s[i], newmstr->s[i], sn);
	}
	return newmstr;
}

int main()
{
	char **istr;
	mstring_t *mstr_i = malloc (sizeof(mstring_t));
	mstring_t *mstr_o;
	mstr_i->n = input(&istr);
	mstr_i->s = istr;
	printf("\n\ninput vars:\n");
	output(mstr_i);
	mstr_o = action(mstr_i);
	printf("\n\noutput vars:\n");
	output(mstr_o);
	erase(mstr_i);
	erase(mstr_o);
}
