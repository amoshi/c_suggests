#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int MAX_SIZE = 20;

typedef struct Table{	
	int key;
	char *info;
} Table;

int plan = 0;
char file[80];
const char *msgs[] = {"0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show"};	
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);	

char *getStr();	
int getInt(int *);	
int insert(Table *, int, char *);	
void delTable(Table *);	 
int dialog(const char *msgs[], int);	
int D_Add(Table *);	 
int D_Find(Table *);	
int D_Delete(Table *);
int D_Show(Table *);
int D_Load(Table *);
int D_Save(Table *);

int (*fptr[])(Table *) = {NULL, D_Add, D_Find, D_Delete, D_Show};

int main()
{
	Table table[MAX_SIZE];
	int rc;
	int i;
	for (i = 0; i < MAX_SIZE; i++)
	{
		table[i].key = 0;
		table[i].info = NULL;
	}
	          printf("Enter name file:");
          scanf("%s",file);

        D_Load(table);
        while ( (rc = dialog(msgs, NMsgs)) )
	{
		if(!fptr[rc](table))
		{
			break;	
		}
	}
	printf("That's all. Bye!\n");
		D_Save(table);
	delTable(table);
	return 0;
}

char *getStr()
{
	char *out;
	out = malloc(sizeof(char) * 80);
	scanf("%s", out);
	return out;
}

int getInt (int *n)
{
	int k;
	do
	{
		k = scanf("%d", n);
		if (!k)
		{
			scanf ("%*c");
		}
	}
	while (!k);
	return k;
}

int insert(Table *pTab, int key, char *info)
{
	int i;
	int j;
	for(i = 0; (pTab[i].key!=0) && (pTab[i].key!=key) && (i!=MAX_SIZE) && (pTab[i].key < key); i++);
	if (i==MAX_SIZE)
	{
		printf("Table is full");
		return 0;
	}
    if (pTab[i].key==key)
    {
        printf("This key already exists");
        return 0;
    }
	if (pTab[i].key!=0)
	{
		for (j = i; (pTab[j].key!=0) && (j!=MAX_SIZE);j++);
		if (i==MAX_SIZE)
		{
			printf("Table is full");
			return 0;
		}
		for (;j!=i;j--)
		{
			pTab[j].key = pTab[j-1].key;
			pTab[j].info = pTab[j-1].info;
		}
		
	}
	pTab[i].key = key;
	pTab[i].info = malloc(sizeof(char)*(strlen(info)+1));
	strcpy(pTab[i].info,info);
	
	
	return 1;
}

void delTable(Table *pTab)
{
	int i;
	for (i = 0; i < MAX_SIZE; i++)
	{
		pTab[i].key = 0;
        free(pTab[i].info);
        pTab[i].info = NULL;
	}
}

int dialog(const char *msgs[], int N)
{
	char *errmsg = ""; 
	int rc;
	int i;
	int n;
	do
	{
		puts(errmsg);
		errmsg = "You are wrong. Repeate, please!";
		for(i = 0; i < N; ++i)
		{
			puts(msgs[i]);
		}
		printf("Make your choice: --> ");
		n = getInt(&rc); 
		if(!n) 
		{
			rc = 0;
		}
	} 
	while(rc < 0 || rc >= N);
	return rc;
}

int D_Add(Table *ptab)
{
	int k;
	int rc;
	int n;
	char *info = NULL;
	printf("Enter key: -->");
	n = getInt(&k);
	if(!n)
	{
		return 0; 
	}
	printf("Enter info: -->");
	info = getStr(); 
	if(!info)
	{
		return 0;	
	}
	rc = insert(ptab, k, info);	
	free(info); 
	return 1;
}

