#include "position.hpp"
#include "bit.hpp"
#include "bitboard.hpp"
#include "types.hpp"
#include "zobrist.hpp"

// access

uint64_t Position::GetHash() const noexcept { return _hash; }
Color Position::GetTurn() const noexcept { return static_cast<Color>(_misc & 0x1); }
Square Position::GetEP() const noexcept {
    const Square sq = static_cast<Square>((_misc >> 5) & 0x7f);
    assert(sq >= A1 && sq <= SQUARE_NONE);
    return sq;
}
Castling Position::GetCastling(Color color) const noexcept {
    assert(color == WHITE || color == BLACK);
    const Castling castling = static_cast<Castling>(
        ((_misc >> 1) & (0x3 << (2 * static_cast<uint16_t>(color)))) >>
        (2 * static_cast<uint16_t>(color))
    );
    assert(
        castling == Castling::None || castling == Castling::King || castling == Castling::Queen ||
        castling == Castling::All
    );
    return castling;
}
bool Position::AllowsCastling(Castling castling, Color color) const noexcept {
    return static_cast<int>(GetCastling(color) & castling) != 0;
}
Piece Position::GetType(Square square) const noexcept {
    assert(square != SQUARE_NONE);
    for (const auto pType : PIECES)
        if (GetPieces(pType) & square) return pType;
    return PIECE_NONE;
}
Color Position::GetColor(Square square) const noexcept {
    assert(square != SQUARE_NONE);
    if (square & _colorBB[static_cast<size_t>(WHITE)])
        return WHITE;
    else if (square & _colorBB[static_cast<size_t>(BLACK)])
        return BLACK;
    else
        return COLOR_NONE;
}
int Position::GetPieceCount(Color color, Piece pType) const noexcept {
    assert(color != COLOR_NONE);
    assert(pType != PIECE_NONE);
    return popcount(GetPieces(color, pType));
}
BB Position::GetPieces() const noexcept { return _colorBB[0] | _colorBB[1]; }
BB Position::GetPieces(Piece pType) const noexcept {
    assert(pType != PIECE_NONE);
    return _pieceBB[pType];
}
BB Position::GetPieces(Color color) const noexcept {
    assert(color != COLOR_NONE);
    return _colorBB[color];
}
BB Position::GetPieces(Color color, Piece pType) const noexcept {
    assert(color != COLOR_NONE);
    assert(pType != PIECE_NONE);
    return _colorBB[color] & _pieceBB[pType];
}
Square Position::GetKing(Color color) const noexcept {
    assert(color != COLOR_NONE);
    return static_cast<Square>(lsb(GetPieces(color, KING)));
}
bool Position::IsKingSafe(Color turn) const noexcept {
    Square king = GetKing(turn);

    BB occ     = GetPieces();
    BB pawns   = GetPieces(~turn, PAWN);
    BB knights = GetPieces(~turn, KNIGHT);
    BB bishops = (GetPieces(~turn, BISHOP) | GetPieces(~turn, QUEEN));
    BB rooks   = (GetPieces(~turn, ROOK) | GetPieces(~turn, QUEEN));

    // Checking jumpers
    if (PawnAttacks(king, turn) & pawns) return false;
    if (Attacks(king, KNIGHT) & knights) return false;

    if (Ray(king, NORTH) & rooks) [[unlikely]]
        if (lsb(Ray(king, NORTH) & rooks) == lsb(Ray(king, NORTH) & occ)) [[unlikely]]
            return false;
    if (Ray(king, EAST) & rooks) [[unlikely]]
        if (lsb(Ray(king, EAST) & rooks) == lsb(Ray(king, EAST) & occ)) [[unlikely]]
            return false;
    if (Ray(king, SOUTH) & rooks) [[unlikely]]
        if (msb(Ray(king, SOUTH) & rooks) == msb(Ray(king, SOUTH) & occ)) [[unlikely]]
            return false;
    if (Ray(king, WEST) & rooks) [[unlikely]]
        if (msb(Ray(king, WEST) & rooks) == msb(Ray(king, WEST) & occ)) [[unlikely]]
            return false;

    if (Ray(king, NORTH_EAST) & bishops) [[unlikely]]
        if (lsb(Ray(king, NORTH_EAST) & bishops) == lsb(Ray(king, NORTH_EAST) & occ)) [[unlikely]]
            return false;
    if (Ray(king, NORTH_WEST) & bishops) [[unlikely]]
        if (lsb(Ray(king, NORTH_WEST) & bishops) == lsb(Ray(king, NORTH_WEST) & occ)) [[unlikely]]
            return false;
    if (Ray(king, SOUTH_EAST) & bishops) [[unlikely]]
        if (msb(Ray(king, SOUTH_EAST) & bishops) == msb(Ray(king, SOUTH_EAST) & occ)) [[unlikely]]
            return false;
    if (Ray(king, SOUTH_WEST) & bishops) [[unlikely]]
        if (msb(Ray(king, SOUTH_WEST) & bishops) == msb(Ray(king, SOUTH_WEST) & occ)) [[unlikely]]
            return false;

    return true;
}

