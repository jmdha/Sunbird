#include <stdexcept>
#include <string>
#include <stdlib.h>

#include "../../classes/headers/constants.hh"
#include "../../classes/headers/board.hh"
#include "../../classes/headers/board_importer.hh"
#include "../../classes/headers/move_gen.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    Board board = Board();
    BoardImporter::ImportFEN(&board, (std::string) argv[2]);
    MoveGen moveGen = MoveGen(board.GetColor());
    MoveGen oppMoveGen = MoveGen(Utilities::GetOppositeColor(board.GetColor()));
    Move* moves = (Move*) calloc(MAXMOVECOUNT, sizeof(Move));
    Move* fakeMoves = (Move*) calloc(MAXMOVECOUNT, sizeof(Move));
    U64 priorAttacks[2] = { 0, 0 };
    // Generate attackboard
    oppMoveGen.GetAllMoves(fakeMoves, board, &priorAttacks);
    U64 attackSquares[2] = { 0, 0 };
    int moveCount = moveGen.GetKingMoves(moves, 0, board, (bool)std::atoi(argv[3]), &attackSquares, priorAttacks);
    free(moves);
    free(fakeMoves);

    if (moveCount == std::atoi(argv[1]))
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect move count | Expected " + (std::string)argv[1] + " - Actual " + std::to_string(moveCount));
}