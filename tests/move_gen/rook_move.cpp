#include <stdexcept>
#include <string>
#include <stdlib.h>

#include "../../classes/headers/constants.hh"
#include "../../classes/headers/bit_board.hh"
#include "../../classes/headers/board_importer.hh"
#include "../../classes/headers/move_gen.hh"

int main(int argc, char* argv[]) {
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, (std::string) argv[2]);
    MoveGen moveGen = MoveGen(board.GetColor());

    Move* moves = (Move*) calloc(256, sizeof(Move));
    int moveCount = moveGen.GetRookMoves(moves, 0, board);
    free(moves);

    if (moveCount == std::atoi(argv[1]))
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect move count | Expected " + (std::string)argv[1] + " - Actual " + std::to_string(moveCount));
}