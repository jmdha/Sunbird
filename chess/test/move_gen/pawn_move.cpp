#include <stdexcept>
#include <stdlib.h>
#include <string>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <chess/internal/constants.hpp>
#include <chess/move_gen.hpp>

int main(int, char *argv[]) {
    Board board = Import::FEN((std::string)argv[2]);

    MoveList moves;
    MoveGen::Generate<MoveGen::GenType::All, PieceType::Pawn>(board, moves);

    if (moves.size() == std::atoi(argv[1]))
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect move count | Expected " + (std::string)argv[1] +
                               " - Actual " + std::to_string(moves.size()));
}
