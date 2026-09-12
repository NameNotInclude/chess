#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "piece.h"
#include "game.h"

#define IS_LETTER(x) ((x)>='a' && (x)<='h')
#define IS_NUM(x) (((x)>='1' && (x)<='8'))
#define VALID_POS(x,y) ((x)>=0 && (x)<8 && (y)>=0 && (y)<8)

#define INVA_MOVE 0
#define NOR_MOVE 1
#define ENPASS 2
#define PAWN_2 3
#define KING_MOVE 4
#define ROOK_MOVE 5
#define O_O_O 6
#define O_O 7
#define PAWN_MOVE 8
#define CAPTURE 9

State pState={0,0,0,0,0,0,-2,-2};

int mate=0,capture=0,c=0;
char prom=0;

void State_Print(void)
{
    printf("Game state:\n");
    printf("  WHITE_KING_MOVE   = %d  (white king has moved)\n", pState.WHITE_KING_MOVE);
    printf("  BLACK_KING_MOVE   = %d  (black king has moved)\n", pState.BLACK_KING_MOVE);
    printf("  WHITE_A_ROOK_MOVE = %d  (white rook from a1 has moved)\n", pState.WHITE_A_ROOK_MOVE);
    printf("  WHITE_H_ROOK_MOVE = %d  (white rook from h1 has moved)\n", pState.WHITE_H_ROOK_MOVE);
    printf("  BLACK_A_ROOK_MOVE = %d  (black rook from a8 has moved)\n", pState.BLACK_A_ROOK_MOVE);
    printf("  BLACK_H_ROOK_MOVE = %d  (black rook from h8 has moved)\n", pState.BLACK_H_ROOK_MOVE);

    printf("  WHITE_EN          = %d  (black may capture en passant on file %c; -2 means none)\n",
        pState.WHITE_EN,
        pState.WHITE_EN >= 0 && pState.WHITE_EN < 8 ? 'a' + pState.WHITE_EN : '-');
    printf("  BLACK_EN          = %d  (white may capture en passant on file %c; -2 means none)\n",
        pState.BLACK_EN,
        pState.BLACK_EN >= 0 && pState.BLACK_EN < 8 ? 'a' + pState.BLACK_EN : '-');
}

