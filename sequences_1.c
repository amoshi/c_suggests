//Дана последовательность из n целых элементов. Сформировать новую последовательность, в которой сначала расположены в порядке возрастания все положительные элементы исходной последовательности, а затем в порядке убывания все отрицательные. Все последовательности: исходную и полученную вывести на экран.

#include <stdio.h>

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

show_element( int *b, int f )
{
	 int i;
	 printf ("length=%d\n",f);
	 for ( i=0; i<f; i++ )
	 {
		  printf("| A[%d]=%d\n",i,b[i]);
	 }
}

get_element(int *b, int c)
{
	int i;
	for ( i=0; i<c; i++ )
	{
		printf("b[%d]=",i);
		scanf("%d",&b[i]);
	}
}

void get_min_element ( int *in, int *b,  int c )
{
	for ( i=0; i<c; i++)
	{
		int tmp = a[i];
		int min = tmp;
		for ( j=0; j<
	}
}

int main()
{
	  while ( 1 )
	  {
		int c;
		int in[100], b[100];
		printf("count = ");
		scanf("%d",&c);
		if ( c == 0 ) return 0;

		if ( c < 0 )
		{
			puts("count less than 0");
			continue;
		}
		if ( c > 100 )
		{
			puts("count great than 100");
			continue;
		}

		get_element(in,c);
		sort_todown (  in,  c );
		get_min_element (  in, b,  c );


		puts("");
		puts("");
		puts("----- result -----");
		show_element(in,c);
		puts("----- ------ -----");
	}
}
