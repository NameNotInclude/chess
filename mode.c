#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "piece.h"
#include "game.h"
#include "mode.h"

Record* play(char board[8][8], State pState)
{
    int player=1;
    char* ctrl;

    int mate=0,capture=0,c=0;
    char prom=0;
    
    Record* m=init();
    while (1)
    {
        State_Print(pState);
        board_print(board,player);
        

        if (check(board,player) )
        {
            printf("check");

            if (no_legal_move(board,player,pState))
            {
                printf("mate!\n%s win",player?"Black":"White");
                break;
            }
            printf("\n");
        }
        else if (no_legal_move(board,player,pState))
        {
            printf("Stalemate!\nDraw");
            break;
        }
    
        int re;
        //printf("%d\n",re);

        ctrl=(char*)malloc(sizeof(char)*6);
        scanf("%5s",ctrl);
        while (!(re=move(ctrl,board,player,1,pState,&c,&mate,&capture,&prom)))
        {
            printf("Invalid\n");
            scanf("%5s",ctrl);
        }

        char* nor=transform(board,ctrl,player,capture,c,mate,prom);
        //printf("%s\n",nor);

        capture=0;
        c=0;
        mate=0;
        prom=0;

        add(m,ctrl,nor);
        
        State_Update(re,player,pState);

        system("clear");
        player=!player;
    }

    return m;
}

void analysis(Record* M)
{
    char board[8][8];
    cp_board(init_board,board);
    int curr=1;
    State pState={0,0,0,0,0,0,-2,-2};
    int mate=0,capture=0,c=0;
    char prom=0;

    if (M==NULL) return ;
    MPtr check=M->head;

    char* ctrl;
    int re;
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
            if (varr_check==NULL)
                break;
            printf("Variation %d : Move %s , standard move : %s \n",i,varr_check->detail_move,varr_check->move);
            i++;
            varr_check=varr_check->next_varr;
        }

        char cmd;
        printf("Command:");
        scanf(" %c", &cmd);

        if (cmd=='a')
        {
            printf("Enter new move:\n");
            ctrl=(char*)malloc(sizeof(char)*6);
            scanf("%5s",ctrl);

            while (!(re=move(ctrl,board,curr,1,pState,&c,&mate,&capture,&prom)))
            {
                printf("Invalid\n");
                scanf("%5s",ctrl);
            }
        }
    }
}