#include "board.hpp"
#include "bit.hpp"
#include "utilities.hpp"
#include <stdexcept>

Board::Board(Position position) { positions.at(current_ply++) = std::move(position); }

bool Board::IsThreefoldRepetition() const noexcept {
    uint64_t hash = Pos().GetHash();
    for (size_t i = 0; i < current_ply - 1; i++)
        if (hash == positions.at(i).GetHash()) return true;
    return false;
}

void Board::MakeMove(const Move &move) noexcept {
    Position pos       = Pos();
    const Square &from = move.GetFrom();
    const Square &to   = move.GetTo();
    const Color &turn  = pos.GetTurn();
    pos.SetTurn(~turn);
    Piece pType         = pos.GetType(move.GetFrom());
    Piece newPType      = pType;
    Piece capturedPiece = pos.GetType(move.GetTo());
    Square captureTo    = to;
    Column ep           = Column::None;
    switch (move.GetType()) {
    case MoveType::KingCastle:
    case MoveType::QueenCastle: {
        const int castleIndex = (int)move.GetType() - 2;
        pos.PlacePiece(CASTLEPOS_KING[(int)turn][castleIndex], KING, turn);
        pos.PlacePiece(CASTLEPOS_ROOK[(int)turn][castleIndex], ROOK, turn);
        pos.RemovePiece(INIT_KINGPOS[(int)turn], KING, turn);
        pos.RemovePiece(INIT_ROOKPOS[(int)turn][castleIndex], ROOK, turn);
        pos.DisallowCastling(Castling::All, turn);
        break;
    }
    case MoveType::NPromotion: newPType = KNIGHT; goto QUIET_PROMOTION;
    case MoveType::BPromotion: newPType = BISHOP; goto QUIET_PROMOTION;
    case MoveType::RPromotion: newPType = ROOK; goto QUIET_PROMOTION;
    case MoveType::QPromotion: newPType = QUEEN; goto QUIET_PROMOTION;
    case MoveType::DoublePawnPush: ep = Utilities::GetColumn(from);
    case MoveType::Quiet:
    QUIET_PROMOTION:
        pos.RemovePiece(from, pType, turn);
        pos.PlacePiece(to, newPType, turn);
        break;
    case MoveType::NPromotionCapture: newPType = KNIGHT; goto CAPTURE_PROMOTION;
    case MoveType::BPromotionCapture: newPType = BISHOP; goto CAPTURE_PROMOTION;
    case MoveType::RPromotionCapture: newPType = ROOK; goto CAPTURE_PROMOTION;
    case MoveType::QPromotionCapture: newPType = QUEEN; goto CAPTURE_PROMOTION;
    case MoveType::EPCapture:
        capturedPiece = PAWN;
        if (turn == WHITE)
            captureTo = (Square)lsb(Shift<Direction::South>(ToBB(captureTo)));
        else
            captureTo = (Square)lsb(Shift<Direction::North>(ToBB(captureTo)));
    case MoveType::Capture:
    CAPTURE_PROMOTION:
        pos.RemovePiece(from, pType, turn);
        pos.RemovePiece(captureTo, capturedPiece, ~turn);
        pos.PlacePiece(to, newPType, turn);
        if (capturedPiece == ROOK && to == INIT_ROOKPOS[(int)~turn][(int)Castling::King - 1])
            if (pos.AllowsCastling(Castling::King, ~turn))
                pos.DisallowCastling(Castling::King, ~turn);
        if (capturedPiece == ROOK && to == INIT_ROOKPOS[(int)~turn][(int)Castling::Queen - 1])
            if (pos.AllowsCastling(Castling::Queen, ~turn))
                pos.DisallowCastling(Castling::Queen, ~turn);
        break;
    default: std::invalid_argument("Unexpected move type: " + std::to_string((int)move.GetType()));
    }

    pos.SetEP(ep);

    if (pType == KING) [[unlikely]]
        pos.DisallowCastling(Castling::All, turn);
    else if (pType == ROOK) [[unlikely]] {
        if (move.GetFrom() == INIT_ROOKPOS[(int)turn][(int)Castling::King - 1]) [[unlikely]]
            pos.DisallowCastling(Castling::King, turn);
        else if (move.GetFrom() == INIT_ROOKPOS[(int)turn][(int)Castling::Queen - 1]) [[unlikely]]
            pos.DisallowCastling(Castling::Queen, turn);
    }

    positions.at(current_ply++) = pos;
    _moves++;
}

void Board::UndoMove() noexcept { current_ply--; }
