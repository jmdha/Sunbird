#include <iostream>

#include "classes/headers/bit_board.hh"
#include "classes/headers/board_importer.hh"
#include "classes/headers/minimax.hh"
#include "classes/headers/move_gen.hh"
#include "classes/headers/perft.hh"

int main(int, char**) {
    BitBoard board = BitBoard();
    board.Initialize();
    
    MiniMax mini = MiniMax(&board);
    Move move = mini.GetBestMove(6);
    printf("Done\n");
}
