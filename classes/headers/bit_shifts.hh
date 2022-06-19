#ifndef BITSHIFTS
#define BITSHIFTS

#include "constants.hh"

class BitShifts {
public:
#pragma region SingleStep
    static U64 NOne (U64 b) { return b << 8; };
    static U64 SOne (U64 b) { return b >> 8; };
    static U64 EOne (U64 b) { return b << 1; };
    static U64 WOne (U64 b) { return b >> 1; };

    static U64 NEOne (U64 b) { return b << 9; };
    static U64 NWOne (U64 b) { return b << 7; };
    static U64 SEOne (U64 b) { return b >> 7; };
    static U64 SWOne (U64 b) { return b >> 9; };
#pragma endregion
#pragma region DoubleStep
    static U64 NTwo (U64 b) { return b << 16; };
    static U64 STwo (U64 b) { return b >> 16; };
#pragma endregion
private:
};
#endif