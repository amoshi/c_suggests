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

void __print( int *b, int f )
{
	int i;
	printf ("length=%d\n",f);
	for ( i=0; i<f; i++ )
	{
		printf("| A[%d]=%d\n",i,b[i]);
	}
}

int deduplicator(int *b, int c, int *h)
{
	int i, j, k = 0;
	for ( i=0; i<c; i++ )
	{
		for ( j=i+1; j<c; j++ )
		{
			if ( b[i] == b[j] )
				break;
		}
		if ( j == c )
			h[k++] = b[i];
	}
	return k;
}

void __scan(int *b, int c)
{
	int i;
	for ( i=0; i<c; i++ )
	{
		printf("b[%d]=",i);
		scanf("%d",&b[i]);
	}
}

int main()
{
	while ( 1 )
	{
		int c;
		int in[100], h[100];
		int i,j,cc;
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
			puts("count bigger than 100");
			continue;
		}

		__scan(in,c);
		
		cc = deduplicator ( in, c, h );
		sort_todown ( h, cc );


		puts("");
		puts("");
		puts("----- anser -----");
		__print(h,cc);
	}
}
