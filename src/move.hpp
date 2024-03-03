#pragma once

#include "types.hpp"
#include <string>

// A chess move. Includes information regarding origin square, destination square and type of move.
class Move {
public:
    enum Type {
        Quiet             = 0,
        DoublePawnPush    = 1,
        KingCastle        = 2,
        QueenCastle       = 3,
        Capture           = 4,
        EPCapture         = 5,
        NPromotion        = 8,
        BPromotion        = 9,
        RPromotion        = 10,
        QPromotion        = 11,
        NPromotionCapture = 12,
        BPromotionCapture = 13,
        RPromotionCapture = 14,
        QPromotionCapture = 15
    };

    Move() noexcept = default;
    Move(Square origin, Square destination, Type type) noexcept;
    // Creates a move from a string in smith notation
    Move(BB occ, BB kings, BB pawns, const std::string &move) noexcept;

    Square Origin() const noexcept;
    Square Destination() const noexcept;
    Type GetType() const noexcept;
    bool IsDefined() const noexcept;
    bool IsCapture() const noexcept;
    bool IsPromotion() const noexcept;
    bool IsEnPassant() const noexcept;
    bool IsDouble() const noexcept;
    bool IsKingCastle() const noexcept;
    bool IsQueenCastle() const noexcept;
    bool IsCastle() const noexcept;
    Piece PromotionPiece() const noexcept;
    std::string Export() const noexcept;
    bool operator==(const Move &move) const noexcept;

private:
    // A move is encoded in 16 bits
    //
    //  6 bits: origin square
    //  6 bits: destination square
    //  4 bits: move type
    //
    // Where Move=0 is an undefined move
    uint16_t internal;
};
