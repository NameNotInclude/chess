#ifndef GAME_H
#define GAME_H

typedef struct node
{
    int turn;

    char* detail_move;
    char* move;
    
    struct node* next;
    struct node* prev;

    struct node* next_varr;
    struct node* prev_varr;
}Move;

typedef Move* MPtr;

typedef struct
{
    MPtr head;
    MPtr last;
}Record;

/*
 * 判断 player 的王是否正被对方将军。
 *   map:    棋盘;player: 1 = 白方,0 = 黑方。
 *   返回:   1 = 被将军,0 = 未被将军。
 */
int check(char map[8][8], int player);

/*
* 判断 player是不是无合法步骤
 *   map:    棋盘;player: 1 = 白方,0 = 黑方。
 *   返回:   1 = 无,0 = 有。
 */
int no_legal_move(char map[8][8], int player);


#endif