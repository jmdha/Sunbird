#include <stdexcept>
#include <string>
#include <stdlib.h>

#include <string>

#include "../../classes/headers/constants.hh"
#include "../../classes/headers/board.hh"
#include "../../classes/headers/board_importer.hh"
#include "../../classes/headers/move.hh"

int main(int argc, char* argv[]) {
    std::string FEN = std::string(argv[1]);
    Board board = Board();
    BoardImporter::ImportFEN(&board, FEN);    

    U64 priorHash = board.GetHash();

    char* sMove = argv[2];

    Square fromSquare = Utilities::GetSquare(sMove[0], sMove[1]);
    Square toSquare = Utilities::GetSquare(sMove[2], sMove[3]);

    PieceType toPiece = board.GetPiece(toSquare);
    MoveType moveType = (toPiece == PieceType::None) ? MoveType::Quiet : MoveType::Capture;

    Move move = Move(moveType, fromSquare, toSquare, board.GetSquareColor(fromSquare), board.GetSquareColor(toSquare), board.GetPiece(fromSquare), toPiece);

    board.DoMove(move);
    board.UndoMove(move);

    U64 newHash = board.GetHash();

    if (priorHash == newHash)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect Hash | Expected " + std::to_string(priorHash) + " - Actual " + std::to_string(newHash));
}