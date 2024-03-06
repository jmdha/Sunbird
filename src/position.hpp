#pragma once

#include "types.hpp"

struct Position {
public:
    // access

    uint64_t GetHash() const noexcept;
    Color GetTurn() const noexcept;
    Column GetEP() const noexcept;
    Castling GetCastling(Color color) const noexcept;
    bool AllowsCastling(Castling castling, Color color) const noexcept;
    Piece GetType(Square square) const noexcept;
    Color GetColor(Square square) const noexcept;
    int GetPieceCount(Color color, Piece pType) const noexcept;
    BB GetPieces() const noexcept;
    BB GetPieces(Piece pType) const noexcept;
    BB GetPieces(Color color) const noexcept;
    BB GetPieces(Color color, Piece pType) const noexcept;
    Square GetKing(Color color) const noexcept;
    bool IsKingSafe(Color turn) const noexcept;
    BB GenerateAttackSquares(Color color) const noexcept;

    // modifiers

    void SetTurn(Color color) noexcept;
    void DisallowCastling(Castling castling, Color color) noexcept;
    void SetCastling(Castling castling, Color color) noexcept;
    void SetEP(Column column) noexcept;
    void PlacePiece(Square square, Piece pType, Color color) noexcept;
    void RemovePiece(Square square, Piece pType, Color color) noexcept;

private:
    uint64_t _hash = 0;
    BB _pieceBB[PIECE_COUNT]{0};
    BB _colorBB[COLOR_COUNT]{0};
    /*
     * 1 bit - [1]:   Turn
     * 4 bit - [2-5]: Castling rights
     * 4 bit - [6,9]: EP
     * column)
     */
    uint16_t _misc = 30;
};
