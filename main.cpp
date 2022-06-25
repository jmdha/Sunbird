#include <iostream>

#include "classes/headers/bit_board.hh"
#include "classes/headers/board_importer.hh"
#include "classes/headers/minimax.hh"
#include "classes/headers/move_gen.hh"
#include "classes/headers/perft.hh"

int main(int, char**) {
     BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, "8/8/8/2RRR3/2RKR3/2RRR3/8/8 w - ");
    MoveGen moveGen = MoveGen(board.GetColor());
    Move* moves = (Move*) calloc(256, sizeof(Move));
    int moveCount = moveGen.GetKingMoves(moves, 0, board);
    free(moves);
}
