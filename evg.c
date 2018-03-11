#include <stdio.h>
#include <stdlib.h>

struct DynTableArrays {
        int count;
        int *array;
};

int input(struct DynTableArrays **tableArray) {
	int length;
	int i, j;
	struct DynTableArrays *table = NULL;
	printf("Input table length: ");
	if ((scanf("%d", &length) == 0) || (length < 0))
	    return -1;
	table = (struct DynTableArrays*)malloc(sizeof(struct DynTableArrays) * length);
	for (i = 0; i < length; ++i) {
		printf("Input array %d of %d count: ", i + 1, length);
		if ((scanf("%d", &table[i].count) == 0) || (table[i].count <= 0))
		   return -1;
		table[i].array = (int*) malloc(sizeof(int) * table[i].count);
		printf(" array of %d \n", table[i].count);
		for (j = 0; j < table[i].count; ++j) {
			printf(" %d: ", j);
			if (scanf("%d", table[i].array + j) == 0)
			   return -1;
		}
	}
	printf("\n");
	*tableArray = table;
	return length;
}

int reverse(int value) {
    int result = 0 ;
    while (value) {
      result = result * 10 + value % 10;
      value /=10;
    }
    return result;
} 

void make(struct DynTableArrays *orgTable, int length, struct DynTableArrays **newTable) {
   int i, j, k;
   struct DynTableArrays *table = NULL;
   table = (struct DynTableArrays*)malloc(sizeof(struct DynTableArrays) * length);
   for (i = 0; i < length; ++i) {
       for (j = 0, k=0; j < orgTable[i].count; ++j)
	   if(orgTable[i].array[j]%2==0)
               k++;
       table[i].count = k;
       table[i].array = calloc(sizeof(int), k);
   }
   for (i = 0; i < length; ++i) {
       for (j = 0, k=0; j < orgTable[i].count; ++j)
           if(orgTable[i].array[j]%2==0)
           {
               table[i].array[k] = orgTable[i].array[j];
               k++;
           }
   }
   *newTable = table;
}

void output(struct DynTableArrays *table, int length, char *defaultText) {
   int i, j;
   printf("%s\n", defaultText);
   for (i = 0; i < length; ++i) {
       for (j = 0; j < table[i].count; ++j)
           printf("%7d ", table[i].array[j]);
       printf("\n");
   }
   printf("\n");
}

void clear(struct DynTableArrays *table, int length) {
   int i;
   for (i = 0; i < length; ++i)
       free(table[i].array); 
   free(table);    
}

int main() {
   struct DynTableArrays *orgTable = NULL;
   struct DynTableArrays *newTable = NULL;
	int length;
	while ( (length = input(&orgTable)) ) {
      
      if (length < 0){ 
         char buf[256];
         gets(buf);
         clear(orgTable, length);
         printf("Error %d\n", length);            
         continue;
      }
      make(orgTable, length, &newTable);
      output(orgTable, length, "Original table");
      output(newTable, length, "New table");
      clear(orgTable, length);
      clear(newTable, length);
   }
   return 0;
}

