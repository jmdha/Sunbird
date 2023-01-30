#include <stdexcept>
#include <string>
#include <cstdlib>

#include <string>

#include "constants.hh"
#include "board.hh"
#include "board_importer.hh"

int main(int argc, char* argv[]) {
    std::string FEN = std::string(argv[1]);
    Board board = Board();
    BoardImporter::ImportFEN(&board, FEN);    

    Board priorBoard = board;

    std::string sMove = std::string(argv[2]);
    Move move = Move(MoveType::Quiet);
    if (sMove == "O-O-O" || sMove == "o-o-o")
        move = Move(MoveType::QueenCastle);
    else if (sMove == "O-O" || sMove == "o-o")
        move = Move(MoveType::KingCastle);
    else {
        Square fromSquare = Utilities::GetSquare(sMove[0], sMove[1]);
        Square toSquare = Utilities::GetSquare(sMove[2], sMove[3]);

        PieceType fromPiece = board.GetType(fromSquare);
        PieceType toPiece = board.GetType(toSquare);
        if (fromPiece == PieceType::Pawn && Utilities::GetColumn(fromSquare) != Utilities::GetColumn(toSquare) && toPiece == PieceType::None)
            move = Move(MoveType::EPCapture, fromSquare, toSquare, PieceType::None);
        else if (fromPiece == PieceType::Pawn && sMove.size() == 5) {
            if (sMove[4] == 'q')
                move = Move((toPiece == PieceType::None) ? MoveType::QPromotion : MoveType::QPromotionCapture, fromSquare, toSquare, toPiece);
            else if (sMove[4] == 'r')
                move = Move((toPiece == PieceType::None) ? MoveType::RPromotion : MoveType::RPromotionCapture, fromSquare, toSquare, toPiece);
            else if (sMove[4] == 'b')
                move = Move((toPiece == PieceType::None) ? MoveType::BPromotion : MoveType::BPromotionCapture, fromSquare, toSquare, toPiece);
            else if (sMove[4] == 'n')
                move = Move((toPiece == PieceType::None) ? MoveType::NPromotion : MoveType::NPromotionCapture, fromSquare, toSquare, toPiece);
        } else {
            MoveType moveType = (toPiece == PieceType::None) ? MoveType::Quiet : MoveType::Capture;

            move = Move(moveType, fromSquare, toSquare, toPiece);
        }
    }

    board.DoMove(move);
    board.UndoMove(move);

    if (priorBoard == board)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Unexpected changes from do-undo move");
}