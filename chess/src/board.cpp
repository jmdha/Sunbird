#include "chess/internal/utilities.hpp"
#include "jank/bit/bit.hpp"
#include <chess/board.hpp>
#include <stdexcept>

using jank::bit::lsb;

using namespace Chess;
Board::Board(Position position) { _positions.push(std::move(position)); }

const Position &Board::Pos() const noexcept { return _positions.top(); }

void Board::MakeMove(const Move &move) noexcept {
    Position pos = Pos();
    const Square &from = move.GetFrom();
    const Square &to = move.GetTo();
    const Color &turn = pos.GetTurn();
    const Color &oppColor = Utilities::GetOppositeColor(turn);
    pos.SetTurn(oppColor);
    PieceType pType = pos.GetType(move.GetFrom());
    PieceType newPType = pType;
    PieceType capturedPiece = pos.GetType(move.GetTo());
    Square captureTo = to;
    switch (move.GetType()) {
    case MoveType::KingCastle:
        if (turn == Color::White) {
            pos.PlacePiece(Square::G1, PieceType::King, Color::White);
            pos.PlacePiece(Square::F1, PieceType::Rook, Color::White);
            pos.RemovePiece(Square::E1, PieceType::King, Color::White);
            pos.RemovePiece(Square::H1, PieceType::Rook, Color::White);
        } else {
            pos.PlacePiece(Square::G8, PieceType::King, Color::Black);
            pos.PlacePiece(Square::F8, PieceType::Rook, Color::Black);
            pos.RemovePiece(Square::E8, PieceType::King, Color::Black);
            pos.RemovePiece(Square::H8, PieceType::Rook, Color::Black);
        }
        pos.DisallowCastling(Castling::All, turn);
        break;
    case MoveType::QueenCastle:
        if (turn == Color::White) {
            pos.PlacePiece(Square::C1, PieceType::King, Color::White);
            pos.PlacePiece(Square::D1, PieceType::Rook, Color::White);
            pos.RemovePiece(Square::E1, PieceType::King, Color::White);
            pos.RemovePiece(Square::A1, PieceType::Rook, Color::White);
        } else {
            pos.PlacePiece(Square::C8, PieceType::King, Color::Black);
            pos.PlacePiece(Square::D8, PieceType::Rook, Color::Black);
            pos.RemovePiece(Square::E8, PieceType::King, Color::Black);
            pos.RemovePiece(Square::A8, PieceType::Rook, Color::Black);
        }
        pos.DisallowCastling(Castling::All, turn);
        break;
    case MoveType::NPromotion:
        newPType = PieceType::Knight;
        goto QUIET_PROMOTION;
    case MoveType::BPromotion:
        newPType = PieceType::Bishop;
        goto QUIET_PROMOTION;
    case MoveType::RPromotion:
        newPType = PieceType::Rook;
        goto QUIET_PROMOTION;
    case MoveType::QPromotion:
        newPType = PieceType::Queen;
        goto QUIET_PROMOTION;
    case MoveType::DoublePawnPush:
    case MoveType::Quiet:
    QUIET_PROMOTION:
        pos.RemovePiece(from, pType, turn);
        pos.PlacePiece(to, newPType, turn);
        break;
    case MoveType::NPromotionCapture:
        newPType = PieceType::Knight;
        goto CAPTURE_PROMOTION;
    case MoveType::BPromotionCapture:
        newPType = PieceType::Bishop;
        goto CAPTURE_PROMOTION;
    case MoveType::RPromotionCapture:
        newPType = PieceType::Rook;
        goto CAPTURE_PROMOTION;
    case MoveType::QPromotionCapture:
        newPType = PieceType::Queen;
        goto CAPTURE_PROMOTION;
    case MoveType::EPCapture:
        capturedPiece = PieceType::Pawn;
        if (turn == Color::White)
            captureTo = (Square)lsb(Shift<Direction::South>(ToBB(captureTo)));
        else
            captureTo = (Square)lsb(Shift<Direction::North>(ToBB(captureTo)));
    case MoveType::Capture:
    CAPTURE_PROMOTION:
        pos.RemovePiece(from, pType, turn);
        pos.RemovePiece(captureTo, capturedPiece, Utilities::GetOppositeColor(turn));
        pos.PlacePiece(to, newPType, turn);
            if (capturedPiece == PieceType::Rook &&
                to == initRookPos[(int)oppColor][(int)Castling::King - 1])
                if (pos.AllowsCastling(Castling::King, oppColor))
                    pos.DisallowCastling(Castling::King, oppColor);
            if (capturedPiece == PieceType::Rook &&
                to == initRookPos[(int)oppColor][(int)Castling::Queen - 1])
                if (pos.AllowsCastling(Castling::Queen, oppColor))
                    pos.DisallowCastling(Castling::Queen, oppColor);
        break;
    default:
        std::invalid_argument("Unexpected move type: " + std::to_string((int)move.GetType()));
    }

    if (move.GetType() == MoveType::DoublePawnPush) [[unlikely]]
        pos.SetEP(Utilities::GetColumn(move.GetFrom()));
    else
        pos.SetEP(Column::None);

    if (pType == PieceType::King) [[unlikely]]
        pos.DisallowCastling(Castling::All, turn);
    else if (pType == PieceType::Rook) [[unlikely]] {
        if (move.GetFrom() == (turn == Color::White ? Square::H1 : Square::H8)) [[unlikely]]
            pos.DisallowCastling(Castling::King, turn);
        else if (move.GetFrom() == (turn == Color::White ? Square::A1 : Square::A8)) [[unlikely]]
            pos.DisallowCastling(Castling::Queen, turn);
    }

    _positions.push(std::move(pos));
}

void Board::UndoMove() noexcept { _positions.pop(); }
