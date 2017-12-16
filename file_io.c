// Дан текстовый файл. Сформировать новый файл, содержащий нечетные строки исходного файла. Программа должна производить проверку особых ситуаций при операциях ввода-вывода.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define BF 300

char *get_ans(FILE *fd)
{
	char *tmp = malloc(BF);
	char *ret = malloc(BF*10);
	ret[0]='\0';
	int i;
	for ( i=1; fgets(tmp, BF, fd); i++ )
	{
		//printf("word=%s\n", tmp);
		if ( strlen(tmp) < 1 )
		{
			break;
		}
		if ( isspace(tmp[0]) )
		{
			continue;
		}
		if ( tmp[strlen(tmp)-1] != '\n' )
		{
			printf("file too long\n");
			break;
		}


		if ( i%2==0 )
		{
			strcat(ret, tmp);
		}
	}
	return ret;
}

int main()
{
	char *str = malloc ( 1000 );
	char *res;
	FILE *fd, *fd_out;
	size_t str_size = 0;
	while ( 1 )
	{
		puts("=======");
		puts("text:");
		gets(str);
		str_size = strlen(str);
		if (str_size == 0) return 1;
		{
			if ( ( fd = fopen(str, "r") ) == NULL )
			{
				perror("fopen");
				return 2;
			}
			if ( ( fd_out = fopen("f_out", "w") ) == NULL )
			{
				perror("fopen");
				return 2;
			}
			res = get_ans(fd);
			res[strlen(res)-1]='\0';
			fclose(fd);
			fprintf(fd_out, "answer:\n'%s'\n", res);
			free(res);
			fclose(fd_out);
		}
	}
}
