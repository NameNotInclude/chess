#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "piece.h"
#include "game.h"


int main()
{
    int player=1;
    char* ctrl;
    
    Record* m=init();
    while (1)
    {
        //State_Print();
        board_print(temp_board,player);
        

        if (check(temp_board,player) )
        {
            printf("check");

            if (no_legal_move(temp_board,player))
            {
                printf("mate!\n%s win",player?"Black":"White");
                break;
            }
            printf("\n");
        }
        else if (no_legal_move(temp_board,player))
        {
            printf("Stalemate!\nDraw");
            break;
        }
    
        int re;
        //printf("%d\n",re);

        ctrl=(char*)malloc(sizeof(char)*6);
        scanf("%5s",ctrl);
        while (!(re=move(ctrl,temp_board,player,1)))
        {
            printf("Invalid\n");
            scanf("%5s",ctrl);
        }

        char* nor=transform(temp_board,ctrl,player,capture,c,mate,prom);
        printf("%s\n",nor);

        capture=0;
        c=0;
        mate=0;
        prom=0;

        add(m,ctrl,nor);
        
        State_Update(re,player);

        system("clear");
        player=!player;
    }

    free_record(m);
    return 0;
}