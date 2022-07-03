#include <iostream>

#include "../classes/headers/bit_board.hh"
#include "../classes/headers/board_importer.hh"
#include "../classes/headers/minimax.hh"
#include "../classes/headers/move_gen.hh"
#include "../classes/headers/perft.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, "r3k2r/p4ppp/1pnbB3/4p3/PpP5/3P1Pn1/8/3Kq3 w kq - 0 23");
    MoveGen moveGen = MoveGen(board.GetColor());
    MoveGen oppMoveGen = MoveGen(Utilities::GetOppositeColor(board.GetColor()));
    board.SetColor(Utilities::GetOppositeColor(board.GetColor()));
    Move* moves = (Move*) calloc(256, sizeof(Move));
    Move* fakeMoves = (Move*) calloc(256, sizeof(Move));
    U64 priorAttacks = 0;
    // Generate attackboard
    oppMoveGen.GetAllMoves(fakeMoves, board, &priorAttacks);
    board.SetColor(Utilities::GetOppositeColor(board.GetColor()));
    U64 attackSquares = 0;
    int moveCount = moveGen.GetKingMoves(moves, 0, board, 0, &attackSquares, priorAttacks);
    free(moves);
    free(fakeMoves);
}
