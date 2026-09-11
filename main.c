#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "piece.h"
#include "game.h"

int main()
{
    int player=1;
    char* ctrl=(char*)malloc(sizeof(char)*6);

    while (1)
    {
        //State_Print();
        board_print(temp_board,player);
        

        if (check(temp_board,player))
        {
            printf("check");

            if (no_legal_move(temp_board,player))
            {
                printf("mate!\n%s win",player?"Black":"White");
                free(ctrl);
                return 0;
            }
            printf("\n");
        }
        else if (no_legal_move(temp_board,player))
        {
            printf("Stalemate!\nDraw");
            free(ctrl);
            return 0;
        }
    
        int re;
        printf("%d\n",re);

        scanf("%5s",ctrl);
        while (!(re=move(ctrl,temp_board,player)))
        {
            printf("Invalid\n");
            scanf("%5s",ctrl);
        }
        
        State_Update(re,player);

        system("clear");
        player=!player;
    }

    free(ctrl);
    return 0;
}