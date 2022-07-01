#ifndef BITSHIFTS
#define BITSHIFTS

#include <array>

#include "constants.hh"

class BitShifts {
public:
    inline static U64 rays[64][8];

    static void Init();
    static U64 Shift (U64 b, Direction dir, int times);
private:
};
#endif
