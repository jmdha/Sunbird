#include "chess/internal/utilities.hpp"
#include "jank/bit/bit.hpp"
#include <chess/board.hpp>
#include <stdexcept>

using jank::bit::lsb;

using namespace Chess;
Board::Board(Position position) { _positions.push(std::move(position)); }

bool Board::IsThreefoldRepetition() const noexcept {
    uint64_t hash = Pos().GetHash();
    int count = 0;
    for (int i = 0; i < _positions.size(); i++)
        if (hash == _positions.at(i).GetHash())
            count++;
    return count > 2;
}

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
    case MoveType::QueenCastle: {
        const int castleIndex = (int)move.GetType() - 2;
        pos.PlacePiece(CASTLEPOS_KING[(int)turn][castleIndex], PieceType::King, turn);
        pos.PlacePiece(CASTLEPOS_ROOK[(int)turn][castleIndex], PieceType::Rook, turn);
        pos.RemovePiece(INIT_KINGPOS[(int)turn], PieceType::King, turn);
        pos.RemovePiece(INIT_ROOKPOS[(int)turn][castleIndex], PieceType::Rook, turn);
        pos.DisallowCastling(Castling::All, turn);
        break;
        }
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
            to == INIT_ROOKPOS[(int)oppColor][(int)Castling::King - 1])
            if (pos.AllowsCastling(Castling::King, oppColor))
                pos.DisallowCastling(Castling::King, oppColor);
        if (capturedPiece == PieceType::Rook &&
            to == INIT_ROOKPOS[(int)oppColor][(int)Castling::Queen - 1])
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
        if (move.GetFrom() == INIT_ROOKPOS[(int)turn][(int)Castling::King - 1]) [[unlikely]]
            pos.DisallowCastling(Castling::King, turn);
        else if (move.GetFrom() == INIT_ROOKPOS[(int)turn][(int)Castling::Queen - 1]) [[unlikely]]
            pos.DisallowCastling(Castling::Queen, turn);
    }

    _positions.push(std::move(pos));
    _moves++;
}

void Board::UndoMove() noexcept { _positions.pop(); }
