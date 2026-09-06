#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

#define IS_LETTER(x) ((x)>='a' && (x)<='z')
#define IS_NUM(x) (((x)>='1' && (x)<='8'))

typedef struct n
{
    char* move;
    char piece;
    struct n* next;
    struct n* varr;
    int result;//1 for white win, -1 for black win, 0 for draw, 114514 for unsettle
}node;


int WHITE_KING_MOVE=0;
int BLACK_KING_MOVE=0;

int WHITE_A_ROOK_MOVE=0;
int WHITE_H_ROOK_MOVE=0;
int BLACK_A_ROOK_MOVE=0;
int BLACK_H_ROOK_MOVE=0;

int WHITE_EN=-2;
int BLACK_EN=-2;


int valid_move(char* move, char map[8][8])
{
    if (move == NULL || strlen(move) < 4)
        return 0;

    int from_row = 7 - (move[1] - '1');
    int from_col = move[0] - 'a';
    int to_row = 7 - (move[3] - '1');
    int to_col = move[2] - 'a';

    if (from_row < 0 || from_row > 7 || from_col < 0 || from_col > 7 ||
        to_row   < 0 || to_row   > 7 || to_col   < 0 || to_col   > 7)
        return 0;

    char type = map[from_row][from_col];

    //move your own piece
    if (curr == 1 && !(type >= 'A' && type <= 'Z'))
        return 0;

    if (curr == 0 && !(type >= 'a' && type <= 'z'))
        return 0;

    if (type == '-')
        return 0;

    int dx = abs(to_col - from_col);
    int dy = abs(to_row - from_row);

    if (dx == 0 && dy == 0) return 0;

    //black pawn to move
    if (type == 'p')
    {
        //backward
        if (to_row - from_row <= 0)
            return 0;

        //move 2 square
        if (from_row == 1 && dx == 0 && to_row - from_row == 2 && map[to_row][to_col] == '-' && map[from_row + 1][to_col] == '-')
            return 1;

        //move 1 square
        if (dx == 0 && to_row - from_row == 1 && map[to_row][to_col] == '-')
            return 1;

        //take piece
        if (dx == 1 && dy == 1 && to_row - from_row == 1 && map[to_row][to_col] >= 'A' && map[to_row][to_col] <= 'Z')
            return 1;

        //enpassant
        if (dx == 1 && dy == 1 && to_row - from_row == 1 && from_row == 4 &&
            to_col == WHITE_EN && map[to_row][to_col] == '-' && map[from_row][to_col] == 'P')
            return 2;

        return 0;
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
                        return 0;
            }

            //move horizontally
            else
            {
                int step = (to_col < from_col) ? -1 : 1;
                for (int i = from_col + step; i != to_col; i += step)
                    if (map[from_row][i] != '-')
                        return 0;
            }

            if (map[to_row][to_col] >= 'a' && map[to_row][to_col] <= 'z')
                return 0;
            return 1;
        }

        return 0;
    }

    //black knight to move
    if (type == 'n')
    {
        if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
        {
            if (map[to_row][to_col] >= 'a' && map[to_row][to_col] <= 'z')
                return 0;
            return 1;
        }
        else
            return 0;
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
                    return 0;
                x += step_x;
                y += step_y;
            }

            if (map[to_row][to_col] >= 'a' && map[to_row][to_col] <= 'z')
                return 0;
            return 1;
        }
        return 0;
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
                    return 0;
                x += step_x;
                y += step_y;
            }

            if (map[to_row][to_col] >= 'a' && map[to_row][to_col] <= 'z')
                return 0;
            return 1;
        }

        if (dx == 0 || dy == 0)
        {
            //move vertically
            if (dx == 0)
            {
                int step = (to_row < from_row) ? -1 : 1;
                for (int i = from_row + step; i != to_row; i += step)
                    if (map[i][from_col] != '-')
                        return 0;
            }

            //move horizontally
            else
            {
                int step = (to_col < from_col) ? -1 : 1;
                for (int i = from_col + step; i != to_col; i += step)
                    if (map[from_row][i] != '-')
                        return 0;
            }

            if (map[to_row][to_col] >= 'a' && map[to_row][to_col] <= 'z')
                return 0;
            return 1;
        }

        return 0;
    }

    //black king to move
    if (type == 'k')
    {
        if ((dy == 1 && dx == 0) || (dy == 0 && dx == 1) || (dx == 1 && dy == 1))
        {
            if (map[to_row][to_col] >= 'a' && map[to_row][to_col] <= 'z')
                return 0;

            return 1;
        }
        else
            return 0;
    }

    //white pawn to move
    if (type == 'P')
    {
        //backward
        if (to_row - from_row >= 0)
            return 0;

        //move 2 square
        if (from_row == 6 && dx == 0 && to_row - from_row == -2 && map[to_row][to_col] == '-' && map[from_row - 1][to_col] == '-')
            return 1;

        //move 1 square
        if (dx == 0 && to_row - from_row == -1 && map[to_row][to_col] == '-')
            return 1;

        //take piece
        if (dx == 1 && dy == 1 && to_row - from_row == -1 && map[to_row][to_col] >= 'a' && map[to_row][to_col] <= 'z')
            return 1;

        //enpassant
        if (dx == 1 && dy == 1 && to_row - from_row == -1 && from_row == 3 &&
            to_col == BLACK_EN && map[to_row][to_col] == '-' && map[from_row][to_col] == 'p')
            return 2;

        return 0;
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
                        return 0;
            }

            //move horizontally
            else
            {
                int step = (to_col < from_col) ? -1 : 1;
                for (int i = from_col + step; i != to_col; i += step)
                    if (map[from_row][i] != '-')
                        return 0;
            }

            if (map[to_row][to_col] >= 'A' && map[to_row][to_col] <= 'Z')
                return 0;

            return 1;
        }

        return 0;
    }

    //white knight to move
    if (type == 'N')
    {
        if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
        {
            if (map[to_row][to_col] >= 'A' && map[to_row][to_col] <= 'Z')
                return 0;
            return 1;
        }
        else
            return 0;
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
                    return 0;
                x += step_x;
                y += step_y;
            }

            if (map[to_row][to_col] >= 'A' && map[to_row][to_col] <= 'Z')
                return 0;
            return 1;
        }
        return 0;
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
                    return 0;
                x += step_x;
                y += step_y;
            }

            if (map[to_row][to_col] >= 'A' && map[to_row][to_col] <= 'Z')
                return 0;
            return 1;
        }

        if (dx == 0 || dy == 0)
        {
            //move vertically
            if (dx == 0)
            {
                int step = (to_row < from_row) ? -1 : 1;
                for (int i = from_row + step; i != to_row; i += step)
                    if (map[i][from_col] != '-')
                        return 0;
            }

            //move horizontally
            else
            {
                int step = (to_col < from_col) ? -1 : 1;
                for (int i = from_col + step; i != to_col; i += step)
                    if (map[from_row][i] != '-')
                        return 0;
            }

            if (map[to_row][to_col] >= 'A' && map[to_row][to_col] <= 'Z')
                return 0;
            return 1;
        }

        return 0;
    }

    //white king to move
    if (type == 'K')
    {
        if ((dy == 1 && dx == 0) || (dx == 1 && dy == 0) || (dx == 1 && dy == 1))
        {
            if (map[to_row][to_col] >= 'A' && map[to_row][to_col] <= 'Z')
                return 0;
            return 1;
        }
        else
            return 0;
    }
    
    return 0;
}
int move(char ctrl[6], char map[8][8])
{
    int cap = curr?0:32;
    int buttom = curr?7:0;

    //long castle
    if (strcmp(ctrl,"O-O-O")==0 || strcmp(ctrl,"o-o-o")==0)
    {
        //already moved
        if (curr)
        {
            if (WHITE_A_ROOK_MOVE==1 || WHITE_KING_MOVE==1)
                return 0;
        }
        else
        {
            if (BLACK_A_ROOK_MOVE==1 || BLACK_KING_MOVE==1)
                return 0;
        }


        if (map[buttom][4] != 'K'+cap || map[buttom][0] != 'R'+cap)
            return 0;

        //blocked
        if (map[buttom][1]!='-' || map[buttom][2]!='-' || map[buttom][3]!='-')
            return 0;

            
        if (check(map,curr))
            return 0;

        for (int i=2;i<=3;i++)
        {
            map[buttom][i]='K'+cap,map[buttom][4]='-';
            
            if (check(map,curr))
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

        if (curr)
        {
            WHITE_A_ROOK_MOVE=1;
            WHITE_KING_MOVE=1;
        }
        else
        {
            BLACK_A_ROOK_MOVE=1;
            BLACK_KING_MOVE=1;
        }

        WHITE_EN = -2;
        BLACK_EN = -2;
        
        return 1;
    }
 
    //short castle
    if (strcmp(ctrl,"O-O")==0 || strcmp(ctrl,"o-o")==0)
    {
        //already moved
        if (curr)
        {
            if (WHITE_H_ROOK_MOVE==1 || WHITE_KING_MOVE==1)
                return 0;
        }
        else
        {
            if (BLACK_H_ROOK_MOVE==1 || BLACK_KING_MOVE==1)
                return 0;
        }

        if (map[buttom][4] != 'K'+cap || map[buttom][7] != 'R'+cap)
            return 0;

        //blocked
        if (map[buttom][5]!='-' || map[buttom][6]!='-')
            return 0;

        if (check(map,curr))
            return 0;

        for (int i=5;i<=6;i++)
        {
            map[buttom][i]='K'+cap,map[buttom][4]='-';
            
            if (check(map,curr))
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

        if (curr)
        {
            WHITE_H_ROOK_MOVE=1;
            WHITE_KING_MOVE=1;
        }
        else
        {
            BLACK_H_ROOK_MOVE=1;
            BLACK_KING_MOVE=1;
        }

        WHITE_EN = -2;
        BLACK_EN = -2;

        return 1;
    }

    //normal move
    if (IS_LETTER(ctrl[0]) && IS_LETTER(ctrl[2]) && IS_NUM(ctrl[1]) && IS_NUM(ctrl[3]) && ctrl[4]=='\0')
    {
        int from[2]={8-(ctrl[1]-'0'),ctrl[0]-'a'};
        int to[2]={8-(ctrl[3]-'0'),ctrl[2]-'a'};

        int piece=map[from[0]][from[1]];
        int dest=map[to[0]][to[1]];

        int result = valid_move(ctrl, map);

        if (result == 0)
            return 0;     

        int ep_captured = 0;
        if (result == 2)
        {
            ep_captured = map[from[0]][to[1]];
            map[from[0]][to[1]] = '-';
        }

        map[from[0]][from[1]] = '-';
        map[to[0]][to[1]] = piece;

        if (check(map, curr))
        {
            map[from[0]][from[1]] = piece;
            map[to[0]][to[1]] = dest;
            if (result == 2)
                map[from[0]][to[1]] = ep_captured;
            return 0;
        }

        if (piece == 'P' && to[0] == 0)
        {
            char p;
            while(1)
            {
                scanf(" %c ",&p);
                if (p!='Q' && p!='R' && p!='B' && p!='N')
                    continue;

                break;
            }
            map[to[0]][to[1]] = p;
        }
        else if (piece == 'p' && to[0] == 7)
        {
            char p;
            while(1)
            {
                scanf(" %c ",&p);
                if (p!='q' && p!='r' && p!='b' && p!='n')
                    continue;

                break;
            }
            map[to[0]][to[1]] = p;
        }


        WHITE_EN = -2;
        BLACK_EN = -2;


        if (piece == 'K' + cap)
        {
            if (curr) WHITE_KING_MOVE = 1;
            else      BLACK_KING_MOVE = 1;
        }

        if (piece == 'R' + cap)
        {
            if (curr)
            {
                if (ctrl[0] == 'a' && ctrl[1] == '1')
                    WHITE_A_ROOK_MOVE = 1;
                else if (ctrl[0] == 'h' && ctrl[1] == '1')
                    WHITE_H_ROOK_MOVE = 1;
            }
            else
            {
                if (ctrl[0] == 'a' && ctrl[1] == '8')
                    BLACK_A_ROOK_MOVE = 1;
                else if (ctrl[0] == 'h' && ctrl[1] == '8')
                    BLACK_H_ROOK_MOVE = 1;
            }
        }

        if (piece == 'P' + cap)
        {
            if (ctrl[1] == '2' && ctrl[3] == '4')
                WHITE_EN = from[1];
            else if (ctrl[1] == '7' && ctrl[3] == '5')
                BLACK_EN = from[1];
        }

        return 1;
    }
    
    //invalid input
    else
        return 0;
}
