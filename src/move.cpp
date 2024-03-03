#include "move.hpp"
#include "utilities.hpp"

Move::Move(Square origin, Square destination, Type type) noexcept
    : internal(
          static_cast<uint16_t>(origin) | (static_cast<uint16_t>(destination) << 6) |
          (static_cast<uint16_t>(type) << 12)
      ) {}

Move::Move(BB occ, BB kings, BB pawns, const std::string &move) noexcept {
    const Square ori   = Utilities::GetSquare(move[0], move[1]);
    const Square dst   = Utilities::GetSquare(move[2], move[3]);
    const bool capture = occ & dst;
    if (move.size() == 4) {
        if (capture)
            *this = Move(ori, dst, Capture);
        else if ((kings & ori) && ((ori == E1 && dst == G1) || (ori == E8 && dst == G8)))
            *this = Move(ori, dst, KingCastle);
        else if ((kings & ori) && ((ori == E1 && dst == C1) || (ori == E8 && dst == C8)))
            *this = Move(ori, dst, QueenCastle);
        else if ((pawns & ori) && (Utilities::GetColumnIndex(ori) != Utilities::GetColumnIndex(dst)) && !capture)
            *this = Move(ori, dst, EPCapture);
        else if ((pawns & ori) && (dst - ori > 8))
            *this = Move(ori, dst, DoublePawnPush);
        else
            *this = Move(ori, dst, Quiet);
    } else {
        const Piece piece = Utilities::GetPieceType(Utilities::GetPieceChar(move[4]));
        if (capture)
            *this = Move(ori, dst, static_cast<Type>((int)NPromotionCapture + piece - KNIGHT));
        else
            *this = Move(ori, dst, static_cast<Type>((int)NPromotion + piece - KNIGHT));
    }
}

Square Move::Origin() const noexcept { return static_cast<Square>(internal & 63); }

Square Move::Destination() const noexcept { return static_cast<Square>((internal >> 6) & 63); }

Move::Type Move::GetType() const noexcept { return static_cast<Type>(internal >> 12); }
bool Move::IsDefined() const noexcept { return internal != 0; }

bool Move::IsCapture() const noexcept { return internal & (4 << 12); }

bool Move::IsPromotion() const noexcept { return internal & (8 << 12); }

bool Move::IsEnPassant() const noexcept { return (internal >> 12) == EPCapture; }

bool Move::IsDouble() const noexcept { return (internal >> 12) == DoublePawnPush; }

bool Move::IsKingCastle() const noexcept { return (internal >> 12) == KingCastle; }

bool Move::IsQueenCastle() const noexcept { return (internal >> 12) == QueenCastle; }

bool Move::IsCastle() const noexcept {
    const Type type = GetType();
    return type == KingCastle || type == QueenCastle;
}

Piece Move::PromotionPiece() const noexcept {
    return static_cast<Piece>((internal >> 12) - 7 - (IsCapture() ? 4 : 0));
}

bool Move::operator==(const Move &move) const noexcept { return this->internal == move.internal; }

std::string Move::Export() const noexcept {
    const Square ori = Origin();
    const Square dst = Destination();
    std::string s    = std::string(SQUARE_NAMES[ori]) + std::string(SQUARE_NAMES[dst]);
    if (IsPromotion()) s += PIECE_CHARS[BLACK][PromotionPiece()];
    return s;
}
