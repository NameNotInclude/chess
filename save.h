#ifndef SAVE_H
#define SAVE_H

#include "piece.h"   /* State 定义(供 game.h 使用) */
#include "game.h"    /* Record / MPtr */

/*
 * save.h —— 棋谱(Record*)的保存:
 *   1) 进程内保存指针:store_record() 把 play 返回的棋谱收进“棋谱库”,
 *      之后用 get_record() 直接拿同一个指针去分析,不需要时 free_all_records()。
 *      (指针只在本次程序运行内有效;跨程序运行要用下面的文件保存)
 *   2) 文件保存:save_record() 写入文本文件,load_record() 下次启动读回来。
 *
 * 文件格式(纯文本,每行一步,行首 '#' 为注释/文件头):
 *     # chess record v1
 *     e2e4 e4
 *     e7e5 e5
 *   第一列 = 程序输入着法(detail_move,如 e2e4 / O-O / e7e8q)
 *   第二列 = 标准代数记法(move,如 e4 / Nf3 / f8=Q#)
 *
 * 关于分支(变例):
 *   同一局面下有多条着法时,它们通过 next_varr 串在一起(主线着法在最前,
 *   后面依次是各个变例)。save_record() 遇到这种分叉会询问用户保留哪一条,
 *   只把选中的那条分支写进文件。
 */

/* 默认存档文件名,可传给 save_record / load_record */
#define RECORD_FILE "record.txt"

/* ---------- 进程内棋谱库:保存指针,供分析直接使用 ---------- */

/*
 * 把棋谱指针存入棋谱库。
 * 注意:所有权移交,存进去之后不要再对这个指针调用 free_record(),
 *       统一由 free_all_records() 释放。
 * 返回:1 = 成功;0 = 失败(M 为空或内存不足)。
 */
int store_record(Record* M);

/*
 * 取出第 index 份棋谱(从 0 开始)的指针,直接用于分析。
 * 返回的是库里那个指针本身,不是副本,不要释放它。
 * index 越界返回 NULL。
 */
Record* get_record(int index);

/* 棋谱库里保存了多少份棋谱 */
int record_count(void);

/* 释放棋谱库中的所有棋谱并清空库(不需要时调用) */
void free_all_records(void);

/* ---------- 文件保存 / 读取 ---------- */

/*
 * 把棋谱 M 保存到文本文件 path(存在则覆盖)。
 * 遇到分支(next_varr)会在终端询问用户保留哪一条分支,
 * 若输入结束/输入非法则默认保留主线。
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
