#include <stdexcept>
#include <string>
#include <stdlib.h>

#include <string>

#include "../../classes/headers/constants.hh"
#include "../../classes/headers/bit_board.hh"
#include "../../classes/headers/board_importer.hh"
#include "../../classes/headers/move.hh"

int main(int argc, char* argv[]) {
    std::string FEN = std::string(argv[1]);
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, FEN);    

    U64 priorHash = board.GetHash();

    char* sMove = argv[2];

    Square fromSquare = Utilities::GetSquare(sMove[0], sMove[1]);
    Square toSquare = Utilities::GetSquare(sMove[2], sMove[3]);

    Move move = Move(fromSquare, toSquare, board.GetSquareColor(fromSquare), board.GetSquareColor(toSquare), board.GetPiece(fromSquare), board.GetPiece(toSquare));

    board.DoMove(move);
    board.UndoMove(move);

    U64 newHash = board.GetHash();

    if (priorHash == newHash)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect Hash | Expected " + std::to_string(priorHash) + " - Actual " + std::to_string(newHash));
}