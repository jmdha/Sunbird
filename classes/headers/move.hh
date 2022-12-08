#ifndef MOVE
#define MOVE

#include <string>

#include "constants.hh"
#include "utilities.hh"

class Move {
public:
    Move() {};
    Move(MoveType type) : move(0)/* , type(type), capturedPiece(PieceType::None) */ {
        SetType(type);
    };

    Move(MoveType type, Square from, Square to) : move(0)/* , type(type), from(from), to(to), capturedPiece(PieceType::None) */ {
        SetType(type);
        SetFrom(from);
        SetTo(to);
    };

    Move(MoveType type, Square from, Square to, PieceType capturedPiece) : move(0)/* , type(type), from(from), to(to), capturedPiece(capturedPiece) */ {
        SetType(type);
        SetFrom(from);
        SetTo(to);
        SetCapture(capturedPiece);
    };

    inline std::string ToString() const;
    // Properties
    inline MoveType GetType() const;
    inline Square GetFrom() const;
    inline Square GetTo() const;
    inline PieceType GetCapturedPiece() const;
    inline bool IsCapture() const;
    inline bool IsPromotion() const;
    inline bool IsEP() const;
    inline bool IsDC() const;
    inline bool IsDC(Color color, Castling side) const;
    inline void SetDisableCastle(Color color, Castling side);
    

private:
    uint32_t move;
    // These are for debug information
/*     MoveType type;
    Square from;
    Square to;
    PieceType capturedPiece; */

    inline void SetType(MoveType type);
    inline void SetFrom(Square square);
    inline void SetTo(Square square);
    inline void SetCapture(PieceType capturedType);
};

inline std::string Move::ToString() const {
    std::string move = "";        
    move += Utilities::GetSquareString(GetFrom());
    move += Utilities::GetSquareString(GetTo());
    return move;
}

inline MoveType Move::GetType() const {
    return (MoveType) (move & 0xf);
}

inline Square Move::GetFrom() const {
    return (Square) ((move >> 4) & 0x3f);
}

inline Square Move::GetTo() const {
    return (Square) ((move >> 12) & 0x3f);
}

inline PieceType Move::GetCapturedPiece() const {
    return (PieceType) ((move >> 20) & 0xf);
}

inline bool Move::IsCapture() const {
    return ((U8) GetType() & CaptureBit) != 0;
}

inline bool Move::IsPromotion() const {
    return ((U8) GetType() & PromotionBit) != 0;
}

inline bool Move::IsEP() const {
    return GetType() == MoveType::EPCapture;
}

inline bool Move::IsDC() const {
    return ((move >> 24) & 0xf) != 0;
}

inline bool Move::IsDC(Color color, Castling side) const {
    return ((move >> 24) & (U8) std::pow(2, (U8) (2 * (U8) color + (U8) side))) != 0;
}

inline void Move::SetType(MoveType type) {
    move |= ((U8)type & 0xf);
}

inline void Move::SetFrom(Square square) {
    move |= (((U8)square & 0x3f)) << 4;
}

inline void Move::SetTo(Square square) {
    move |= (((U8)square & 0x3f)) << 12;
}

inline void Move::SetCapture(PieceType capturedType) {
    move |= (((U8)capturedType & 0xf)) << 20;
}

inline void Move::SetDisableCastle(Color color, Castling side) {
    move |= (U8) std::pow(2, (U8) (2 * (U8) color + (U8) side)) << 24;
}

#endif // MOVE
