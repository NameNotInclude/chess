#ifndef PIECE_H
#define PIECE_H

/*
 * piece.h —— 走法合法性判定与行棋模块(piece.c)的公共接口
 *
 * 坐标与阵营约定(与 board.h 一致):
 *   - 棋盘 map[8][8]:map[0] 是第 8 横线(黑方底线),map[7] 是第 1 横线(白方底线);
 *   - 大写字母 = 白方棋子,小写字母 = 黑方棋子,'-' = 空位;
 *   - 轮到谁走棋由 board.h 中的全局 curr 决定:1 = 白方,0 = 黑方。
 *
 * 着法字符串格式(move 的 ctrl):
 *   - 普通走法:代数记法,4 个小写字符 + '\0',例如 "e2e4"(起点 + 终点);
 *   - 易位:"O-O"(王翼短易位)或 "O-O-O"(后翼长易位),大小写不敏感(o-o 亦可)。
 */

#include "board.h"

typedef struct state
{
    int WHITE_KING_MOVE;
    int BLACK_KING_MOVE;

    int WHITE_A_ROOK_MOVE;
    int WHITE_H_ROOK_MOVE;
    int BLACK_A_ROOK_MOVE;
    int BLACK_H_ROOK_MOVE;

    int WHITE_EN;
    int BLACK_EN;

}State;



/*
 * 判断 move 所指走法是否合法(只读,不修改棋盘)。
 *   move: 形如 "e2e4" 的普通走法字符串;
 *   返回: 0 = 非法,非 0 = 合法。
 */
int valid_move(const char *move, char map[8][8], int player);

/*
 * 执行一步棋(普通走法、吃过路兵或易位)。
 *   ctrl: 见文件头注释的着法格式;
 *   返回: 1 = 走法合法并已写入 map;0 = 非法(此时 map 不被改动)。
 */
int move(const char *ctrl, char map[8][8], int player);

/*
 * 更新棋子状态(王移动、车移动、过路兵)
 * comm来自于move函数的返回
 */
void State_Update(int comm, int player);

/* 打印当前对局状态，供调试使用。 */
void State_Print(void);

#endif /* PIECE_H */
