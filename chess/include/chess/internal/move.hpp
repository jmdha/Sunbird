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
    explicit Move(uint16_t move) : move(move) {}
    explicit Move(MoveType type) : move((U8)type & 0xf){};

    Move(MoveType type, Square from, Square to)
        : move(((U8)type & 0xf) + (((U8)from & 0x3f) << 4) + (((U8)to & 0x3f) << 10)){};

    inline std::string ToString() const;

    inline uint16_t GetValue() const noexcept { return move; }
    inline MoveType GetType() const noexcept { return (MoveType)(move & 0xf); }
    inline Square GetFrom() const noexcept { return (Square)((move >> 4) & 0x3f); }
    inline Square GetTo() const noexcept { return (Square)((move >> 10) & 0x3f); }
    inline bool IsCapture() const noexcept { return move & CaptureBit; }
    inline bool IsPromotion() const noexcept { return move & PromotionBit; }
    inline bool IsCastle() const noexcept { return move & CastlingBit; }
    inline bool IsEP() const noexcept { return GetType() == MoveType::EPCapture; }

    friend bool operator==(Move const &lhs, Move const &rhs) { return lhs.move == rhs.move; }

private:
    uint16_t move = 0;
};

inline std::string Move::ToString() const {
    std::string tempMove;
    tempMove += Utilities::GetSquareString(GetFrom());
    tempMove += Utilities::GetSquareString(GetTo());
    if (IsPromotion()) {
        if (GetType() == MoveType::QPromotion || GetType() == MoveType::QPromotionCapture)
            tempMove += "q";
        else if (GetType() == MoveType::RPromotion || GetType() == MoveType::RPromotionCapture)
            tempMove += "r";
        else if (GetType() == MoveType::BPromotion || GetType() == MoveType::BPromotionCapture)
            tempMove += "b";
        else if (GetType() == MoveType::NPromotion || GetType() == MoveType::NPromotionCapture)
            tempMove += "n";
    }
    return tempMove;
}
} // namespace Chess

#endif // MOVE
