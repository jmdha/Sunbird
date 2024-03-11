#pragma once

#include "types.hpp"
#include <cstdint>

// A chess move. Includes information regarding origin square, destination square and type of move.
// However, does not include the type of piece moved, nor captured.
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

    Move() = default;
    Move(Square origin, Square destination, Type type);
    // Creates a move from a string in smith notation
    // Requires occupancy mask to check for captures
    Move(BB occ, BB kings, BB pawns, const std::string &move);

    Square Origin() const;
    Square Destination() const;

    Type GetType() const;
    bool IsDefined() const;
    bool IsCapture() const;
    bool IsPromotion() const;
    bool IsEnPassant() const;
    bool IsDouble() const;
    bool IsKingCastle() const;
    bool IsQueenCastle() const;
    bool IsCastle() const;
    Piece PromotionPiece() const;

    std::string Export() const;

    bool operator==(const Move &move) const;

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
