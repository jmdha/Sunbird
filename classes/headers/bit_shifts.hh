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
    inline static U64 rays[64][8];
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

#endif // BIT_SHIFTS
