// Дана последовательность из n целых элементов. Сформировать новую последовательность, состояющую из минимальных цифр элементов упорядоченной по убыванию исходной последовательности. Все последовательности: исходную и полученную вывести на экран.

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

void show_element( int *b, int f )
{
	 int i;
	 printf ("length=%d\n",f);
	 for ( i=0; i<f; i++ )
	 {
		  printf("| A[%d]=%d\n",i,b[i]);
	 }
}

void get_element(int *b, int c)
{
	int i;
	for ( i=0; i<c; i++ )
	{
		printf("b[%d]=",i);
		scanf("%d",&b[i]);
	}
}

void get_min_element ( int *a, int *b,  int c )
{
	int i;
	for ( i=0; i<c; i++)
	{
		int tmp = 1;
		int min = a[i];
		int y = 1;
		int j;
		//puts("----");
		tmp = a[i]/y %10;
		for ( j=0; tmp>0; j++ )
		{
			//printf("tmp=%d\n", tmp);
			if ( min > tmp)
				min = tmp;
			y=y*10;
			tmp = a[i]/y %10;
		}
		b[i]=min;
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
		puts("----- array -----");
		show_element(in,c);
		puts("");
		puts("----- result -----");
		show_element(b,c);
		puts("----- ------ -----");
	}
}
