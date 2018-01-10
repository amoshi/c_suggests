#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define BUF 20

void sort_toup ( int *a, int n )
{
	int i, j, tmp;
	for (i=1; i<n; i++, a[j+1]=tmp )
	{
		for ( tmp=a[i], j=i-1; j>=0 && tmp<a[j];a[j+1]=a[j],j-- );
	}
}
void sort_todown ( int *a, int n )
{
	 int i, j, tmp;
	 for (i=1; i<n; i++, a[j+1]=tmp )
	 {
		 for ( tmp=a[i], j=i-1; j>=0 && tmp>a[j];a[j+1]=a[j],j-- );
	 }
}

void wr(int *a, int n)
{
	//printf("wr enter %d\n", n);
	int i;
	for (i=0; i<n; i++)
		printf("answ[%d] = %d\n", i, a[i]);
	//puts("wr exit");
}

void sort(int *a, int n)
{
	int *b = malloc(n), *c = malloc(n);
	int p = 0, o = 0;
	int i;
	for ( i=0; i<n; i++ )
		if ( a[i]>0 )
			b[p++] = a[i];
		else if ( a[i]<0 )
			c[o++] = a[i];
	sort_todown(b, p);
	sort_toup(c, o);
	wr(b, p);
	wr(c, o);
	free(b);
	free(c);
}

int write_file(FILE *ptr_myfile)
{
	int i;
	int my_record[BUF];

	for ( i=0; i<BUF; i++ )
		my_record[i] = (int)pow(-2, i);
	for ( i=0; i<BUF; i++ )
		printf("write %d\n", my_record[i]);
	for ( i=0; i<BUF; i++ )
		fwrite(&my_record[i], sizeof(my_record), 1, ptr_myfile);
	fclose(ptr_myfile);
	return 0;
}

int *get_ans(FILE *ptr_myfile)
{
	int counter;
	int *my_record = malloc(100);

	if (!ptr_myfile)
	{
		printf("Unable to open file!");
		return NULL;
	}
	fread(my_record,sizeof(int),BUF,ptr_myfile);
	//for ( counter=0; counter < BUF; counter++)
	//{
	//	printf("%d\n",my_record[counter]);
	//}
	fclose(ptr_myfile);
	return my_record;
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
		puts("filename:");
		gets(str);
		str_size = strlen(str);
		if (str_size == 0) return 1;
		{

			fd_out=fopen(str,"wb");
			if (!fd_out)
			{
				perror("fopen: Unable to open file!");
				return 1;
			}
			write_file(fd_out);


			if ( ( fd = fopen(str, "rb") ) == NULL )
			{
				perror("fopen");
				return 2;
			}
			int *digits = get_ans(fd);
			sort(digits, BUF);
			free(digits);
		}
	}
}
