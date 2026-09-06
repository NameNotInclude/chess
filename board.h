#ifndef BOARD_H
#define BOARD_H

// 1 for white to play, 0 for black.
extern int curr;
extern char board[8][8];
extern char temp_board[8][8];

void board_print(char map[8][8]);
void cp_board(char sou[8][8], char des[8][8]);
int check(char map[8][8], int player);

#endif