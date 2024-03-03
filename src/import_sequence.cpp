#include "import.hpp"
#include "utilities.hpp"

namespace Import {
// HACK: Refactor this
void MoveSequence(Board &board, std::string moves) {
    std::string move;
    for (size_t i = 0; i < moves.length(); i++) {
        if (moves[i] != ' ') move.push_back(moves[i]);

        if (moves[i] == ' ' || i == moves.length() - 1) {
            Square fromSquare = Utilities::GetSquare(move[0], move[1]);
            Square toSquare   = Utilities::GetSquare(move[2], move[3]);
            Piece fromType    = board.Pos().GetType(fromSquare);
            Piece toType      = board.Pos().GetType(toSquare);
            Color fromColor   = board.Pos().GetColor(fromSquare);
            Move::Type type;
            // Is castling?
            //// Is kingside castle
            if (fromType == KING && (move == "e1g1" || move == "e8g8")) type = Move::KingCastle;
            //// Is queenside castle
            else if (fromType == KING && (move == "e1c1" || move == "e8c8"))
                type = Move::QueenCastle;

            else if (fromType == PAWN) {
                // If double pawn push
                if (std::abs(
                        (int)Utilities::GetRowIndex(fromSquare) -
                        (int)Utilities::GetRowIndex(toSquare)
                    ) == 2)
                    type = Move::DoublePawnPush;
                // If promotion
                else if (Utilities::GetRow(toSquare) == ((fromColor == WHITE) ? Row::Row8 : Row::Row1)) {
                    // If none capture promotion
                    if (toType == PIECE_NONE) {
                        if (moves.length() == 5) {
                            if (move[4] == 'r')
                                type = Move::RPromotion;
                            else if (move[4] == 'n')
                                type = Move::NPromotion;
                            else if (move[4] == 'b')
                                type = Move::BPromotion;
                        } else
                            type = Move::QPromotion;
                    } else {
                        // Capture promotion
                        if (moves.length() == 5) {
                            if (move[4] == 'r')
                                type = Move::RPromotionCapture;
                            else if (move[4] == 'n')
                                type = Move::NPromotionCapture;
                            else if (move[4] == 'b')
                                type = Move::BPromotionCapture;
                        } else
                            type = Move::QPromotionCapture;
                    }
                } else if (toType != PIECE_NONE)
                    type = Move::Capture;
                else {
                    if (Utilities::GetColumn(fromSquare) != Utilities::GetColumn(toSquare))
                        type = Move::EPCapture;
                    else
                        type = Move::Quiet;
                }
            } else if (toType != PIECE_NONE)
                type = Move::Capture;
            else
                type = Move::Quiet;
            Move tempMove = Move(fromSquare, toSquare, type);
            board.MakeMove(tempMove);
            move = "";
        }
    }
}

Board MoveSequence(std::string moves) {
    Board board = Import::FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    MoveSequence(board, moves);
    return board;
}

} // namespace Import
