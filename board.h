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
 */

// 当前对局棋盘(开局为标准初始局面)。
extern char board[8][8];

// 临时棋盘,供“试走/回滚”等场合复制/备份用。
extern char temp_board[8][8];

/*
 * 以当前行棋方(curr)视角打印整张棋盘(白方视角 a1 在左下,
 * 黑方视角 h8 在左下)。
 *   map: 要打印的棋盘(通常传 board)。
 */
void board_print(char map[8][8],int player);

/*
 * 把 sou 棋盘整体复制到 des(8×8 全量拷贝)。
 */
void cp_board(char sou[8][8], char des[8][8]);

#endif
