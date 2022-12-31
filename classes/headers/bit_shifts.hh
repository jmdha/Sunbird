#ifndef BIT_SHIFTS
#define BIT_SHIFTS

#include <array>
#include <algorithm>

#include "constants.hh"
#include "utilities.hh"

class BitShifts {
public:
    static void Init();

    static inline U64 GetRay(Square square, DirectionIndex direction);
    static inline U64 GetRay(U64 square, DirectionIndex direction);
    static inline U64 GetRay(U64 square, Direction direction);
    static inline U64 GetBehind(U64 from, U64 to);
    static inline U64 GetAttacks(U64 square, PieceType pieceType);
    static inline U64 GetBB(U8 sq, PieceType type);
    static inline U64 Shift(U64 b, Direction dir, int times);
private:
    inline static U64 rays[SQUARECOUNT][DIRECTIONCOUNT] = { 0 };
    inline static U64 pieceAttacks[PIECECOUNT][SQUARECOUNT] = { 0 };
    inline static U64 blockersAndBeyond[PIECECOUNT][SQUARECOUNT] = { 0 };
    inline static U64 behind[SQUARECOUNT][SQUARECOUNT] = { 0 };

    inline static U64 GenerateRay(U8 square, DirectionIndex direction);
    inline static U64 GenerateBehind(U8 from, U8 to);
    inline static U64 GenerateBB(U8 sq, PieceType type);
};

inline U64 BitShifts::GetRay(const Square square, DirectionIndex direction) {
    return rays[(int) square][(int) direction];
};

inline U64 BitShifts::GetRay(const U64 square, DirectionIndex direction) {
    return rays[square][(int) direction];
};

inline U64 BitShifts::GetRay(const U64 square, Direction direction) {
    return GetRay(square, Utilities::GetDirectionIndex(direction));
};

inline U64 BitShifts::GetBehind(U64 from, U64 to) {
    return behind[from][to];
}

inline U64 BitShifts::Shift(U64 b, Direction dir, int times) {
    if ((int) dir > 0)
        return b << (int) dir * times;
    else
        return b >> std::abs((int) dir) * times;
}

U64 BitShifts::GenerateRay(U8 square, DirectionIndex direction) {
    U64 bit = C64((U64) square);
    U64 ray = 0;
    while (bit & (U64) notEdges[(int) direction]) {
        bit = Shift(bit, directions[(int) direction], 1);
        ray |= bit;
    }
    return ray;
}

// Uses rays
U64 BitShifts::GenerateBehind(U8 from, U8 to) {
    return rays[to][(int) Utilities::GetDirectionIndex((Square) from, (Square) to)];
}

U64 BitShifts::GetAttacks(unsigned long long int square, PieceType pieceType) {
    return pieceAttacks[(int) pieceType][square];
}

U64 BitShifts::GenerateBB(unsigned short sq, PieceType type) {
    return GetAttacks(sq, type) ^ EDGE;
}

U64 BitShifts::GetBB(unsigned short sq, PieceType type) {
    return blockersAndBeyond[(int) type][sq];
}

#endif // BIT_SHIFTS
