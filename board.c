#include <stdio.h>
#include "board.h"
#include "piece.h"

#define VALID_POS(x,y) ((x)>=0 && (x)<8 && (y)>=0 && (y)<8)
// 1 for white to play, 0 for black.
int player = 1;

// a   b   c   d   e   f   g   h 
char temp_board[8][8] = {{'r','n','b','q','k','b','n','r'},
                         {'p','p','p','p','p','p','p','p'},
                         {'-','-','-','-','-','-','-','-'},
                         {'-','-','-','-','-','-','-','-'},
                         {'-','-','-','-','-','-','-','-'},
                         {'-','-','-','-','-','-','-','-'},
                         {'P','P','P','P','P','P','P','P'},
                         {'R','N','B','Q','K','B','N','R'}};

void board_print(char map[8][8],int player)
{
    if (player)
    {
        for (int i=0;i<8;i++)
        {
            printf("%d  ",8-i);
            for (int j=0;j<8;j++)
            {
                if (map[i][j]=='-')
                {
                    if ((i+j)%2==0)
                        printf("· ");
                    else 
                        printf("- ");
                }
                else
                    printf("%c ",map[i][j]);
            }
            printf("\n");
        }
        printf("\n   a b c d e f g h\n");
    }

    else
    {
        for (int i=0;i<8;i++)
        {
            printf("%d  ",i+1);
            for (int j=0;j<8;j++)
            {
                if (map[7-i][7-j]=='-')
                {
                    if ((14-i-j)%2==0)
                        printf("· ");
                    else 
                        printf("- ");
                }
                else
                    printf("%c ",map[7-i][7-j]);
            }
            printf("\n");
        }
        printf("\n   h g f e d c b a\n");
    }
}

void cp_board(char sou[8][8], char des[8][8])
{
    for (int i=0;i<8;i++)
        for (int j=0;j<8;j++)
            des[i][j]=sou[i][j];
}