int D_Find(Table *ptab)
{
    int first = 0;
    int last;
	int x;
	int mid;
	int i;
	int n;
    do
	{
		printf("Enter key:");
        getInt(&x);
	}
	while(x==0);
	for (n=0, i=0; i < MAX_SIZE;i++)
	{
		if ( ptab[i].key != 0 )
			n++;
	}
	last = n;
        if (n == 0) 
	{
		printf("Database is empty\n");
                return 1;
        } 
	else if (ptab[0].key > x) 
	{
		printf("No matches\n");
        return 1;
    } 
	else if (ptab[n - 1].key < x) 
	{
        printf("No matches\n");
        return 1;
    }
    while (first < last) 
	{
        mid = first + (last - first) / 2;
 
        if (x <= ptab[mid].key)
            last = mid;
        else
            first = mid + 1;
    }
    if (ptab[last].key == x) 
	{
        printf("key = %d\tstring = \"%s\"\n",ptab[last].key,ptab[last].info);
        return 1;
    } 
	else 
	{
        printf("No matches\n");
        return 1;
    }
}

int D_Delete(Table *ptab)
{
	int i;
	int j;
     int iot;
     int ido;
     int fl = 0;

     do
	{
		printf("Enter the range of keys (X<=Y):");
          getInt(&iot);
          getInt(&ido);
	} 
	while(iot > ido);
	for (i=0; ptab[i].key != 0;)
     {
          if ((ptab[i].key >= iot)&&(ptab[i].key <= ido))
          {
			for (j=i; ptab[j].key != 0; j++)
               {
                    ptab[j].key = ptab[j+1].key;
                    ptab[j].info = ptab[j+1].info;
                    
               };
			j--;   
               ptab[j].key = 0;
               free(ptab[j].info);
               ptab[j].info = NULL;
               fl = 1;
          }
          else
          {
           i++;
          }
     }
	if (!fl)
	{
		printf("No matches\n");
	}
	return 1;
	
}

int D_Show(Table *ptab)
{
	int i;
     for (i=0; i < MAX_SIZE; i++)
     {
          if (ptab[i].key!=0)
          {
               printf("key = %d\tstring = \"%s\"\n",ptab[i].key,ptab[i].info);
          }
     }
	return 1;
}


int D_Load(Table *ptab)
{
	int i;
	int len_word[MAX_SIZE];
	FILE *in_file;
        in_file = fopen(file, "r+b");
        if ( !in_file )
	{
		puts("file not found");
		return 0;
	}
	for (i = 0; i < MAX_SIZE; i++)
	{
		ptab[i].key = 0;
		ptab[i].info = NULL;
                len_word[i] = 0;
	}
	for (i = 0; i < MAX_SIZE; i++)
	{
		fread(&ptab[i].key, sizeof(int), 1, in_file);        
		fread(&len_word[i], sizeof(int), 1, in_file);			
                ptab[i].info = malloc(sizeof(char)*(len_word[i]+1));
        }
     
	for (i = 0; i < MAX_SIZE; i++)
	{
          if (ptab[i].key != 0)
          {
               fread(ptab[i].info, sizeof(char), len_word[i], in_file);
               ptab[i].info[len_word[i]] = '\0';
          }
	}
     
	fclose(in_file);
	return 1;
}

int D_Save(Table *ptab)
{
	int i;
	int buf;
	FILE *out_file;
	out_file = fopen(file, "w+b");
	for (i = 0; i < MAX_SIZE; i++)
	{
          if (ptab[i].key != 0)
          {
               fwrite(&ptab[i].key, sizeof(int), 1, out_file);	
               buf = strlen(ptab[i].info);          
               fwrite(&buf, sizeof(int), 1, out_file);
          }
          else
          {
               buf = 0;
               fwrite(&buf, sizeof(int), 1, out_file);	      
               fwrite(&buf, sizeof(int), 1, out_file);
          }
	}
	for (i = 0; i < MAX_SIZE; i++)
	{
          if (ptab[i].key != 0)
          {
               fwrite(ptab[i].info,sizeof(char),strlen(ptab[i].info),out_file);
          }
	}
	fclose(out_file);
	return 1;
}

