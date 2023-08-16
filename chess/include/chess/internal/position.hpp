#ifndef CHESS_POSITION
#define CHESS_POSITION

#include "bitboard.hpp"
#include "chess/internal/utilities.hpp"
#include "constants.hpp"
#include "move.hpp"
#include "zobrist.hpp"
#include <jank/bit/bit.hpp>

namespace Chess {
struct Position {
public:
    // access

    inline uint64_t GetHash() const noexcept;
    inline Color GetTurn() const noexcept;
    inline Column GetEP() const noexcept;
    inline Castling GetCastling(Color color) const noexcept;
    inline bool AllowsCastling(Castling castling, Color color) const noexcept;

    inline PieceType GetType(Square square) const noexcept;
    inline Color GetColor(Square square) const noexcept;
    inline int GetPieceCount(Color color, PieceType pType) const noexcept;
    inline BB GetPieces() const noexcept;
    inline BB GetPieces(PieceType pType) const noexcept;
    inline BB GetPieces(Color color) const noexcept;
    inline BB GetPieces(Color color, PieceType pType) const noexcept;

    bool IsKingSafe(BB tempOccuracyBoard, BB tempEnemyBoard, BB tempKingBoard) const noexcept;
    inline bool IsKingSafe(BB tempOccuracyBoard, BB tempEnemyBoard) const noexcept;
    inline bool IsKingSafe(BB tempOccuracyBoard) const noexcept;
    inline bool IsKingSafe() const noexcept;

    BB GenerateAttackSquares(Color color) const noexcept;

    // modifiers

    inline void SetTurn(Color color) noexcept;
    inline void SetCastling(Castling castling, Color color) noexcept;
    inline void DisallowCastling(Castling castling, Color color) noexcept;
    inline void SetEP(Column column) noexcept;
    inline void PlacePiece(Square square, PieceType pType, Color color) noexcept;
    inline void RemovePiece(Square square, PieceType pType, Color color) noexcept;

private:
    uint64_t _hash = 0;
    BB _pieceBB[PIECECOUNT]{0};
    BB _colorBB[COLORCOUNT]{0};
    /*
     * 1 bit - [1]:   Turn
     * 4 bit - [2-5]: Castling rights
     * 4 bit - [6,9]: EP
     * column)
     */
    uint16_t _misc = 0;
};

inline uint64_t Position::GetHash() const noexcept { return _hash; }
inline Color Position::GetTurn() const noexcept { return (Color)(_misc & 0x1); }
inline Column Position::GetEP() const noexcept { return COLUMNS[(_misc >> 5) & 0xf]; }
inline Castling Position::GetCastling(Color color) const noexcept {
    return (Castling) (((_misc >> 1) & (0x3 << (2 * (int)color))) >> (2 * (int)color));
}
inline bool Position::AllowsCastling(Castling castling, Color color) const noexcept {
    return (int)GetCastling(color) & (int)castling;
}
inline PieceType Position::GetType(Square square) const noexcept {
    assert(square != Square::None);
    for (const auto pType : PIECES)
        if (GetPieces(pType) & square)
            return pType;
    return PieceType::None;
}
inline Color Position::GetColor(Square square) const noexcept {
    assert(square != Square::None);
    if (square & _colorBB[(int)Color::White])
        return Color::White;
    else if (square & _colorBB[(int)Color::Black])
        return Color::Black;
    else
        return Color::None;
}
inline int Position::GetPieceCount(Color color, PieceType pType) const noexcept {
    assert(color != Color::None);
    assert(pType != PieceType::None);
    return jank::bit::popcount(GetPieces(color, pType));
}
inline BB Position::GetPieces() const noexcept { return _colorBB[0] | _colorBB[1]; }
inline BB Position::GetPieces(PieceType pType) const noexcept {
    assert(pType != PieceType::None);
    return _pieceBB[(int)pType];
}
inline BB Position::GetPieces(Color color) const noexcept {
    assert(color != Color::None);
    return _colorBB[(int)color];
}
inline BB Position::GetPieces(Color color, PieceType pType) const noexcept {
    assert(color != Color::None);
    assert(pType != PieceType::None);
    return _colorBB[(int)color] & _pieceBB[(int)pType];
}
inline void Position::SetTurn(Color color) noexcept {
    _misc &= ~0x1;
    _misc |= (int)color & 0x1;
}
inline void Position::SetCastling(Castling castling, Color color) noexcept {
    _misc &= ~((0x3 << (2 * (int)color)) << 1);
    _misc |= ((int)castling << (2 * (int)color)) << 1;
}
inline void Position::DisallowCastling(Castling castling, Color color) noexcept {
    const Castling priorCastling = GetCastling(color);
    SetCastling((Castling)((int)priorCastling & ~((int)castling)), color);
}
inline void Position::SetEP(Column column) noexcept {
    const int index = Utilities::GetColumnIndex(column);
    _misc &= ~0x1e0;
    _misc |= (index & 0xf) << 5;
}

inline bool Position::IsKingSafe(BB tempOccuracyBoard, BB tempEnemyBoard) const noexcept {
    return IsKingSafe(tempOccuracyBoard, tempEnemyBoard, GetPieces(GetTurn(), PieceType::King));
}
inline bool Position::IsKingSafe(BB tempOccuracyBoard) const noexcept {
    return IsKingSafe(tempOccuracyBoard, GetPieces(Utilities::GetOppositeColor(GetTurn())));
}
inline bool Position::IsKingSafe() const noexcept { return IsKingSafe(GetPieces()); }
inline void Position::PlacePiece(Square square, PieceType pType, Color color) noexcept {
    assert(square != Square::None);
    assert(pType != PieceType::None);
    assert(color != Color::None);
    _pieceBB[(int)pType] |= square;
    _colorBB[(int)color] |= square;
    _hash = Zobrist::FlipSquare(_hash, square, pType, color);
}
inline void Position::RemovePiece(Square square, PieceType pType, Color color) noexcept {
    assert(square != Square::None);
    assert(pType != PieceType::None);
    assert(color != Color::None);
    _pieceBB[(int)pType] ^= square;
    _colorBB[(int)color] ^= square;
    _hash = Zobrist::FlipSquare(_hash, square, pType, color);
}
} // namespace Chess

#endif
