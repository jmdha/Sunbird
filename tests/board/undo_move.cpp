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

    Board priorBoard = board;

    std::string sMove = std::string(argv[2]);
    Move move = Move(MoveType::Quiet);
    if (sMove == "O-O-O" || sMove == "o-o-o") {
        if (board.GetColor() == Color::White)
            move = Move(MoveType::QueenCastle, Square::E1, Square::C1);
        else
            move = Move(MoveType::QueenCastle, Square::E8, Square::C8);
    } else if (sMove == "O-O" || sMove == "o-o") {
        if (board.GetColor() == Color::White)
            move = Move(MoveType::KingCastle, Square::E1, Square::G1);
        else
            move = Move(MoveType::KingCastle, Square::E8, Square::G8);
    } else {
        Square fromSquare = Utilities::GetSquare(sMove[0], sMove[1]);
        Square toSquare = Utilities::GetSquare(sMove[2], sMove[3]);

        PieceType toPiece = board.GetType(toSquare);
        MoveType moveType = (toPiece == PieceType::None) ? MoveType::Quiet : MoveType::Capture;

        move = Move(moveType, fromSquare, toSquare, toPiece);
    }

    board.DoMove(move);
    board.UndoMove(move);

    if (priorBoard == board)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Unexpected changes from do-undo move");
}