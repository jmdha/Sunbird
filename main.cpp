#include <iostream>

#include "classes/headers/bit_board.hh"
#include "classes/headers/board_importer.hh"
#include "classes/headers/minimax.hh"
#include "classes/headers/move_gen.hh"
#include "classes/headers/perft.hh"

int main(int, char**) {
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, "r3k2r/8/8/8/8/8/8/8 b kq - 0 1");
    MoveGen moveGen = MoveGen(board.GetColor());
    MoveGen oppMoveGen = MoveGen(Utilities::GetOppositeColor(board.GetColor()));
    board.SetColor(Utilities::GetOppositeColor(board.GetColor()));
    Move* moves = (Move*) calloc(256, sizeof(Move));
    Move* fakeMoves = (Move*) calloc(256, sizeof(Move));
    U64 attackMoves = 0;
    // Generate attackboard
    oppMoveGen.GetAllMoves(fakeMoves, board, &attackMoves);
    board.SetColor(Utilities::GetOppositeColor(board.GetColor()));
    int moveCount = moveGen.GetKingMoves(moves, 0, board, 0, attackMoves);
    free(moves);
    free(fakeMoves);
}
