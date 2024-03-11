#pragma once

#include "types.hpp"

struct Position {
public:
    // access

    uint64_t GetHash() const noexcept;
    Color GetTurn() const noexcept;
    Square GetEP() const noexcept;
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
    void SetEP(Square sq) noexcept;
    void PlacePiece(Square square, Piece pType, Color color) noexcept;
    void RemovePiece(Square square, Piece pType, Color color) noexcept;

private:
    uint64_t _hash = 0;
    BB _pieceBB[PIECE_COUNT]{0};
    BB _colorBB[COLOR_COUNT]{0};
    // A move is encoded in 16 bits
    //
    //  1 bit: Turn
    //  4 bits: Castling rights
    //  6 bits: EP square
    //
    // Where Move=0 is an undefined move
    uint16_t _misc = 30;
};
