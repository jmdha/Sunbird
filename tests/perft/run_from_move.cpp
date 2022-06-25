#include <stdexcept>
#include <string>
#include <stdlib.h>

#include <string>

#include "../../classes/headers/constants.hh"
#include "../../classes/headers/bit_board.hh"
#include "../../classes/headers/board_importer.hh"
#include "../../classes/headers/move.hh"
#include "../../classes/headers/perft.hh"

int main(int argc, char* argv[]) {
    int expectedLeafCount = std::atoi(argv[1]);

    std::string FEN = std::string(argv[2]);
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, FEN);    

    char* sMove = argv[3];
    int depth = std::atoi(argv[4]);

    Square fromSquare = Utilities::GetSquare(sMove[0], sMove[1]);
    Square toSquare = Utilities::GetSquare(sMove[2], sMove[3]);

    Move move = Move(MoveType::Quiet, fromSquare, toSquare, board.GetSquareColor(fromSquare), board.GetSquareColor(toSquare), board.GetPiece(fromSquare), board.GetPiece(toSquare));

    Perft perft = Perft(&board);
    int actualLeafCount = perft.RunFromMove(move, depth);

    if (expectedLeafCount == actualLeafCount)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect leaf count | Expected " + std::to_string(expectedLeafCount) + " - Actual " + std::to_string(actualLeafCount));
}