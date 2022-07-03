#include <iostream>

#include "classes/headers/bit_board.hh"
#include "classes/headers/board_importer.hh"
#include "classes/headers/minimax.hh"
#include "classes/headers/move_gen.hh"
#include "classes/headers/perft.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, "8/8/8/8/4P3/r6K/6P1/8 w - - 0 1");
    MoveGen moveGen = MoveGen(board.GetColor());

    Move* moves = (Move*) calloc(256, sizeof(Move));
    U64 attackMoves = 0;
    int moveCount = moveGen.GetPawnMoves(moves, 0, board, 0, &attackMoves);
    free(moves);
}
