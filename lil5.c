#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct LString
{
    char s;
    struct LString *next;
}LString;

LString *erase(LString *mylist)
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

int strInput(LString ** mylist)
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

void strOutput(LString *mylist)
{
    printf("\"");
    for(;mylist!=NULL;mylist=mylist->next)
        printf("%c",mylist->s);
    printf("\"\n");
};

void nulldetector(LString **mylist)
{
    int flag = 1;
    int flagstart = 1;
    LString *next = *mylist;
    LString *prev = *mylist;
    LString *ptr = *mylist;
    for(;next;ptr=next)
    {
        next = ptr->next;
        if (flag == 1 && ptr->s == '0')
        {
            if ( flagstart )
            {
                *mylist = next;
            }
            prev->next = next;
            continue;
        }
        else
        {
            flag = 0;
            flagstart = 0;
        }
        if ( ptr->s == ' ' )
            flag = 1;
        prev = ptr;
    }
};

LString* palindrom(LString *mylist)
{
	LString *ptr;
	LString *def;
	LString *cur;
	LString *tmp1;
	LString *tmp2;
	LString *new = NULL;
	LString *newptr = NULL;
	for ( ptr = mylist; ptr; ptr = ptr->next )
	{
		tmp1 = malloc(sizeof(LString));
		tmp1->next = NULL;
		int c, d;
		def = ptr;
		for ( c = 0; ptr && ptr->s != ' ' && ptr->s != '\t'; ptr = ptr->next, c++ )
		{
			tmp1->s = ptr->s;
			//printf("put %c(%p)\n", tmp1->s, tmp1);
			tmp2 = malloc(sizeof(LString));
			tmp2->next = tmp1;
			tmp1 = tmp2;
		}
		tmp1 = tmp1->next;
		free(tmp2);
		tmp2 = tmp1;
		//printf("c %c(%p)\n", def->s, def);
		//printf("t %c(%p)\n", tmp1->s, tmp1);

		for ( d = 0, cur = def; tmp1 && cur && tmp1->s == cur->s; printf("%c:%c\n", tmp1->s, cur->s ), tmp1 = tmp1->next, cur = cur->next, d++);

		//printf("d=%d c=%d\n", d, c);
		if ( d == c )
		{
			//puts("palindrome!");
			if ( !new )
			{
				new = newptr = tmp2;
				while ( new->next ) new = new->next;
				new->next = NULL;
			}
			else
			{
				new->next = malloc(sizeof(LString));
				new = new->next;
				new->s = ' ';
				new->next = tmp2;
				while ( new->next ) new = new->next;
				new->next = NULL;
			}
		}
		if ( !(ptr) )
			break;
	}
	return newptr;
}

LString* palindr(LString *mylist)
{
    LString *ptr;
    LString *start = mylist;
    LString *tmp = malloc ( sizeof(LString) );
    LString *tmp1 = tmp;
    tmp->next = NULL;

    while ( mylist )
    {
        while((mylist) && (mylist->s!=' ') && (mylist->s!='\t'))
        {
            tmp1->s = mylist->s;
            mylist = mylist->next;
            tmp = malloc ( sizeof(LString) );
            tmp->next = tmp1;
            tmp1 = tmp;
        }
        if ( !mylist )
            break;
        tmp = malloc ( sizeof(LString) );
        tmp1->s = ' ';
        tmp->next = tmp1;
        tmp1 = tmp;
        if ( mylist->next )
            mylist = mylist->next;
    }

    tmp = tmp->next;
    free(tmp1);

    strOutput(tmp);

    mylist = start;
    LString *tmpstart = tmp;
    
    while ( mylist )
    {
        int flag = 0;
        while((mylist) && (mylist->s!=' ') && (mylist->s!='\t'))
        {
            printf("comparing %c and %c\n", tmp->s, mylist->s );
            if ( tmp->s != mylist->s )
            {
                flag = 1;
                break;
            }
            mylist = mylist->next;
            tmp = tmp->next;
        }
        if ( flag )
            puts("NO palindrome");
        if ( !flag )
            puts("palindrome");
        if ( mylist && mylist->next )
            mylist = mylist->next;
        if ( tmp && tmp->next )
            tmp = tmp->next;
    }

    return ptr;
};

LString *deleteSpace(LString *mylist)
{
    LString *line;
    while(mylist && (mylist->s == ' ' || mylist->s == '\t'))
    {
        line = mylist;
        mylist=mylist->next;
        free(line);
    }
    return mylist;
};

LString *nextWord(LString *mylist)
{
    while((mylist->next) && (mylist->next->s!=' ') && (mylist->next->s!='\t'))
    {
        mylist = mylist->next;
    }
    return mylist;
}

LString *changeWord(LString *mylist)
{
    LString *p, *newlist, *cur;
    p = mylist;
    newlist=mylist;
    cur=mylist;
    while((cur) && (cur->s!=' ') && (cur->s!='\t'))
    {
        cur = cur->next;
    }
    mylist=mylist->next;
    p->next = cur;
    while((mylist) && (mylist->s!=' ') && (mylist->s!='\t'))
    {
        cur=mylist;
        mylist=mylist->next;
        cur->next=p;
        p = cur;
        //strOutput(p);
    }
    /*while((p->next) && (p->next->s!=' ') && (p->next->s!='\t'))
    {
        p = p->next;
    }*/
    mylist=p;
    //strOutput(p);
    //strOutput(mylist);
    return mylist;
}

int lenWord(LString *mylist)
{
    LString *line;
    int len = 0;
    line = mylist;

    while((line->next) && (line->next->s!=' ') && (line->next->s!='\t'))
    {
        line = line->next;
        len++;
    }
    return len;
}

LString *skipWord(LString *mylist)
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

LString *work(LString *mylist)
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
        cursym->next = changeWord(cursym->next);
        //strOutput(cursym);
        cursym = nextWord(cursym->next);
        //strOutput(cursym);
        lastsym = cursym;
        cursym = cursym->next;
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

int main()
{
    LString *mylist = NULL;
    while(printf("enter your numbers: "), strInput(&mylist))
    {
        /*if(mylist == NULL)
        {
            mylist = erase(mylist);
            continue;
        }*/
        printf("inputted string: ");
        strOutput(mylist);
        mylist = work(mylist);
        nulldetector(&mylist);
        printf("result: ");
        strOutput(mylist);
        LString *pal = palindrom(mylist);
	puts("palindrom");
        strOutput(pal);
	puts("erase");
        mylist = erase(mylist);
    }
    return 0;
}

