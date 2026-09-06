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

/* ---------- 易位相关状态(王/车是否已移动) ----------
 * 0 = 尚未移动;1 = 已移动。每局开始时都应复位为 0,
 * 由 move() 内部维护;也可以在新对局开始时直接清零。
 */
extern int WHITE_KING_MOVE;   /* 白王是否已移动               */
extern int BLACK_KING_MOVE;   /* 黑王是否已移动               */
extern int WHITE_A_ROOK_MOVE; /* 白方 a1 车是否已移动(后翼)   */
extern int WHITE_H_ROOK_MOVE; /* 白方 h1 车是否已移动(王翼)   */
extern int BLACK_A_ROOK_MOVE; /* 黑方 a8 车是否已移动(后翼)   */
extern int BLACK_H_ROOK_MOVE; /* 黑方 h8 车是否已移动(王翼)   */

/* ---------- 吃过路兵标记 ----------
 * 记录“最近一步是否是二格进兵”,以及该兵所在的列(0~7 = a~h 文件)。
 *   -2 = 上一手不是二格兵,不存在可吃的过路兵;
 *   >=0 = 该列刚走过二格兵,对方下一步可在此列吃过路兵(仅限一步之内有效)。
 * WHITE_EN 由白方二格进兵产生、供黑方吃;BLACK_EN 反之。
 */
extern int WHITE_EN;
extern int BLACK_EN;

/*
 * 判断 move 所指走法是否合法(只读,不修改棋盘)。
 *   move: 形如 "e2e4" 的普通走法字符串;
 *   返回: 0 = 非法,非 0 = 合法。
 */
int valid_move(char* move, char map[8][8]);

/*
 * 执行一步棋(普通走法或易位)。
 *   ctrl: 见文件头注释的着法格式;
 *   返回: 1 = 走法合法并已写入 map;0 = 非法(应保证 map 不被改动)。
 *   注:函数内部不负责切换行棋方 curr,也不处理升变(兵到底线)与吃过路兵
 *   时被吃子的移除——这些是当前实现遗留的问题,使用时需留意。
 */
int move(char ctrl[6], char map[8][8]);

#endif /* PIECE_H */
