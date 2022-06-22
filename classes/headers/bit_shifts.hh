#ifndef BITSHIFTS
#define BITSHIFTS

#include "constants.hh"

class BitShifts {
public:
    static U64 Shift (U64 b, Direction dir, int times) { 
        if ((int) dir > 0)
            return b << (int) dir * times;
        else
            return b >> std::abs((int) dir) * times;
    }; 
private:
};
#endif