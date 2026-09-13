#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "piece.h"
#include "game.h"

#define VALID_POS(x,y) ((x)>=0 && (x)<8 && (y)>=0 && (y)<8)

int c,mate,capture;
char prom;
//check wether 'player' is in check, 1 for white, 0 for black
int check(char map[8][8], int player)
{   
    int dir[8][2]={{1,1},{1,-1},{-1,1},{-1,-1},{1},{0,1},{-1},{0,-1}};
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

int no_legal_move(char map[8][8], int player, State pState)
{
    int dir[8][2]={{1,1},{1,-1},{-1,1},{-1,-1},{1},{0,1},{-1},{0,-1}};
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

                    if (move(str,map,player,0,pState,&c,&mate,&capture,&prom))
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
                        int des=map[i+l*dir[k][1]][j+l*dir[k][0]];

                        char str[5]={'a'+j,'8'-i,'a'+j+l*dir[k][0],'8'-i-l*dir[k][1],'\0'};

                        if (move(str,map,player,0,pState,&c,&mate,&capture,&prom))
                        {
                            map[i][j]=piece;
                            map[i+l*dir[k][1]][j+l*dir[k][0]]=des;
                            
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

                        if (move(str,map,player,0,pState,&c,&mate,&capture,&prom))
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

                        if (move(str,map,player,0,pState,&c,&mate,&capture,&prom))
                        {
                            map[i][j]=piece;
                            map[i+l*dir[k][1]][j+l*dir[k][0]]=des;
                            return 0;
                        }
                    }      
                }
        
            if (piece == 'K'+cap)
            {
                for (int k=0;k<8;k++)
                {
                    if (!VALID_POS(i+dir[k][1],j+dir[k][0]))
                        continue;

                    int des=map[i+dir[k][1]][j+dir[k][0]];

                    char str[5]={'a'+j,'8'-i,'a'+j+dir[k][0],'8'-i-dir[k][1],'\0'};

                    if (move(str,map,player,0,pState,&c,&mate,&capture,&prom))
                    {
                        map[i][j]=piece;
                        map[i+dir[k][1]][j+dir[k][0]]=des;

                        return 0;
                    }
                }

                if (move("o-o-o",map,player,0,pState,&c,&mate,&capture,&prom))
                {
                    map[buttom][2]='-';
                    map[buttom][3]='-';
                    map[buttom][0]='R'+cap;
                    map[buttom][4]='K'+cap;

                    return 0;
                }

                if (move("o-o",map,player,0,pState,&c,&mate,&capture,&prom))
                {
                    map[buttom][5]='-';
                    map[buttom][6]='-';
                    map[buttom][7]='R'+cap;
                    map[buttom][4]='K'+cap;

                    return 0;
                }
            }       
            

            if (piece == 'P'+cap)
            {
                for (int k=-1;k<=1;k++)
                {
                    if (!VALID_POS(i+forward,j+k))
                        continue;

                    int des=map[i+forward][j+k];
                    //记录可能的过路兵
                    int try=map[i][j+k];

                    char str[5]={'a'+j,'8'-i,'a'+j+k,'8'-i-forward,'\0'};

                    if (move(str,map,player,0,pState,&c,&mate,&capture,&prom))
                    {
                        map[i][j]=piece;
                        map[i+forward][j+k]=des;
                        map[i][j+k]=try;

                        return 0;
                    }
                }

                //2 square
                if (VALID_POS(i+forward*2,j))
                {
                    int des=map[i+2*forward][j];

                    char str[5]={'a'+j,'8'-i,'a'+j,'8'-i-2*forward,'\0'};

                    if (move(str,map,player,0,pState,&c,&mate,&capture,&prom))
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

Record* init(void)
{
    MPtr head = (MPtr)calloc(1, sizeof(Move));
    if (head == NULL)
        return NULL;

    head->detail_move=NULL;
    head->move=NULL;
    head->prev=NULL;
    head->next=NULL;
    head->next_varr=NULL;
    head->prev_varr=NULL;

    Record* result=(Record*)malloc(sizeof(Record));
    if (result == NULL)
    {
        free(head);
        return NULL;
    }
    result->head=head;
    result->last=head;

    return result;
}

void add(Record* M,char* ctrl,char* stan)
{
    if (M == NULL)
        return;

    MPtr newnode = (MPtr)calloc(1, sizeof(Move));
    if (newnode == NULL)
        return;

    newnode->move=stan;
    newnode->detail_move=ctrl;

    newnode->prev=M->last;
    newnode->next=NULL;
    newnode->next_varr=NULL;
    newnode->prev_varr=NULL;

    M->last->next=newnode;
    M->last=newnode;   /* 关键:尾指针必须前移,否则下一次 add 会覆盖同一个 next 并丢失旧节点 */
}

void free_record(Record* M)
{
    if (M == NULL)
        return;

    MPtr p = M->head;
    while (p != NULL)
    {
        MPtr nextnode = p->next;

        free(p->detail_move);
        free(p->move);
        free(p);

        p = nextnode;
    }

    free(M);
}

