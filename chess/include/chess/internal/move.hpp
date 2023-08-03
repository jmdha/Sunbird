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
    explicit Move(uint32_t move) : move(move) {}
    explicit Move(MoveType type) : move(0) { SetType(type); };

    Move(MoveType type, Square from, Square to) : move(0) {
        SetType(type);
        SetFrom(from);
        SetTo(to);
        assert(move != 0);
    };

    inline std::string ToString() const;
    // Properties
    inline uint32_t GetValue() const { return move; };
    inline MoveType GetType() const;
    inline Square GetFrom() const;
    inline Square GetTo() const;
    inline Column GetDEP() const;
    inline bool IsCapture() const;
    inline bool IsPromotion() const;
    inline bool IsCastle() const;
    inline bool IsEP() const;
    inline bool IsDEP() const;
    inline bool IsDC() const;
    inline bool IsDC(Color color, Castling side) const;
    inline bool IsDefined() const { return move != 0; };
    inline void SetDisableCastle(Color color, Castling side);

    friend bool operator==(Move const &lhs, Move const &rhs) { return lhs.move == rhs.move; }

private:
    uint32_t move = 0;

    inline void SetType(MoveType type);
    inline void SetFrom(Square square);
    inline void SetTo(Square square);
    inline void SetCapture(PieceType capturedType);
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

inline MoveType Move::GetType() const { return (MoveType)(move & 0xf); }

inline Square Move::GetFrom() const { return (Square)((move >> 4) & 0x3f); }

inline Square Move::GetTo() const { return (Square)((move >> 12) & 0x3f); }

inline Column Move::GetDEP() const {
    return (Column)Utilities::GetColumn(((move >> 28) & 0xf) - 1);
}

inline bool Move::IsCapture() const { return ((U8)GetType() & CaptureBit) != 0; }

inline bool Move::IsPromotion() const { return ((U8)GetType() & PromotionBit) != 0; }

inline bool Move::IsCastle() const { return ((U8)GetType() & CastlingBit) != 0 && !IsPromotion(); }

inline bool Move::IsEP() const { return GetType() == MoveType::EPCapture; }

inline bool Move::IsDEP() const { return ((move >> 28) & 0xf) != 0; }

inline bool Move::IsDC() const { return ((move >> 24) & 0xf) != 0; }

inline bool Move::IsDC(Color color, Castling side) const {
    return ((move >> 24) & (U8)std::pow(2, (U8)(2 * (U8)color + (U8)side))) != 0;
}

inline void Move::SetType(MoveType type) { move |= ((U8)type & 0xf); }

inline void Move::SetFrom(Square square) { move |= (((U8)square & 0x3f)) << 4; }

inline void Move::SetTo(Square square) { move |= ((U8)square & 0x3f) << 12; }

inline void Move::SetCapture(PieceType capturedType) { move |= (((U8)capturedType & 0xf)) << 20; }

inline void Move::SetDisableCastle(Color color, Castling side) {
    move |= (U8)std::pow(2, (U8)(2 * (U8)color + (U8)side)) << 24;
}
} // namespace Chess

#endif // MOVE
