#ifndef CHESS_MOVE
#define CHESS_MOVE

#include <cassert>
#include <string>

#include "constants.hpp"
#include "utilities.hpp"

namespace Chess {
class Move {
public:
    Move() = default;
    explicit Move(MoveType type) : _type((U8)type){};

    Move(MoveType type, Square from, Square to) : _type((U8)type), _from((U8)from), _to((U8)to){};

    inline std::string ToString() const;
    // Properties
    inline uint32_t GetValue() const {
        return _type.to_ulong() + (_from.to_ulong() << 4) + (_to.to_ulong() << 12);
    };
    inline MoveType GetType() const { return (MoveType)_type.to_ulong(); }
    inline Square GetFrom() const { return (Square)_from.to_ulong(); }
    inline Square GetTo() const { return (Square)_to.to_ulong(); }
    inline bool IsCapture() const { return (_type.to_ulong() & CaptureBit) != 0; }
    inline bool IsPromotion() const { return (_type.to_ulong() & PromotionBit) != 0; }
    inline bool IsCastle() const { return (_type.to_ulong() & CastlingBit) != 0; }
    inline bool IsEP() const { return (MoveType)_type.to_ulong() == MoveType::EPCapture; }

    friend bool operator==(Move const &lhs, Move const &rhs) {
        return lhs._type == rhs._type && lhs._from == rhs._from && lhs._to == rhs._to;
    }

private:
    std::bitset<4> _type;
    std::bitset<6> _from;
    std::bitset<6> _to;
};

inline std::string Move::ToString() const {
    std::string tempMove;
    tempMove += Utilities::GetSquareString(GetFrom());
    tempMove += Utilities::GetSquareString(GetTo());
    if (IsPromotion()) {
        if (GetType() == MoveType::RPromotion)
            tempMove += "r";
        else if (GetType() == MoveType::BPromotion)
            tempMove += "b";
        else if (GetType() == MoveType::NPromotion)
            tempMove += "n";
    }
    return tempMove;
}
} // namespace Chess

#endif // MOVE
