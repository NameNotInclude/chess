#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "piece.h"
#include "game.h"
#include "mode.h"
#include "save.h"

int main()
{
    char board[8][8];
    State pState={0,0,0,0,0,0,-2,-2};

    cp_board(init_board,board);

    Record* m=play(board,pState,1);

    printf("%p\n",m);

    analysis(m);
    
    save_record(m,"record/record1.txt");
    save_record(m,"record/record2.txt");

    free_record(m);
    return 0;
}