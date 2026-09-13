#ifndef SAVE_H
#define SAVE_H

#include "piece.h"   /* State 定义(供 game.h 使用) */
#include "game.h"    /* Record / MPtr */

/*
 * save.h —— 棋谱(Record*)的持久化:把棋谱写到文本文件,
 *           下次程序启动时再读回来,实现“跨进程长期保存”。
 *
 * 文件格式(纯文本,每行一步,行首 '#' 为注释/文件头):
 *     # chess record v1
 *     e2e4 e4
 *     e7e5 e5
 *   第一列 = 程序输入着法(detail_move,如 e2e4 / O-O / e7e8q)
 *   第二列 = 标准代数记法(move,如 e4 / Nf3 / f8=Q#)
 *
 * 说明:
 *   - 只保存主变(next 链);变例(next_varr/prev_varr)目前没有启用,
 *     以后启用时需要在这里补充保存/读取/释放。
 *   - 读取出来的新棋谱由调用方负责用 free_record() 释放。
 */

/* 默认存档文件名,可传给 save_record / load_record */
#define RECORD_FILE "record.txt"

/*
 * 把棋谱 M 的主变保存到文本文件 path(存在则覆盖)。
 * 返回:1 = 成功;0 = 失败(M 或 path 为空、文件打不开)。
 */
int save_record(Record* M, const char* path);

/*
 * 从文本文件 path 读取棋谱,返回一份新的 Record*(head + 主变链)。
 * 返回:成功 = 新棋谱指针;失败(参数为空、文件打不开、内存不足)= NULL。
 * 调用方用完必须 free_record()。
 */
Record* load_record(const char* path);

#endif /* SAVE_H */
