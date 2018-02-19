#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#define ARG_INPUT_MAXSIZE 1024000

typedef struct mstring_t
{
	char **s;
	size_t n;
} mstring_t;

//char *normalizer(char *s, size_t l)
//{
//	printf("str is '%s' (%zu) \n", s, l);
//	uint64_t i, j;
//	char *ret = malloc (l);
//	for ( i=strspn(s," \t"), j=0; i<l; i++ )
//	{
//		if ( strspn(s+i," \t") > 1 )
//			continue;
//		ret[j]=s[i];
//
//		j++;
//	}
//	if ( strspn(s+j," \t") > 0 )
//		ret[j-1]='\0';
//	else    ret[j]='\0';
//
//	printf("ret is '%s' (%zu)\n", ret, strlen(ret));
//	return ret;
//}

void output (mstring_t *mstr_i)
{
	size_t n = mstr_i->n;
	int64_t i;
	for ( i=0; i<n; i++ )
	{
		printf("[%"PRIu64"] : '%s'\n", i, mstr_i->s[i]);
	}
}

size_t input (char ***istr)
{
	char *arg = malloc( ARG_INPUT_MAXSIZE );

	istr[0] = malloc( (sizeof(char*))*10000 );
	int64_t i;

	for ( i=0; fgets(arg, ARG_INPUT_MAXSIZE, stdin); i++ )
	{
		puts("============");
		//printf("arg=%s\n", arg);
		size_t arg_len = strlen(arg);
		//istr[0] = realloc( (istr[0]), (sizeof(char*))*i );
		//printf("allocating %zu\n", arg_len);
		malloc(arg_len);
		istr[0][i] = malloc(arg_len);
		strlcpy(istr[0][i],arg,arg_len);
		//printf("\targ=%s\tistr=%s\n", arg, istr[0][i]);
	}
	return i;
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
		for ( j=0, k=0; j<sn+1; j++ )
		{
	//		if ( ( mstr_i->s[i][j] == '1' && mstr_i->s[i][j+1] == '0' ) || ( mstr_i->s[i][j] == '0' && mstr_i->s[i][j+1] == '1' ) )
			if ( ( !strncmp(mstr_i->s[i]+j, "1", 1) && !strncmp(mstr_i->s[i]+j+1, "0", 1) ) || ( !strncmp(mstr_i->s[i]+j, "0", 1) && !strncmp(mstr_i->s[i]+j+1, "1", 1) ) )
			{
				//printf("%% %c\n", mstr_i->s[i][j]);
				//printf("%% %c\n", mstr_i->s[i][j+1]);
				newmstr->s[i][k] = mstr_i->s[i][j];
				newmstr->s[i][k+1] = mstr_i->s[i][j+1];
				//printf("newmstr->s[%"PRIu64"][%"PRIu64"]=%c\n", i, k, newmstr->s[i][k]);
				//printf("newmstr->s[%"PRIu64"][%"PRIu64"]=%c\n", i, k+1, newmstr->s[i][k+1]);
				k+=2;
				j++;
			}
		}
		newmstr->s[i][k] = '\0';
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
	puts("input vars:");
	output(mstr_i);
	mstr_o = action(mstr_i);
	puts("output vars:");
	output(mstr_o);
}
