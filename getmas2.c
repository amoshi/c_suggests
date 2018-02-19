/*Подключаем стандартные библиотеки*/
#include <stdio.h>
#include <stdlib.h>

/*Описываем структуру матрицы*/
struct Line 
{
	int n;
	int *a;
};
struct Matr
{
	int m;
	struct Line *p;
};

/*Заголовки функций*/
int getInt (int *);
int getLine (struct Line *);
int getMatr (struct Matr *);
void work (struct Matr *, struct Line *);
void printLine (struct Line);
void printMatr (struct Matr);
void erase (struct Matr *);

/*Основная функция*/
int main()
{
	struct Matr M;				/*Инициализация матрицы*/
	struct Line L;
	
	printf("Input matrix \n");
	if (getMatr(&M)<0)			/*Ввод матрицы*/
	{
		printf("Error\n");
		return 1;
	}
	printf("Matrix \n");
	printMatr(M);				/*Вывод матрицы*/
	work(&M, &L);					/*Задача*/
	printf("Line \n");
	printLine(L);				/*Вывод матрицы*/
	erase(&M);					/*Освобождение памяти*/
	return 0;
}

/*Функция проверки ввода целого числа*/
int getInt (int *n)
{
	int k;
	do
	{
		k = scanf("%d",n);
		if (k==0)
		{
			scanf ("%*c");
		}
	}
	while (k==0);
	return k==1?0:-1;
}


/*Функция ввода вектора*/
int getLine (struct Line *L)
{
	int i;
	printf("Input count elemenst \n");
	do
	{
		if (getInt(&L->n)<0)
			return -1;
		if (L->n<=0)
			printf("Error input. Repeat \n");
	}while (L->n<=0);
	L->a=malloc (sizeof(int)*L->n);
	for (i = 0; i < L->n; i++)
	{
		printf("Input %d element \n",i);
		if (getInt(&L->a[i])<0)
		{
			free (L->a);
			return -1;
		}
	}
	return 0;
}

/*Функция ввода матрицы*/
int getMatr (struct Matr *M)
{
	int i;
	int j;
	printf("Input count line \n");
	do
	{
		if (getInt (&M->m) < 0)
			return -1;
		if (M->m <= 0)
			printf("Error input. Repeat. \n");
	}while (M->m <= 0);
	
	
	M->p = malloc(sizeof(struct Line)*M->m);	/*Выделение памяти*/
	for (i = 0; i < M->m; i++)
	{
		printf("Input %d line \n", i);
		if (getLine (&M->p[i]) < 0)
		{
			for (j=0; j < i; j++)
				free (&(M->p[j]));
			free (M->p);
			return -1;
		}
	}
	return 0;
}

/*Функция задачи*/
void work (struct Matr *M, struct Line *L)
{
	int i;
	int j;
	int l;
	int buf;
	L->n = M->m;
	L->a=malloc (sizeof(int)*L->n);
	for(i=0; i < M->m; i++)
	{
		/*Сортировка элементов строк матрицы*/
		for (j=0; j < (M->p[i].n - 1); j++)
		{
			for (l=j+1; l < M->p[i].n; l++)
			{
				if (M->p[i].a[j] > M->p[i].a[l])
				{
					buf = M->p[i].a[j];
					M->p[i].a[j] = M->p[i].a[l];
					M->p[i].a[l] = buf;
				}
			}
		}
		buf = M->p[i].a[0];
		L->a[i] = 1;
		for (j = 1; j < M->p[i].n; j++)
		{
			if (M->p[i].a[j]!=buf)
			{
				buf = M->p[i].a[j];
				L->a[i]++;
			}
		}
	}
}


/*Функция вывода вектора*/
void printLine (struct Line L)
{
	int i;
	for (i = 0; i < L.n; i++)
	{
		printf("%d ", L.a[i]);
	}
	printf("\n");
}


/*Функция вывода матрицы*/
void printMatr (struct Matr M)
{
	int i;
	for (i = 0; i < M.m; i++)
	{
		printLine (M.p[i]);
	};
}

/*Функция освобождения памяти*/
void erase (struct Matr *M)
{
	int i;
	for (i = 0; i < M->m; i++)
		free (M->p[i].a);
	free (M->p);
	M->m = 0;
	M->p = NULL;
}
