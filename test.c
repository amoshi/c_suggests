
#include <stdio.h>
#include <stdlib.h>


// структура строк маттрицы
typedef struct row_t {
    int *value; // массив значений
    int length; // длина массива
} row_t;

// получение матрицы из m строк
row_t *get_matrix(int m) {
    row_t *matrix = (row_t *)malloc(m * sizeof(row_t)); // выделяем память под m строк

    for (int i = 0; i < m; i++) {
        int fl=0;
        while(fl==0){
            printf("Enter m%d: ", i + 1);
	      int k;
	      do
	      {
	      	k = scanf("%d",&matrix[i].length);
	      	if (k==0)
	      	{
	      		scanf ("%*c");
	      	}
	      }
	      while (k==0);
                  if((matrix[i].length > 0)) fl=1;
                  else printf("Error input! Enter a positive number\n");
              }

        matrix[i].value = (int *)malloc(matrix[i].length * sizeof(int)); // выделяем память под массив-строку из ni элементов
    }
    for (int i = 0; i < m; i++) {
        printf("Enter row %d: ", i + 1);

        for (int j = 0; j < matrix[i].length; j++)
	{


	      int k;
	      do
	      {
	      	k = scanf("%d",&matrix[i].value[j]);
	      	if (k==0)
	      	{
	      		scanf ("%*c");
	      	}
	      }
	      while (k==0);
	}

    }

    return matrix; // возвращаем указатель на матрицу
}

// вывод мартицы matrix из m строк на экран вместе с сообщением-комментарием message
void print_matrix(row_t *matrix, int m, const char *message) {
    printf("%s\n", message); // выводим сообщение

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < matrix[i].length; j++)
            printf("%-6d", matrix[i].value[j]); // выводим элемент matrix[i][j]

        printf("\n");
    }
}


//функция подсчета количества разных элементов в i строке матрицы
int index_of_count(row_t *matrix, int i) {
    int count=0;
    int i2=0, i1 = 0;

    int flag;                                         /*нужен для проверки, если ли
                                                      совпадения в сканируемом элементе*/

    for (i1 = 0; i1 < matrix[i].length; i1++){        //прогоняем каждый элемент строки
        flag=0;
        for (i2 = i1+1; i2 < matrix[i].length; i2++){ /*сравниваем i1 элемент с остальными элементами
                                                        строки*/
            if ((matrix[i].value[i1] == matrix[i].value[i2]) && (i1 != i2)){ /*текущий элемент(i1)
                                                                              совпал с другим? (i2)*/
                flag=1;
            }
        }
        if (flag==0) count++;
    }

    return count;

}



int main() {
    int m;                                        // число строк матрицы

    int fl=0;                                     // проверка на ввод
    while(fl==0){
        printf("Enter m: ");                          // вывод сообщения для ввода размеров матрицы
        scanf("%d", &m);
        if(m > 0) fl=1;
        else printf("Error input! Enter a positive number\n");
    }

    row_t *matrix = get_matrix(m);
    int vec[m];

    print_matrix(matrix, m, "Entered matrix: ");  // вывод матрицы

    printf ("result vector : ");
    for (int i = 0; i < m; i++) {                 //цикл формирования вектора
        vec[i] = index_of_count(matrix, i);       /*вызов функции подсчета
                                                  количества разных элементов в i строке матрицы*/
        printf("%d ", vec[i]);                    //вывод вектора

    }
    printf("\n");

    for (int i=0; i<m; i++){
        free(matrix[i].value);
    }

    return 0;
}

