#ifndef MOVE
#define MOVE

#include <string>

#include "constants.hh"
#include "utilities.hh"

class Move {
public:
    Move(MoveType type) : type(type) {};
    Move(MoveType type, Square from, Square to) : type(type), from(from), to(to) {};
    inline std::string ToString() const;
    // Properties
    inline MoveType GetType() const;
    inline Square GetFrom() const;
    inline Square GetTo() const;
    inline bool IsCapture() const;
    inline bool IsPromotion() const;
    inline bool IsEP() const;

private:
    MoveType type;
    Square from;
    Square to;
};

inline std::string Move::ToString() const {
    if (type == MoveType::KingCastle)
            return "O-O";
    else if (type == MoveType::QueenCastle)
            return "O-O-O";
    std::string move = "";        
    move += Utilities::GetSquareString(from);
    move += Utilities::GetSquareString(to);
    return move;
}

inline MoveType Move::GetType() const {
    return type;
}

inline Square Move::GetFrom() const {
    return from;
}

inline Square Move::GetTo() const {
    return to;
}

inline bool Move::IsCapture() const {
    return ((unsigned short) type & (unsigned short) CaptureBit) != 0;
}

inline bool Move::IsPromotion() const {
    return ((unsigned short) type & (unsigned short) PromotionBit) != 0;
}

inline bool Move::IsEP() const {
    return type == MoveType::EPCapture;
}

#endif
