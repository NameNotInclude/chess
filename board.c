#include <stdio.h>
#include "board.h"

#define VALID_POS(x,y) ((x)>=0 && (x)<8 && (y)>=0 && (y)<8)
// 1 for white to play, 0 for black.
int curr = 1;

//                    a   b   c   d   e   f   g   h 
char board[8][8] = {{'r','n','b','q','k','b','n','r'},//8
                    {'p','p','p','p','p','p','p','p'},//7
                    {'-','-','-','-','-','-','-','-'},//6
                    {'-','-','-','-','-','-','-','-'},//5
                    {'-','-','-','-','-','-','-','-'},//4
                    {'-','-','-','-','-','-','-','-'},//3
                    {'P','P','P','P','P','P','P','P'},//2
                    {'R','N','B','Q','K','B','N','R'}};//1

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

//check wether 'player' is in check, 1 for white, 0 for black
int check(char map[8][8], int player)
{   
    int dir[8][2]={{1,1},{1,-1},{-1,1},{-1,-1},{1,0},{0,1},{-1,0},{0,-1}};
    int night[8][2]={{1,2},{1,-2},{-1,2},{-1,-2},{2,1},{2,-1},{-2,1},{-2,-1}};

    char myking = player ? 'K' : 'k';   
    int  att = player ? 0 : 32; 

    int  d = player ? -1 : 1; 

    //先找到本方王的位置
    int kx=-1, ky=-1;
    for (int x=0;x<8 && kx<0;x++)
        for (int y=0;y<8;y++)
            if (map[x][y]==myking)
            {
                kx=x; ky=y;
                break;
            }

    if (kx < 0)
        return 0;

    for (int i=0;i<8;i++)
        if (VALID_POS(kx+night[i][0],ky+night[i][1]) &&
            map[kx+night[i][0]][ky+night[i][1]]=='n'-att)
            return 1;

    if ((VALID_POS(kx+d,ky-1) && map[kx+d][ky-1]=='p'-att) ||
        (VALID_POS(kx+d,ky+1) && map[kx+d][ky+1]=='p'-att))
        return 1;

    for (int i=4;i<8;i++)
    {
        int len=1;
        while (1)
        {
            int nx = kx + len*dir[i][0];
            int ny = ky + len*dir[i][1];
            if (!VALID_POS(nx,ny) || map[nx][ny] != '-')
                break; 
            len++;
        }
        int nx = kx + len*dir[i][0];
        int ny = ky + len*dir[i][1];
        if (VALID_POS(nx,ny))
        {
            char c = map[nx][ny];
            if (c=='r'-att || c=='q'-att)
                return 1;
        }
    }

    for (int i=0;i<4;i++)
    {
        int len=1;
        while (1)
        {
            int nx = kx + len*dir[i][0];
            int ny = ky + len*dir[i][1];
            if (!VALID_POS(nx,ny) || map[nx][ny] != '-')
                break;
            len++;
        }
        int nx = kx + len*dir[i][0];
        int ny = ky + len*dir[i][1];
        if (VALID_POS(nx,ny))
        {
            char c = map[nx][ny];
            if (c=='b'-att || c=='q'-att)
                return 1;
        }
    }

    return 0;
}