BB Position::GenerateAttackSquares(Color color) const noexcept {
    const BB allPieces = GetPieces();
    BB attacks         = 0;

    for (BB pieces = GetPieces(color, PAWN); pieces;)
        attacks |= PawnAttacks(lsb_pop(pieces), color);

    for (const Piece p : NON_PAWNS) {
        for (BB pieces = GetPieces(color, p); pieces;) {
            const Square sq = lsb_pop(pieces);
            BB attacks1     = Attacks(sq, p);

            for (BB b = allPieces & BAndB(sq, p); b != 0; b &= (b - 1))
                attacks1 &= ~XRay(sq, lsb(b));

            attacks |= attacks1;
        }
    }

    return attacks;
}

// modifiers

void Position::SetTurn(Color color) noexcept {
    assert(color == WHITE || color == BLACK);
    if (color != GetTurn()) _hash = Zobrist::FlipColor(_hash);
    _misc &= ~0x1;
    _misc |= static_cast<int>(color) & 0x1;
}
void Position::DisallowCastling(Castling castling, Color color) noexcept {
    assert(color == WHITE || color == BLACK);
    assert(
        castling == Castling::None || castling == Castling::King || castling == Castling::Queen ||
        castling == Castling::All
    );
    const Castling priorCastling = GetCastling(color);
    SetCastling(priorCastling & ~castling, color);
    if (priorCastling != GetCastling(color)) _hash = Zobrist::FlipCastling(_hash, color, castling);
}
void Position::SetCastling(Castling castling, Color color) noexcept {
    assert(color == WHITE || color == BLACK);
    assert(
        castling == Castling::None || castling == Castling::King || castling == Castling::Queen ||
        castling == Castling::All
    );
    _hash = Zobrist::FlipCastling(_hash, color, castling);
    _misc &= ~((0x3 << (2 * static_cast<uint16_t>(color))) << 1);
    _misc |= (static_cast<uint16_t>(castling) << (2 * static_cast<uint16_t>(color))) << 1;
}
void Position::SetEP(Square sq) noexcept {
    assert(sq >= A1 && sq <= SQUARE_NONE);
    _hash = Zobrist::FlipEnPassant(_hash, sq);
    _misc &= ~0xfe0;
    _misc |= (static_cast<uint16_t>(sq) & 0x7f) << 5;
}

void Position::PlacePiece(Square square, Piece pType, Color color) noexcept {
    assert(square != SQUARE_NONE);
    assert(pType != PIECE_NONE);
    assert(color != COLOR_NONE);
    _pieceBB[pType] |= square;
    _colorBB[color] |= square;
    _hash = Zobrist::FlipSquare(_hash, square, pType, color);
}
void Position::RemovePiece(Square square, Piece pType, Color color) noexcept {
    assert(square != SQUARE_NONE);
    assert(pType != PIECE_NONE);
    assert(color != COLOR_NONE);
    _pieceBB[pType] ^= square;
    _colorBB[color] ^= square;
    _hash = Zobrist::FlipSquare(_hash, square, pType, color);
}