int valid_move(const char *move, char map[8][8], int player)
{
    if (move == NULL || strlen(move) < 4)
        return INVA_MOVE;

    int from_row = 7 - (move[1] - '1');
    int from_col = move[0] - 'a';
    int to_row = 7 - (move[3] - '1');
    int to_col = move[2] - 'a';

    if (from_row < 0 || from_row > 7 || from_col < 0 || from_col > 7 ||
        to_row   < 0 || to_row   > 7 || to_col   < 0 || to_col   > 7)
        return INVA_MOVE;

    char type = map[from_row][from_col];

    //move your own piece
    if (player == 1 && !(type >= 'A' && type <= 'Z'))
        return INVA_MOVE;

    if (player == 0 && !(type >= 'a' && type <= 'z'))
        return INVA_MOVE;

    if (type == '-')
        return INVA_MOVE;  

    int dx = abs(to_col - from_col);
    int dy = abs(to_row - from_row);

    if (dx == 0 && dy == 0) return INVA_MOVE;

    //black pawn to move
    if (type == 'p')
    {
        //backward
        if (to_row - from_row <= 0)
            return INVA_MOVE;

        //move 2 square
        if (from_row == 1 && dx == 0 && to_row - from_row == 2 && map[to_row][to_col] == '-' && map[from_row + 1][to_col] == '-')
            return PAWN_2;

        //move 1 square
        if (dx == 0 && to_row - from_row == 1 && map[to_row][to_col] == '-')
            return PAWN_MOVE;

        //take piece
        if (dx == 1 && dy == 1 && to_row - from_row == 1 && map[to_row][to_col] >= 'A' && map[to_row][to_col] <= 'Z')
            return PAWN_MOVE;

        //enpassant
        if (dx == 1 && dy == 1 && to_row - from_row == 1 && from_row == 4 && to_col == pState.WHITE_EN && map[to_row][to_col] == '-' && map[from_row][to_col] == 'P')
            return ENPASS;

        return INVA_MOVE;
    }

    //black rook to move
    if (type == 'r')
    {
        if (dx == 0 || dy == 0)
        {
            //move vertically
            if (dx == 0)
            {
                int step = (to_row < from_row) ? -1 : 1;
                for (int i = from_row + step; i != to_row; i += step)
                    if (map[i][from_col] != '-')
                        return INVA_MOVE;
            }

            //move horizontally
            else
            {
                int step = (to_col < from_col) ? -1 : 1;
                for (int i = from_col + step; i != to_col; i += step)
                    if (map[from_row][i] != '-')
                        return INVA_MOVE;
            }

            if (map[to_row][to_col] >= 'a' && map[to_row][to_col] <= 'z')
                return INVA_MOVE;
            return ROOK_MOVE;
        }

        return INVA_MOVE;
    }

    //black knight to move
    if (type == 'n')
    {
        if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
        {
            if (map[to_row][to_col] >= 'a' && map[to_row][to_col] <= 'z')
                return INVA_MOVE;
            return NOR_MOVE;
        }
        else
            return INVA_MOVE;
    }

    //black bishop to move
    if (type == 'b')
    {
        if (dx == dy)
        {
            int step_x = (to_col > from_col) ? 1 : -1;
            int step_y = (to_row > from_row) ? 1 : -1;
            int x = from_col + step_x;
            int y = from_row + step_y;

            while (x != to_col && y != to_row)
            {
                if (map[y][x] != '-')
                    return INVA_MOVE;
                x += step_x;
                y += step_y;
            }

            if (map[to_row][to_col] >= 'a' && map[to_row][to_col] <= 'z')
                return INVA_MOVE;
            return NOR_MOVE;
        }
        return INVA_MOVE;
    }

    //black queen to move
    if (type == 'q')
    {
        //move diagonally
        if (dx == dy)
        {
            int step_x = (to_col > from_col) ? 1 : -1;
            int step_y = (to_row > from_row) ? 1 : -1;
            int x = from_col + step_x;
            int y = from_row + step_y;

            while (x != to_col && y != to_row)
            {
                if (map[y][x] != '-')
                    return INVA_MOVE;
                x += step_x;
                y += step_y;
            }

            if (map[to_row][to_col] >= 'a' && map[to_row][to_col] <= 'z')
                return INVA_MOVE;
            return NOR_MOVE;
        }

        if (dx == 0 || dy == 0)
        {
            //move vertically
            if (dx == 0)
            {
                int step = (to_row < from_row) ? -1 : 1;
                for (int i = from_row + step; i != to_row; i += step)
                    if (map[i][from_col] != '-')
                        return INVA_MOVE;
            }

            //move horizontally
            else
            {
                int step = (to_col < from_col) ? -1 : 1;
                for (int i = from_col + step; i != to_col; i += step)
                    if (map[from_row][i] != '-')
                        return INVA_MOVE;
            }

            if (map[to_row][to_col] >= 'a' && map[to_row][to_col] <= 'z')
                return INVA_MOVE;
            return NOR_MOVE;
        }

        return INVA_MOVE;
    }

    //black king to move
    if (type == 'k')
    {
        if ((dy == 1 && dx == 0) || (dy == 0 && dx == 1) || (dx == 1 && dy == 1))
        {
            if (map[to_row][to_col] >= 'a' && map[to_row][to_col] <= 'z')
                return INVA_MOVE;

            return KING_MOVE;
        }
        else
            return INVA_MOVE;
    }

    //white pawn to move
    if (type == 'P')
    {
        //backward
        if (to_row - from_row >= 0)
            return INVA_MOVE;

        //move 2 square
        if (from_row == 6 && dx == 0 && to_row - from_row == -2 && map[to_row][to_col] == '-' && map[from_row - 1][to_col] == '-')
            return PAWN_2;

        //move 1 square
        if (dx == 0 && to_row - from_row == -1 && map[to_row][to_col] == '-')
            return PAWN_MOVE;

        //take piece
        if (dx == 1 && dy == 1 && to_row - from_row == -1 && map[to_row][to_col] >= 'a' && map[to_row][to_col] <= 'z')
            return PAWN_MOVE;

        //enpassant
        if (dx == 1 && dy == 1 && to_row - from_row == -1 && from_row == 3 &&
            to_col == pState.BLACK_EN && map[to_row][to_col] == '-' && map[from_row][to_col] == 'p')
            return ENPASS;

        return INVA_MOVE;
    }

    //white rook to move
    if (type == 'R')
    {
        if (dx == 0 || dy == 0)
        {
            //move vertically
            if (dx == 0)
            {
                int step = (to_row < from_row) ? -1 : 1;
                for (int i = from_row + step; i != to_row; i += step)
                    if (map[i][from_col] != '-')
                        return INVA_MOVE;
            }

            //move horizontally
            else
            {
                int step = (to_col < from_col) ? -1 : 1;
                for (int i = from_col + step; i != to_col; i += step)
                    if (map[from_row][i] != '-')
                        return INVA_MOVE;
            }

            if (map[to_row][to_col] >= 'A' && map[to_row][to_col] <= 'Z')
                return INVA_MOVE;

            return ROOK_MOVE;
        }

        return INVA_MOVE;
    }

    //white knight to move
    if (type == 'N')
    {
        if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
        {
            if (map[to_row][to_col] >= 'A' && map[to_row][to_col] <= 'Z')
                return INVA_MOVE;
            return NOR_MOVE;
        }
        else
            return INVA_MOVE;
    }

    //white bishop to move
    if (type == 'B')
    {
        if (dx == dy)
        {
            int step_x = (to_col > from_col) ? 1 : -1;
            int step_y = (to_row > from_row) ? 1 : -1;
            int x = from_col + step_x;
            int y = from_row + step_y;

            while (x != to_col && y != to_row)
            {
                if (map[y][x] != '-')
                    return INVA_MOVE;
                x += step_x;
                y += step_y;
            }

            if (map[to_row][to_col] >= 'A' && map[to_row][to_col] <= 'Z')
                return INVA_MOVE;
            return NOR_MOVE;
        }
        return INVA_MOVE;
    }

    //white queen to move
    if (type == 'Q')
    {
        //move diagonally
        if (dx == dy)
        {
            int step_x = (to_col > from_col) ? 1 : -1;
            int step_y = (to_row > from_row) ? 1 : -1;
            int x = from_col + step_x;
            int y = from_row + step_y;

            while (x != to_col && y != to_row)
            {
                if (map[y][x] != '-')
                    return INVA_MOVE;
                x += step_x;
                y += step_y;
            }

            if (map[to_row][to_col] >= 'A' && map[to_row][to_col] <= 'Z')
                return INVA_MOVE;
            return NOR_MOVE;
        }

        if (dx == 0 || dy == 0)
        {
            //move vertically
            if (dx == 0)
            {
                int step = (to_row < from_row) ? -1 : 1;
                for (int i = from_row + step; i != to_row; i += step)
                    if (map[i][from_col] != '-')
                        return INVA_MOVE;
            }

            //move horizontally
            else
            {
                int step = (to_col < from_col) ? -1 : 1;
                for (int i = from_col + step; i != to_col; i += step)
                    if (map[from_row][i] != '-')
                        return INVA_MOVE;
            }

            if (map[to_row][to_col] >= 'A' && map[to_row][to_col] <= 'Z')
                return INVA_MOVE;
            return NOR_MOVE;
        }

        return INVA_MOVE;
    }

    //white king to move
    if (type == 'K')
    {
        if ((dy == 1 && dx == 0) || (dx == 1 && dy == 0) || (dx == 1 && dy == 1))
        {
            if (map[to_row][to_col] >= 'A' && map[to_row][to_col] <= 'Z')
                return INVA_MOVE;

            return KING_MOVE;
        }
        else
            return INVA_MOVE;
    }
    
    return INVA_MOVE;
}
int move(const char *ctrl, char map[8][8], int player)
{

    int cap = player?0:32;
    int buttom = player?7:0;

    //long castle
    if (strcmp(ctrl,"O-O-O")==0 || strcmp(ctrl,"o-o-o")==0)
    {
        //already moved
        if (player)
        {
            if (pState.WHITE_A_ROOK_MOVE==1 || pState.WHITE_KING_MOVE==1)
                return 0;
        }
        else
        {
            if (pState.BLACK_A_ROOK_MOVE==1 || pState.BLACK_KING_MOVE==1)
                return 0;
        }


        if (map[buttom][4] != 'K'+cap || map[buttom][0] != 'R'+cap)
            return 0;

        //blocked
        if (map[buttom][1]!='-' || map[buttom][2]!='-' || map[buttom][3]!='-')
            return 0;

            
        if (check(map,player))
            return 0;

        for (int i=2;i<=3;i++)
        {
            map[buttom][i]='K'+cap,map[buttom][4]='-';
            
            if (check(map,player))
            {
                map[buttom][i]='-',map[buttom][4]='K'+cap;
                return 0;
            }
            map[buttom][i]='-',map[buttom][4]='K'+cap;
        }

        map[buttom][3]='R'+cap;
        map[buttom][2]='K'+cap;
        map[buttom][0]='-';
        map[buttom][4]='-';

        c=check(map,!player);
        mate=c && no_legal_move(map,!player);
        prom=0;
        capture=0;
        
        return O_O_O;
    }
 
    //short castle
    if (strcmp(ctrl,"O-O")==0 || strcmp(ctrl,"o-o")==0)
    {
        //already moved
        if (player)
        {
            if (pState.WHITE_H_ROOK_MOVE==1 || pState.WHITE_KING_MOVE==1)
                return 0;
        }
        else
        {
            if (pState.BLACK_H_ROOK_MOVE==1 || pState.BLACK_KING_MOVE==1)
                return 0;
        }

        if (map[buttom][4] != 'K'+cap || map[buttom][7] != 'R'+cap)
            return 0;

        //blocked
        if (map[buttom][5]!='-' || map[buttom][6]!='-')
            return 0;

        if (check(map,player))
            return 0;

        for (int i=5;i<=6;i++)
        {
            map[buttom][i]='K'+cap,map[buttom][4]='-';
            
            if (check(map,player))
            {
                map[buttom][i]='-',map[buttom][4]='K'+cap;
                return 0;
            }
            map[buttom][i]='-',map[buttom][4]='K'+cap;
        }

        map[buttom][5]='R'+cap;
        map[buttom][6]='K'+cap;
        map[buttom][4]='-';
        map[buttom][7]='-';

        c=check(map,!player);
        mate=c && no_legal_move(map,!player);
        prom=0;
        capture=0;

        return O_O;
    }

    //normal move
    if (IS_LETTER(ctrl[0]) && IS_LETTER(ctrl[2]) && IS_NUM(ctrl[1]) && IS_NUM(ctrl[3]) && ctrl[4]=='\0')
    {
        int from[2]={8-(ctrl[1]-'0'),ctrl[0]-'a'};
        int to[2]={8-(ctrl[3]-'0'),ctrl[2]-'a'};

        int piece=map[from[0]][from[1]];
        int dest=map[to[0]][to[1]];

        int result = valid_move(ctrl, map, player);

        if (result == 0)
            return 0;     

        char ep_captured;
        //吃过路兵
        if (result == ENPASS)
        {
            ep_captured = map[from[0]][to[1]];
            map[from[0]][to[1]] = '-';
        }

        int capture=0;
        if (map[to[0]][to[1]] != '-')
            capture=1;

        map[from[0]][from[1]] = '-';
        map[to[0]][to[1]] = piece;

        if (check(map, player))
        {
            //回滚送将操作
            map[from[0]][from[1]] = piece;
            map[to[0]][to[1]] = dest;
            if (result == ENPASS)
                map[from[0]][to[1]] = ep_captured;
            return INVA_MOVE;
        }
        
        char p=0;
        if (piece == 'P'+cap && to[0] == 7-buttom)
        {
            
            printf("Choose to promote:\n");
            getchar();
            scanf("%c",&p);

            while (p!='q' && p!='r' && p!='b' && p!='n'
                && p!='Q' && p!='R' && p!='B' && p!='N')
                scanf(" %c ",&p);
            
            if (p>='a' && p<='z') 
                map[to[0]][to[1]] = p -32 + cap;

            else 
                map[to[0]][to[1]] = p + cap;
        }

        c=check(map,!player);
        mate=c && no_legal_move(map,!player);
        prom=p;
        capture= dest=='-'?0:1;

        if (result == ENPASS)
            return ENPASS;

        if (result == PAWN_2)
            return -to[1];

        if (result == KING_MOVE)
            return KING_MOVE;

        if (result == ROOK_MOVE)
            return 100+from[0]*10+from[1];

        return 1;
    }
    
    //invalid input
    else
        return 0;
}
void State_Update(int comm ,int player)
{
    if (comm == KING_MOVE)
    {
        if (player)
        {
            pState.BLACK_EN=-2;
            pState.WHITE_KING_MOVE=1;
        }
        else
        {
            pState.WHITE_EN=-2;
            pState.BLACK_KING_MOVE=1;
        }
    }   
    if (comm < 0)
    {
        if (player)
        {
            pState.WHITE_EN=-comm;
            pState.BLACK_EN=-2;
        }
        else
        {
            pState.WHITE_EN=-2;
            pState.BLACK_EN=-comm;
        }
    }
    if (comm > 100)
    {
        if (player)
        {
            if (comm%10 == 7 && comm/10%10 == 7)
                pState.WHITE_H_ROOK_MOVE=1;
            
            else if (comm%10 == 0 && comm/10%10 == 7)
                pState.WHITE_A_ROOK_MOVE=1;

            pState.BLACK_EN=-2;
        }

        else
        {
            if (comm%10 == 7 && comm/10%10 == 0)
                pState.BLACK_H_ROOK_MOVE=1;
            
            else if (comm%10 == 0 && comm/10%10 == 0)
                pState.BLACK_A_ROOK_MOVE=1;

            pState.WHITE_EN=-2;
        }
    }
    if (comm == NOR_MOVE)
    {
        if (player)
            pState.BLACK_EN=-2;
        else
            pState.WHITE_EN=-2;
    }
    if (comm == O_O_O)
    {
        if (player)
        {
            pState.WHITE_KING_MOVE=1;
            pState.WHITE_A_ROOK_MOVE=1;
            pState.BLACK_EN=-2;
        }
        else
        {
            pState.BLACK_KING_MOVE=1;
            pState.BLACK_A_ROOK_MOVE=1;
            pState.WHITE_EN=-2;
        }
    }
    if (comm == O_O)
    {
        if (player)
        {
            pState.WHITE_KING_MOVE=1;
            pState.WHITE_H_ROOK_MOVE=1;
            pState.BLACK_EN=-2;
        }
        else
        {
            pState.BLACK_KING_MOVE=1;
            pState.BLACK_H_ROOK_MOVE=1;
            pState.WHITE_EN=-2;
        }
    }
}
char* transform(char map[8][8], const char* ctrl, int player, int capture, int check, int mate, char prom)
{
    if (strcmp(ctrl,"o-o-o")==0 || strcmp(ctrl,"O-O-O"))
    {
        if (mate==1)
        {
            char* result=(char*)malloc(sizeof(char)*7);
            strcpy(result,"O-O-O#");
            return result;
        }
        if (check==1)
        {
            char* result=(char*)malloc(sizeof(char)*7);
            strcpy(result,"O-O-O+");
            return result;
        }
        else
        {
            char* result=(char*)malloc(sizeof(char)*7);
            strcpy(result,"O-O-O");
            return result;
        }
    }
    if (strcmp(ctrl,"o-o")==0 || strcmp(ctrl,"O-O"))
    {
        if (mate==1)
        {
            char* result=(char*)malloc(sizeof(char)*5);
            strcpy(result,"O-O#");
            return result;
        }
        if (check==1)
        {
            char* result=(char*)malloc(sizeof(char)*5);
            strcpy(result,"O-O+");
            return result;
        }
        else
        {
            char* result=(char*)malloc(sizeof(char)*4);
            strcpy(result,"O-O");
            return result;
        }
    }

    int from[2]={8-(ctrl[1]-'0'),ctrl[0]-'a'};
    int to[2]={8-(ctrl[3]-'0'),ctrl[2]-'a'};

    int dir[8][2]={{1,1},{1,-1},{-1,1},{-1,-1},{1,0},{0,1},{-1,0},{0,-1}};
    int night[8][2]={{1,2},{1,-2},{-1,2},{-1,-2},{2,1},{2,-1},{-2,1},{-2,-1}};

    int cap=player?0:32;
    int buttom=player?0:7;

    char piece=map[from[0]][from[1]];
    if (piece=='P'+cap)
    {
        char* result=(char*)malloc(sizeof(char)*8);
        int top=0;

        if (capture==1)
        {
            result[top++]=ctrl[0];
            result[top++]='x';
        }
        result[top++]=ctrl[2];
        result[top++]=ctrl[3];

        if (to[0]==buttom)
        {
            result[top++]='=';
            result[top++]=prom;
        }

        if (mate==1)
            result[top++]='#';
        else if (check==1)
            result[top++]='+';

        result[top++]='\0';

        return result;
    }
    if (piece=='N'+cap)
    {
        char* result=(char*)malloc(sizeof(char)*8);
        int top=0;

        result[top++]='N';
        int col=1,row=1;
        for (int i=0;i<8;i++)
        {
            if (VALID_POS(from[0]-night[i][0],from[1]-night[i][1]) && map[from[0]-night[i][0]][from[1]-night[i][1]]=='N'+cap)
            {
                if (from[1]!=from[1]-night[i][1])
                    col=0;
                if (from[0]!=from[0]-night[i][0])
                    row=0;
            }
        }
        if (col == 0)
            result[top++]=ctrl[0];
        if (row == 0)
            result[top++]=ctrl[1];

        if (capture == 1)
            result[top++]='x';
        
        result[top++]=ctrl[2];
        result[top++]=ctrl[3];

        if (mate == 1)
            result[top++]='#';
        else if (check==1)
            result[top++]='+';

        result[top++]='\0';
        return result;
    }
    if (piece=='R'+cap)
    {
        char* result=(char*)malloc(sizeof(char)*8);
        int top=0;

        result[top++]='R';
        int col=1,row=1;
        for(int i=4;i<8;i++)
        {
            int x_i=from[0]+dir[i][0];
            int y_i=from[1]+dir[i][1];
            while (VALID_POS(x_i,y_i) && map[x_i][y_i]!='R'+cap);

            if (map[x_i][y_i]=='R'+cap)
            {
                if (x_i==from[0])
                    row=0;
                if (y_i==from[1])
                    col=0;
            }
        }
        if (col == 0)
            result[top++]=ctrl[0];
        if (row == 0)
            result[top++]=ctrl[1];

        if (capture == 1)
            result[top++]='x';
        
        result[top++]=ctrl[2];
        result[top++]=ctrl[3];    

        if (mate == 1)
            result[top++]='#';
        else if (check==1)
            result[top++]='+';

        result[top++]='\0';
        return result;
    }
    if (piece=='B'+cap)
    {
        char* result=(char*)malloc(sizeof(char)*8);
        int top=0;

        result[top++]='B';
        int col=1,row=1;
        for(int i=0;i<4;i++)
        {
            int x_i=from[0]+dir[i][0];
            int y_i=from[1]+dir[i][1];
            while (VALID_POS(x_i,y_i) && map[x_i][y_i]!='R'+cap);

            if (map[x_i][y_i]=='R'+cap)
            {
                if (x_i==from[0])
                    row=0;
                if (y_i==from[1])
                    col=0;
            }
        }
        if (col == 0)
            result[top++]=ctrl[0];
        if (row == 0)
            result[top++]=ctrl[1];

        if (capture == 1)
            result[top++]='x';
        
        result[top++]=ctrl[2];
        result[top++]=ctrl[3];    

        if (mate == 1)
            result[top++]='#';
        else if (check==1)
            result[top++]='+';

        result[top++]='\0';
        return result;
    }
    if (piece=='Q'+cap)
    {
        char* result=(char*)malloc(sizeof(char)*8);
        int top=0;

        result[top++]='Q';
        int col=1,row=1;
        for(int i=0;i<8;i++)
        {
            int x_i=from[0]+dir[i][0];
            int y_i=from[1]+dir[i][1];
            while (VALID_POS(x_i,y_i) && map[x_i][y_i]!='R'+cap);

            if (map[x_i][y_i]=='R'+cap)
            {
                if (x_i==from[0])
                    row=0;
                if (y_i==from[1])
                    col=0;
            }
        }
        if (col == 0)
            result[top++]=ctrl[0];
        if (row == 0)
            result[top++]=ctrl[1];

        if (capture == 1)
            result[top++]='x';
        
        result[top++]=ctrl[2];
        result[top++]=ctrl[3];    

        if (mate == 1)
            result[top++]='#';
        else if (check==1)
            result[top++]='+';

        result[top++]='\0';
        return result;
    }
    if (piece=='K'+cap)
    {
        char* result=(char*)malloc(sizeof(char)*6);
        int top=0;

        result[top++]='K';

        if (capture == 1)
            result[top++]='x';
        
        result[top++]=ctrl[2];
        result[top++]=ctrl[3];    

        if (mate == 1)
            result[top++]='#';
        else if (check==1)
            result[top++]='+';

        result[top++]='\0';

        return result;
    }

    /* 兜底:未识别的棋子类型,返回原始着法串(避免函数末尾无返回值) */
    static char result[8];
    snprintf(result, sizeof result, "%s", ctrl);
    return result;
}