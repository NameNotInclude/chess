#ifndef MODE_H
#define MODE_H

/*
 * 根据传入的棋盘开始下棋。
 */
Record* play(char board[8][8], State pState, int curr);

/*
 * 分析棋谱。
 */
void analysis(Record* M);

#endif