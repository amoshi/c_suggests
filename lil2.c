#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct LString
{
    char s;
    struct LString *next;
}LString;

LString *erase(LString *mylist)// очистка памяти
{
    LString *str = NULL;
    while (str != NULL)
    {
        str = mylist;
        mylist=mylist->next;
        free(str);
    }
    return mylist;
}

int strInput(LString ** mylist)//ввод строки
{
    char *str, buf[21];
    LString head = {'-',NULL};
    LString *tail = &head;
    int n, result=1;

    do
    {
        //scanf("%*c");
        n = scanf("%20[^\n]", buf);
        if (n<0)
        {
            erase(head.next);
            head.next = NULL;
            result = 0;
            continue;
        }
        else if (n>0)
        {
            for(str=buf;*str!='\0';++str)
            {
                tail->next = (LString*)malloc(sizeof(LString));
                tail = tail->next;
                tail->s = *str;
            }
            tail->next = NULL;
        }
        else
            scanf("%*c");
    }while(n>0);
    *mylist = head.next;
    return result;
};

void strOutput(LString *mylist)//вывод строки
{
    printf("\"");
    for(;mylist!=NULL;mylist=mylist->next)
        printf("%c",mylist->s);
    printf("\"\n");
};

LString* deleteZero(LString *mylist)// удаление пробелов из списка
{
    LString *line, *cur;
    cur = mylist;
    while(cur != NULL){

        if ((cur->s == ' ' || cur->s == '\t') && (cur->next->s == '0'))
        {
            line = cur->next;
            cur=cur->next;
            free(line);
        }
        cur=cur->next;
    }
    return mylist;
};

LString *nextWord(LString *mylist)//
{
    while((mylist->next) && (mylist->next->s!=' ') && (mylist->next->s!='\t'))
    {
        mylist = mylist->next;
    }
    return mylist;
}

int lenWord(LString *mylist)// длина слова
{
    LString *line;
    int len = 0;
    line = mylist;

    while((line->next) && (line->next->s!=' ') && (line->next->s!='\t'))
    {
        line = line->next;
        if( (line->s!=' ') && (line->s!='\t') )len++;
    }
    return len;
}

LString *skipWord(LString *mylist)// пропуск слова в строке
{
    LString *line;
    while((mylist) && (mylist->s!=' ') && (mylist->s!='\t'))
    {
        line = mylist;
        mylist = mylist->next;
        free(line);
    }
    return mylist;
}

/*LString *work2(LString *mylist)// формирование  новой строки
{
    LString head = {'-', mylist};
    LString *cursym = &head, *lastsym = &head;
    int len = 0;
    cursym->next = deleteSpace(cursym->next);
    //strOutput(cursym);
    len = lenWord(cursym);
    //cursym->next = skipWord(cursym->next);
    while (cursym && (cursym->next = deleteSpace(cursym->next)))
    {
        if(lenWord(cursym) == len)
        {
            cursym = nextWord(cursym->next);
            lastsym = cursym;
            cursym = cursym->next;
        }
        else
        {
            //printf("word1 = %d, curword = %d\n", len, lenWord(cursym->next));
            cursym->next = skipWord(cursym->next);
        }

        if(cursym)
            cursym->s = ' ';

    }
    if(lastsym->next)
    {
        free(lastsym->next);
        lastsym->next = NULL;
    }
    return head.next;
};
LString *work1(LString *mylist)// формирование  новой строки
{
    LString head = {'-', mylist};
    LString *cursym = &head, *lastsym = &head;
    int len = 0;
   while (cursym && (cursym->next = deleteSpace(cursym->next)))
   {
      len = lenWord(cursym);
        for(i=0;len;i++)
           cursym->s=cursym->(s-i-1);

    }*/

/*LString *work(LString *mylist)// формирование  новой строки
   {
       LString head = {'-', mylist};
       LString *cursym = &head, *lastsym = &head;
       int len = 0;
       int i=0;
       printf("a1");
       cursym->next = deleteSpace(cursym->next);
       //strOutput(cursym);
       len = lenWord(cursym);
       //cursym->next = skipWord(cursym->next);
       printf("a");
       while (cursym && (cursym->next = deleteSpace(cursym->next)))
       {
           printf("b");
           for(i=0;i<=len;i++)
              cursym->s=cursym->s-i-1;
           printf("c");


           if(cursym)
               cursym->s = ' ';

       }
       if(lastsym->next)
       {
           free(lastsym->next);
           lastsym->next = NULL;
       }
       return head.next;
   };*/
//int i=0;

/* Два буфера : один хранит число до пробела, уже инвертированное;
 * второй хранит новый одновсязный список с пробелами.
 * Когда новый список посчитан, буфер очищается
*/
LString* printFromTail(LString *mylist) {
    char bufword[16];
    int len=0;

    LString *newlist = (LString*) malloc (sizeof(LString));
    LString *cur = newlist;

    while (mylist != NULL){


        len=lenWord(mylist)+1;

        int ibuf = len-1;
        for(int i = 0; i<len; i++){

            bufword[ibuf] = mylist->s;
            ibuf--;
            mylist=mylist->next;
        }
        //до текущего момента записал в буфер число
        if ( mylist && mylist->s )
            	while(mylist->s == ' ' || mylist->s == '\t')
		{
			if ( mylist->next)
				mylist=mylist->next;
			else
				break;
		}


        for(int j = 0; j<len; j++){
            if (bufword[0]!=' ' && bufword[0] != '\t'){
                cur->s = bufword[j];
                cur->next = malloc(sizeof(LString));
                cur = cur->next;
            }
        }
        if( mylist && mylist->next && (bufword[0] != ' ' || bufword[0] != '\t')){
            cur->next = malloc(sizeof(LString));
            cur->s = ' ';
            cur = cur->next;
        }
    }

    cur->next=NULL;
    cur=newlist;

    //удаление последнего (лишнего) элемента
    while(cur!=NULL){

        if(cur->next != NULL){
            if(cur->next->next == NULL){
               cur->next = NULL;
                break;
            }
        } else if (cur->next == NULL){
            cur->s = 'n';
            break;
        }
        cur=cur->next;
    }

    //переопределения указателя основного списка и очистка буферного
    mylist = newlist;
    newlist = erase(newlist);

    return mylist;
}
int main()
{
    LString *mylist = NULL;
    while(printf("Введите строку: "), strInput(&mylist))
    {
        /*if(mylist == NULL)
        {
            mylist = erase(mylist);
            continue;
        }*/
        printf("Введенная строка: ");
        strOutput(mylist);



        while(mylist->s == ' ' || mylist->s =='\t'){
            mylist=mylist->next;
        }
        mylist = printFromTail(mylist);

        printf("Модифицированная строка: ");
        strOutput(mylist);
        mylist = erase(mylist);
    }
    return 0;
}

