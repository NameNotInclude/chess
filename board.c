#include <stdio.h>
#include "board.h"

#define VALID_POS(x,y) ((x)>=0 && (x)<8 && (y)>=0 && (y)<8)
// 1 for white to play, 0 for black.
int curr = 1;

char board[8][8] = {{'r','n','b','q','k','b','n','r'},
                    {'p','p','p','p','p','p','p','p'},
                    {'-','-','-','-','-','-','-','-'},
                    {'-','-','-','-','-','-','-','-'},
                    {'-','-','-','-','-','-','-','-'},
                    {'-','-','-','-','-','-','-','-'},
                    {'P','P','P','P','P','P','P','P'},
                    {'R','N','B','Q','K','B','N','R'}};

char temp_board[8][8] = {{'r','n','b','q','k','b','n','r'},
                         {'p','p','p','p','p','p','p','p'},
                         {'-','-','-','-','-','-','-','-'},
                         {'-','-','-','-','-','-','-','-'},
                         {'-','-','-','-','-','-','-','-'},
                         {'-','-','-','-','-','-','-','-'},
                         {'P','P','P','P','P','P','P','P'},
                         {'R','N','B','Q','K','B','N','R'}};

void board_print(char map[8][8])
{
    if (curr)
    {
        for (int i=0;i<8;i++)
        {
            printf("%d  ",8-i);
            for (int j=0;j<8;j++)
                printf("%c ",map[i][j]);
            printf("\n\n");
        }
        printf("   a b c d e f g h\n");

        curr=!curr;
    }

    else
    {
        for (int i=0;i<8;i++)
        {
            printf("%d  ",i+1);
            for (int j=0;j<8;j++)
                printf("%c ",map[7-i][7-j]);
            printf("\n\n");
        }
        printf("   h g f e d c b a\n");
        
        curr=!curr;
    }
}

void cp_board(char sou[8][8], char des[8][8])
{
    for (int i=0;i<8;i++)
        for (int j=0;j<8;j++)
            des[i][j]=sou[i][j];
}

//check wether 'palyer' is in check, 1 for white, 0 for black
int check(char map[8][8], int player)
{   
    int dir[8][2]={{1,1},{1,-1},{-1,1},{-1,-1},{1,0},{0,1},{-1,0},{0,-1}};
    int night[8][2]={{1,2},{1,-2},{-1,2},{-1,-2},{2,1},{2,-1},{-2,1},{-2,-1}};

    int cap = player?0:32;
    int d = player?-1:1;

    int x,y;
    for (x=0;x<8;x++)
        for (y=0;y<8;y++)
            if (map[x][y]=='k'+cap-32)
                break;

    //knight
    for (int i=0;i<8;i++)
        if (VALID_POS(x+night[i][0],y+night[i][1]) && map[x+night[i][0],y+night[i][1]]=='N'+cap)
            return 1;

    //pawn
    if ((VALID_POS(x+d,y-1) && map[x+d][y-1]=='P'+cap) || (VALID_POS(x+d,y+1) && map[x+d][y+1]=='P'+cap))
        return 1;

    //rook & queen
    for (int i=0;i<4;i++)
    {
        int len=1;
        while (VALID_POS(x+len*dir[i][0],y+len*dir[i][1]) && map[x+len*dir[i][0]][y+len*dir[i][1]] == '-')
            len++;
        
        if (map[x+len*dir[i][0]][y+len*dir[i][1]] == 'R'+cap || map[x+len*dir[i][0]][y+len*dir[i][1]] == 'Q'+cap)
            return 1;
    }

    //bishop & queen
    for (int i=4;i<8;i++)
    {
        int len=1;
        while (VALID_POS(x+len*dir[i][0],y+len*dir[i][1]) && map[x+len*dir[i][0]][y+len*dir[i][1]] == '-')
            len++;
        
        if (map[x+len*dir[i][0]][y+len*dir[i][1]] == 'B'+cap || map[x+len*dir[i][0]][y+len*dir[i][1]] == 'Q'+cap)
            return 1;
    }

    return 0;

}

