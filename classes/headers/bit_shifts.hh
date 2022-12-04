#ifndef BIT_SHIFTS
#define BIT_SHIFTS

#include <array>

#include "constants.hh"

class BitShifts {
public:
    inline static U64 rays[64][8];

    static void Init();
    static inline U64 Shift(U64 b, Direction dir, int times);
private:
};

inline U64 BitShifts::Shift(U64 b, Direction dir, int times) { 
    if ((int) dir > 0)
        return b << (int) dir * times;
    else
        return b >> std::abs((int) dir) * times;
}; 

#endif // BIT_SHIFTS
