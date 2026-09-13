#ifndef GAME_H
#define GAME_H

typedef struct node
{
    char* detail_move;  //详细的招法：如e2e4
    char* move;   //标准记法：如Nf3
    
    struct node* next;  //下一招
    struct node* prev;  //上一招

    //分析棋局时，可以对同一个局面采用不同的走法
    struct node* next_varr;  //下一个变例
    struct node* prev_varr;  //上一个变例
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
int no_legal_move(char map[8][8], int player, State* pState);

/*
 * 把一步着法追加到棋谱主变的末尾(挂在 M->last 之后)。
 *   ctrl : 程序输入格式的着法,如 "e2e4";易位为 "o-o" / "O-O-O";
 *          兵升变可以写成 5 个字符,如 "e7e8q"(第 5 位是升变子);
 *   piece: 被移动的棋子(用于识别兵升变;不需要时传 0);
 *   turn : 该着法由哪一方走出,1 = 白方,0 = 黑方。
 *
 * 记录内容:
 *   detail_move —— 完整保留调用方传入的着法串(含升变子),可用于回放;
 *   move        —— 存的是标准代数记法(SAN)。
 */
Record* init(void);

/*
 * 将新的一步添加到棋谱当中
 *    ctrl :程序输入的招法
 *    stan :标准代数记法
 */
void add(Record* M,char* ctrl,char* stan);

/*
 * 释放整份棋谱:主变链上的每个节点,以及节点持有的 detail_move / move 字符串。
 * 说明:变例链(next_varr / prev_varr)目前没有使用;若以后要用,
 *       这里需要一并遍历释放。
 */
void free_record(Record* M);
#endif