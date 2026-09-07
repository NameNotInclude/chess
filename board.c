#include <stdio.h>
#include "board.h"
#include "piece.h"

#define VALID_POS(x,y) ((x)>=0 && (x)<8 && (y)>=0 && (y)<8)
// 1 for white to play, 0 for black.
int player = 1;

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

void board_print(char map[8][8],int player)
{
    if (player)
    {
        for (int i=0;i<8;i++)
        {
            printf("%d  ",8-i);
            for (int j=0;j<8;j++)
                printf("%c ",map[i][j]);
            printf("\n\n");
        }
        printf("   a b c d e f g h\n");
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

int legal_move(char map[8][8], int player)
{
    int dir[8][2]={{1,1},{1,-1},{-1,1},{-1,-1},{1,0},{0,1},{-1,0},{0,-1}};
    int night[8][2]={{1,2},{1,-2},{-1,2},{-1,-2},{2,1},{2,-1},{-2,1},{-2,-1}};
    int forward=player?-1:1;
    int buttom=player?7:0;

    int cap=player?0:32;

    for (int i=0;i<8;i++)
    {
        for (int j=0;j<8;j++)
        {
            char piece=map[i][j];

            if (piece == 'N'+cap)
                for (int k=0;k<8;k++)
                {
                    if (!VALID_POS(i+night[k][1],j+night[k][0]))
                        continue;

                    int des=map[i+night[k][1]][j+night[k][0]];

                    char str[5]={'a'+j,'8'-i,'a'+j+night[k][0],'8'-i-night[k][1],'\0'};

                    if (move(str,map))
                    {
                        map[i][j]=piece;
                        map[i+night[k][1]][j+night[k][0]]=des;  

                        return 0;
                    }
                }

            if (piece == 'R'+cap)
                for (int k=4;k<8;k++)
                {
                    for (int l=1;l<8 && VALID_POS(i+l*dir[k][1],j+l*dir[k][0]);l++)
                    {
                        int ROOK1,ROOK2;
                        if (player)
                        {
                            ROOK1=WHITE_A_ROOK_MOVE;
                            ROOK2=WHITE_H_ROOK_MOVE;
                        }
                        else
                        {
                            ROOK1=BLACK_A_ROOK_MOVE;
                            ROOK2=BLACK_H_ROOK_MOVE;
                        }

                        int des=map[i+l*dir[k][1]][j+l*dir[k][0]];

                        char str[5]={'a'+j,'8'-i,'a'+j+l*dir[k][0],'8'-i-l*dir[k][1],'\0'};

                        if (move(str,map))
                        {
                            map[i][j]=piece;
                            map[i+l*dir[k][1]][j+l*dir[k][0]]=des;

                            if (player)
                            {
                                WHITE_A_ROOK_MOVE=ROOK1;
                                WHITE_H_ROOK_MOVE=ROOK2;
                            }
                            else
                            {
                                BLACK_A_ROOK_MOVE=ROOK1;
                                BLACK_H_ROOK_MOVE=ROOK2;
                            }
                            
                            return 0;
                        }
                    }      
                }

            if (piece == 'B'+cap)
                for (int k=0;k<4;k++)
                {
                    for (int l=1;l<8 && VALID_POS(i+l*dir[k][1],j+l*dir[k][0]);l++)
                    {
                        int des=map[i+l*dir[k][1]][j+l*dir[k][0]];

                        char str[5]={'a'+j,'8'-i,'a'+j+l*dir[k][0],'8'-i-l*dir[k][1],'\0'};

                        if (move(str,map))
                        {
                            map[i][j]=piece;
                            map[i+l*dir[k][1]][j+l*dir[k][0]]=des;
                            return 0;
                        }
                    }      
                }

            if (piece == 'Q'+cap)
                for (int k=0;k<8;k++)
                {
                    for (int l=1;l<8 && VALID_POS(i+l*dir[k][1],j+l*dir[k][0]);l++)
                    {
                        int des=map[i+l*dir[k][1]][j+l*dir[k][0]];

                        char str[5]={'a'+j,'8'-i,'a'+j+l*dir[k][0],'8'-i-l*dir[k][1],'\0'};

                        if (move(str,map))
                        {
                            map[i][j]=piece;
                            map[i+l*dir[k][1]][j+l*dir[k][0]]=des;
                            return 0;
                        }
                    }      
                }
        
            if (piece == 'K'+cap)
            {
                int KING;
                if (player)
                    KING=WHITE_KING_MOVE;
                else
                    KING=BLACK_KING_MOVE;

                for (int k=0;k<8;k++)
                {
                    if (!VALID_POS(i+dir[k][1],j+dir[k][0]))
                        continue;

                    int des=map[i+dir[k][1]][j+dir[k][0]];

                    char str[5]={'a'+j,'8'-i,'a'+j+dir[k][0],'8'-i-dir[k][1],'\0'};

                    if (move(str,map))
                    {
                        map[i][j]=piece;
                        map[i+dir[k][1]][j+dir[k][0]]=des;

                        if (player)
                            WHITE_KING_MOVE=KING;
                        else
                            BLACK_KING_MOVE=KING;

                        return 0;
                    }
                }

                if (move("o-o-o",map))
                {
                    map[buttom][2]='-';
                    map[buttom][3]='-';
                    map[buttom][0]='R'+cap;
                    map[buttom][4]='K'+cap;

                    if (player)
                    {
                        WHITE_KING_MOVE=0;
                        WHITE_A_ROOK_MOVE=0;
                    }
                    else
                    {
                        BLACK_A_ROOK_MOVE=0;
                        BLACK_KING_MOVE=0;
                    }
                }

                if (move("o-o",map))
                {
                    map[buttom][5]='-';
                    map[buttom][6]='-';
                    map[buttom][7]='R'+cap;
                    map[buttom][4]='K'+cap;

                    if (player)
                    {
                        WHITE_KING_MOVE=0;
                        WHITE_H_ROOK_MOVE=0;
                    }
                    else
                    {
                        BLACK_H_ROOK_MOVE=0;
                        BLACK_KING_MOVE=0;
                    }
                }
            }       
            

            if (piece == 'P'+cap)
            {
                for (int k=-1;k<=1;k++)
                {
                    if (!VALID_POS(i+forward,j+k))
                        continue;

                    int EN_STATE;
                    if (player)
                        EN_STATE=BLACK_EN;
                    else
                        EN_STATE=WHITE_EN;

                    int des=map[i+forward][j+k];
                    //记录可能的过路兵
                    int try=map[i][j+k];

                    char str[5]={'a'+j,'8'-i,'a'+j+k,'8'-i-forward,'\0'};

                    if (move(str,map))
                    {
                        map[i][j]=piece;
                        map[i+forward][j+k]=des;
                        map[i][j+k]=try;

                        if (player)
                            BLACK_EN=EN_STATE;
                        else
                            WHITE_EN=EN_STATE;

                        return 0;
                    }
                }

                //2 square
                if (VALID_POS(i+forward*2,j))
                {
                    int des=map[i+2*forward][j];

                    char str[5]={'a'+j,'8'-i,'a'+j,'8'-i-2*forward,'\0'};

                    if (move(str,map))
                    {
                        map[i][j]=piece;
                        map[i+forward*2][j]=des;

                        return 0;
                    }
                }


            }
        }
    }

    return 1;
}
