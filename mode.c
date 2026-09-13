#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "piece.h"
#include "game.h"
#include "mode.h"

static int read_line(char* buf,int size)
{
    if (fgets(buf,size,stdin)==NULL)
    {
        buf[0]='\0';
        return 0;
    }

    if (strchr(buf,'\n')==NULL)
    {
        int ch;
        while ((ch=getchar())!='\n' && ch!=EOF);
    }
    else
        buf[strcspn(buf,"\n")]='\0';

    return 1;
}

/*
 * 清掉 move() 里 scanf 留在缓冲区里的本行剩余字符。
 * 只有发生升变时 move() 才会读字符,所以只在 prom!=0 时调用。
 */
static void clear_line(void)
{
    int ch;
    while ((ch=getchar())!='\n' && ch!=EOF);
}

Record* play(char board[8][8], State pState,int curr)
{
    system("clear");

    int player=curr;
    char* ctrl;
    char line[128];

    int mate=0,capture=0,c=0;
    char prom=0;
    
    Record* m=init();
    while (1)
    {
        //State_Print(&pState);
        board_print(board,player);
        

        if (check(board,player) )
        {
            printf("check");

            if (no_legal_move(board,player,&pState))
            {
                printf("mate!\n%s win\n",player?"Black":"White");
                break;
            }
            printf("\n");
        }
        else if (no_legal_move(board,player,&pState))
        {
            printf("Stalemate!\nDraw");
            break;
        }
    
        int re=0;
        
        ctrl=(char*)malloc(sizeof(char)*6);
        while (1)
        {
            if (!read_line(line,sizeof line))
            {
                re=0;
                break;
            }

            if (line[0]=='\0')
                continue;

            sscanf(line,"%5s",ctrl);

            re=move(ctrl,board,player,1,&pState,&c,&mate,&capture,&prom,' ');
            if (re)
                break;

            printf("Invalid\n");
        }

        if (!re)
        {
            free(ctrl);
            break;
        }

        if (prom!=0)
            clear_line();

        printf("%d\n",re);
        
        char* nor=transform(board,ctrl,player,capture,c,mate,prom);
        //printf("%s\n",nor);

        capture=0;
        c=0;
        mate=0;
        prom=0;

        add(m,ctrl,nor);
        
        State_Update(re,player,&pState);

        system("clear");
        player=!player;
    }

    return m;
}

void analysis(Record* M)
{
    system("clear");

    char board[8][8];
    cp_board(init_board,board);
    int curr=1;
    State pState={0,0,0,0,0,0,-2,-2};
    int mate=0,capture=0,c=0;
    char prom=0;
    char line[128];

    if (M==NULL) return ;
    MPtr check=M->head->next;

    while (1)
    {
        if (check==NULL)
            break;
        board_print(board,curr);

        printf("Next move:\n");
        MPtr varr_check=check;
        int i=1;
        while (1)
        {
            if (varr_check->next_varr==NULL)
            {
                printf("Variation %d : Move %s , standard move : %s \n",i,varr_check->detail_move,varr_check->move);
                break;
            }
                
            printf("Variation %d : Move %s , standard move : %s \n",i,varr_check->detail_move,varr_check->move);
            i++;
            varr_check=varr_check->next_varr;
        }

        printf("Command:");

        if (!read_line(line,sizeof line))
            return;

        char cmd=line[0];
        if (cmd=='\0')
            continue;

        if (cmd=='n')
        {
            int re;

            MPtr Vc=check;
            int l=0;
            while(Vc!=NULL)
            {
                Vc=Vc->next_varr;
                l++;
            }

            int v=0;
            while (1)
            {
                printf("Enter Variation:\n");

                if (!read_line(line,sizeof line))
                    return;

                if (sscanf(line,"%d",&v)==1 && v>=1 && v<=l)
                    break;

                printf("Invalid\n");
            }

            
            system("clear");

            Vc=check;
            int j;
            for (j=0;j<v && Vc->next_varr!=NULL;j++,Vc=Vc->next_varr);

            char* p=Vc->move;
            while (p!='\0' && *p!='=')
                p++;

            char a_prom=*p=='='?*(p+1):' ';
            
            re=move(Vc->detail_move,board,curr,1,&pState,&c,&mate,&capture,&prom,a_prom);

            if (prom!=0)
                clear_line();

            State_Update(re,curr,&pState);

            curr=!curr;
        }

        else if (cmd=='p')
        {
            Record* temp=play(board,pState,curr);
            system("clear");

            varr_check->next_varr=temp->head->next;
            temp->head->next->prev_varr=varr_check;
            free(temp->head);
            free(temp);

            return ;
        }

        else if (cmd=='x')
            return;

        else
            continue;

        check=check->next;
    }
}
