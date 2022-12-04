#ifndef MOVE
#define MOVE

#include <string>

#include "constants.hh"
#include "utilities.hh"

class Move {
public:
    Move(MoveType type) : move(0) {
        SetType(type);
    };

    Move(MoveType type, Square from, Square to) : move(0) {
        SetType(type);
        SetFrom(from);
        SetTo(to);
    };

    inline std::string ToString() const;
    // Properties
    inline MoveType GetType() const;
    inline Square GetFrom() const;
    inline Square GetTo() const;
    inline bool IsCapture() const;
    inline bool IsPromotion() const;
    inline bool IsEP() const;

private:
    // 0  - 3  bit is the movetype, see constants.h for movetypes
    // 4 - 12 bit is from square
    // 13 - 21 bit is to square
    // ?  - ?  bit is captured piece type
    // ?  - ? bit is disabled castling
    U64 move;

    inline void SetType(MoveType type);
    inline void SetFrom(Square square);
    inline void SetTo(Square square);
};

inline std::string Move::ToString() const {
    if (GetType() == MoveType::KingCastle)
            return "O-O";
    else if (GetType() == MoveType::QueenCastle)
            return "O-O-O";
    std::string move = "";        
    move += Utilities::GetSquareString(GetFrom());
    move += Utilities::GetSquareString(GetTo());
    return move;
}

inline MoveType Move::GetType() const {
    return (MoveType) (move & (U64) 0xf);
}

inline Square Move::GetFrom() const {
    return (Square) ((move >> 4) & (U64) 0x3f);
}

inline Square Move::GetTo() const {
    return (Square) ((move >> 12) & (U64) 0x3f);
}

inline bool Move::IsCapture() const {
    return ((U64) GetType() & (U64) CaptureBit) != 0;
}

inline bool Move::IsPromotion() const {
    return ((U64) GetType() & (U64) PromotionBit) != 0;
}

inline bool Move::IsEP() const {
    return GetType() == MoveType::EPCapture;
}

inline void Move::SetType(MoveType type) {
    move &= (U64) ~0xf;
    move |= (U64) type;
}

inline void Move::SetFrom(Square square) {
    move &= ((U64) ~0x3f) << 4;
    move |= ((U64) square) << 4;
}

inline void Move::SetTo(Square square) {
    move &= ((U64) ~0x3f) << 12;
    move |= ((U64) square) << 12;
}

#endif // MOVE
