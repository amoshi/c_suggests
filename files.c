#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAXSTR 100
#define MAXLEN 100

void m_free(char **m, size_t n)
{
	int i;
	for (i=0; i<n; i++)
	{
		free(m[i]);
	}
	free(m);
}

void res_print(char **m, size_t n)
{
	int i;
	for (i=0; i<n; i++)
	{
		printf("%s\n", m[i]);
	}
}

char** get_ans(FILE *fd, size_t *k)
{
	char **tmp = malloc(MAXSTR);
	char *field = malloc(MAXLEN);
	int i;
	for ( i=0; fgets(field, MAXLEN, fd); i++ )
	{
		if ( strlen(field) < 1 )
		{
		    break;
		}
		if ( isspace(field[0]) )
		{
		    continue;
		}
		if ( field[strlen(field)-1] != '\n' )
		{
		    printf("file too long\n");
		    break;
		}

		size_t n = strlen(field);
		tmp[i] = malloc(n);
		strncpy(tmp[i], field, n-1);
	}
	*k = i;
	i--;
	return tmp;
}

int main()
{
	char *str = malloc ( MAXLEN );
	FILE *fd;
	size_t n;
	char **res;
	size_t str_size = 0;
	while ( 1 )
	{
		puts("=======");
		puts("filename:");
		gets(str);
		str_size = strlen(str);
		if (str_size == 0) return 1;
		{
			printf("trying to open %s to read\n", str);
			if ( ( fd = fopen(str, "r") ) == NULL )
			{
				perror("fopen");
				return 2;
			}
			res = get_ans(fd, &n);
			res_print(res, n);
			m_free(res, n);
			fclose(fd);
		}
	}
}
