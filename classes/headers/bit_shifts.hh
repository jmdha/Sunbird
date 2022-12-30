#ifndef BIT_SHIFTS
#define BIT_SHIFTS

#include <array>
#include <algorithm>

#include "constants.hh"

class BitShifts {
public:
    static void Init();

    static inline U64 GetRay(Square square, DirectionIndex direction);
    static inline U64 GetRay(U64 square, DirectionIndex direction);
    static inline U64 Shift(U64 b, Direction dir, int times);
private:
    inline static U64 rays[SQUARECOUNT][DIRECTIONCOUNT];
    inline static U64 pieceAttacks[PIECECOUNT][SQUARECOUNT];
    inline static U64 blockersAndBeyond[PIECECOUNT][SQUARECOUNT];
    inline static U64 behind[SQUARECOUNT][SQUARECOUNT];

    inline static U64 GenerateRay(U64 square, DirectionIndex direction);
};

inline U64 BitShifts::GetRay(Square square, DirectionIndex direction) {
    return rays[(int) square][(int) direction];
};

inline U64 BitShifts::GetRay(U64 square, DirectionIndex direction) {
    return rays[square][(int) direction];
};

inline U64 BitShifts::Shift(U64 b, Direction dir, int times) {
    if ((int) dir > 0)
        return b << (int) dir * times;
    else
        return b >> std::abs((int) dir) * times;
}

U64 BitShifts::GenerateRay(U64 square, DirectionIndex direction) {
    square = C64(square);
    U64 ray = 0;
    while (square) {
        square = Shift(square, directions[(int) direction], 1);
        ray |= square;
    }
    return ray;
}

#endif // BIT_SHIFTS
