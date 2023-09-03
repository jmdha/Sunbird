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
    explicit Move(uint16_t move) : _move(move) {}
    explicit Move(MoveType type) : _move(static_cast<uint16_t>(type) & 0xf) {}

    Move(MoveType type, Square from, Square to)
        : _move(static_cast<uint16_t>(static_cast<uint16_t>(type) & 0xf) +
                static_cast<uint16_t>((static_cast<uint16_t>(from) & 0x3f) << 4) +
                static_cast<uint16_t>((static_cast<uint16_t>(to) & 0x3f) << 10)) {}

    inline std::string ToString() const;

    inline uint16_t GetValue() const noexcept { return _move; }
    inline MoveType GetType() const noexcept { return static_cast<MoveType>(_move & 0xf); }
    inline Square GetFrom() const noexcept { return static_cast<Square>((_move >> 4) & 0x3f); }
    inline Square GetTo() const noexcept { return static_cast<Square>((_move >> 10) & 0x3f); }
    inline bool IsCapture() const noexcept { return _move & CaptureBit; }
    inline bool IsPromotion() const noexcept { return _move & PromotionBit; }
    inline bool IsTactical() const noexcept { return IsCapture() || IsPromotion(); }
    inline bool IsCastle() const noexcept { return _move & CastlingBit; }
    inline bool IsEP() const noexcept { return GetType() == MoveType::EPCapture; }

    friend bool operator==(Move const &lhs, Move const &rhs) { return lhs._move == rhs._move; }

private:
    uint16_t _move = 0;
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
