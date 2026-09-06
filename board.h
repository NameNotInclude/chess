#ifndef BOARD_H
#define BOARD_H

/*
 * board.h —— 棋盘状态与基础工具模块(board.c)的公共接口
 *
 * 坐标系约定(所有函数/数组共用):
 *   - map[8][8] 中行下标 0~7 对应第 8~1 横线:
 *        map[0] = 第 8 横线(黑方底线,即黑方后、车、象……所在行)
 *        map[7] = 第 1 横线(白方底线)
 *   - 列下标 0~7 对应 a~h 文件。
 *   - 格内字符:大写字母 = 白方棋子,小写字母 = 黑方棋子,'-' = 空位。
 *
 * 行棋方:
 *   - curr = 1 表示轮到白方,curr = 0 表示轮到黑方。
 *   - 注意:board_print() 每次调用都会把 curr 取反——它既是
 *     “轮到谁走”,也被复用来决定打印时以哪一方视角朝下;
 *     因此若在游戏主循环中打印棋盘,行棋方会自动切换,请勿再手动翻转 curr。
 */

// 1 for white to play, 0 for black.
extern int curr;

// 当前对局棋盘(开局为标准初始局面)。
extern char board[8][8];

// 临时棋盘,供“试走/回滚”等场合复制/备份用。
extern char temp_board[8][8];

/*
 * 以当前行棋方(curr)视角打印整张棋盘(白方视角 a1 在左下,
 * 黑方视角 h8 在左下),打印结束后会把 curr 取反(见文件头注释)。
 *   map: 要打印的棋盘(通常传 board)。
 */
void board_print(char map[8][8]);

/*
 * 把 sou 棋盘整体复制到 des(8×8 全量拷贝)。
 */
void cp_board(char sou[8][8], char des[8][8]);

/*
 * 判断 player 的王是否正被对方将军。
 *   map:    棋盘;player: 1 = 白方,0 = 黑方。
 *   返回:   1 = 被将军,0 = 未被将军。
 */
int check(char map[8][8], int player);

#endif
