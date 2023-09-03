#ifndef CHESS_POSITION
#define CHESS_POSITION

#include "bit.hpp"
#include "bitboard.hpp"
#include "constants.hpp"
#include "move.hpp"
#include "utilities.hpp"
#include "zobrist.hpp"

namespace Chess {
struct Position {
public:
    // access

    inline uint64_t GetHash() const;
    inline Color GetTurn() const;
    inline Column GetEP() const;
    inline Castling GetCastling(Color color) const;
    inline bool AllowsCastling(Castling castling, Color color) const;

    inline PieceType GetType(Square square) const;
    inline Color GetColor(Square square) const;
    inline int GetPieceCount(Color color, PieceType pType) const;
    inline BB GetPieces() const;
    inline BB GetPieces(PieceType pType) const;
    inline BB GetPieces(Color color) const;
    inline BB GetPieces(Color color, PieceType pType) const;
    inline Square GetKing(Color color) const;

    bool IsKingSafe(BB tempOccuracyBoard, BB tempEnemyBoard, BB tempKingBoard) const;
    inline bool IsKingSafe(BB tempOccuracyBoard, BB tempEnemyBoard) const;
    inline bool IsKingSafe(BB tempOccuracyBoard) const;
    inline bool IsKingSafe() const;

    BB GenerateAttackSquares(Color color) const;

    // modifiers

    inline void SetTurn(Color color);
    inline void DisallowCastling(Castling castling, Color color);
    inline void SetEP(Column column);
    inline void PlacePiece(Square square, PieceType pType, Color color);
    inline void RemovePiece(Square square, PieceType pType, Color color);

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
    uint16_t _misc = 30;
    inline void SetCastling(Castling castling, Color color);
};

inline uint64_t Position::GetHash() const { return _hash; }
inline Color Position::GetTurn() const { return static_cast<Color>(_misc & 0x1); }
inline Column Position::GetEP() const {
    const size_t index = (_misc >> 5) & 0xf;
    if (index == 0)
        return Column::None;
    else
        return COLUMNS[index - 1];
}
inline Castling Position::GetCastling(Color color) const {
    return static_cast<Castling>(((_misc >> 1) & (0x3 << (2 * static_cast<uint16_t>(color)))) >>
                                 (2 * static_cast<uint16_t>(color)));
}
inline bool Position::AllowsCastling(Castling castling, Color color) const {
    return static_cast<int>(GetCastling(color) & castling) != 0;
}
inline PieceType Position::GetType(Square square) const {
    assert(square != Square::None);
    for (const auto pType : PIECES)
        if (GetPieces(pType) & square)
            return pType;
    return PieceType::None;
}
inline Color Position::GetColor(Square square) const {
    assert(square != Square::None);
    if (square & _colorBB[static_cast<size_t>(Color::White)])
        return Color::White;
    else if (square & _colorBB[static_cast<size_t>(Color::Black)])
        return Color::Black;
    else
        return Color::None;
}
inline int Position::GetPieceCount(Color color, PieceType pType) const {
    assert(color != Color::None);
    assert(pType != PieceType::None);
    return Bit::popcount(GetPieces(color, pType));
}
inline BB Position::GetPieces() const { return _colorBB[0] | _colorBB[1]; }
inline BB Position::GetPieces(PieceType pType) const {
    assert(pType != PieceType::None);
    return _pieceBB[static_cast<size_t>(pType)];
}
inline BB Position::GetPieces(Color color) const {
    assert(color != Color::None);
    return _colorBB[static_cast<size_t>(color)];
}
inline BB Position::GetPieces(Color color, PieceType pType) const {
    assert(color != Color::None);
    assert(pType != PieceType::None);
    return _colorBB[static_cast<size_t>(color)] & _pieceBB[static_cast<size_t>(pType)];
}
inline Square Position::GetKing(Color color) const {
    assert(color != Color::None);
    return static_cast<Square>(Bit::lsb(GetPieces(color, PieceType::King)));
}
inline void Position::SetTurn(Color color) {
    if (color != GetTurn())
        _hash = Zobrist::FlipColor(_hash);
    _misc &= ~0x1;
    _misc |= static_cast<int>(color) & 0x1;
}
inline void Position::SetCastling(Castling castling, Color color) {
    _hash = Zobrist::FlipCastling(_hash, color, castling);
    _misc &= ~((0x3 << (2 * static_cast<uint16_t>(color))) << 1);
    _misc |= (static_cast<uint16_t>(castling) << (2 * static_cast<uint16_t>(color))) << 1;
}
inline void Position::DisallowCastling(Castling castling, Color color) {
    const Castling priorCastling = GetCastling(color);
    SetCastling(priorCastling & ~castling, color);
    if (priorCastling != GetCastling(color))
        _hash = Zobrist::FlipCastling(_hash, color, castling);
}
inline void Position::SetEP(Column column) {
    if (auto EP = GetEP(); column != EP)
        _hash = Zobrist::FlipEnPassant(_hash, EP);
    _misc &= static_cast<uint16_t>(~0x1e0);
    if (column == Column::None) [[likely]]
        return;
    const int index = Bit::lsb(static_cast<BB>(column)) + 1;
    _misc |= (index & 0xf) << 5;
    _hash = Zobrist::FlipEnPassant(_hash, column);
}

inline bool Position::IsKingSafe(BB tempOccuracyBoard, BB tempEnemyBoard) const {
    return IsKingSafe(tempOccuracyBoard, tempEnemyBoard, GetPieces(GetTurn(), PieceType::King));
}
inline bool Position::IsKingSafe(BB tempOccuracyBoard) const {
    return IsKingSafe(tempOccuracyBoard, GetPieces(~GetTurn()));
}
inline bool Position::IsKingSafe() const { return IsKingSafe(GetPieces()); }
inline void Position::PlacePiece(Square square, PieceType pType, Color color) {
    assert(square != Square::None);
    assert(pType != PieceType::None);
    assert(color != Color::None);
    _pieceBB[static_cast<size_t>(pType)] |= square;
    _colorBB[static_cast<size_t>(color)] |= square;
    _hash = Zobrist::FlipSquare(_hash, square, pType, color);
}
inline void Position::RemovePiece(Square square, PieceType pType, Color color) {
    assert(square != Square::None);
    assert(pType != PieceType::None);
    assert(color != Color::None);
    _pieceBB[static_cast<size_t>(pType)] ^= square;
    _colorBB[static_cast<size_t>(color)] ^= square;
    _hash = Zobrist::FlipSquare(_hash, square, pType, color);
}
} // namespace Chess

#endif
