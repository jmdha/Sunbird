#ifndef BITSHIFTS
#define BITSHIFTS

#include "constants.hh"

class BitShifts {
public:
    static U64 Shift (U64 b, Direction dir, int times) { return b << (int) dir * times; }; 
private:
};
#endif