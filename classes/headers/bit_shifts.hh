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
    static U64 Ray (Square square, Direction dir) {
        switch (dir)
        {
        case Direction::North:
            return (U64) Ray::North >> (int) square;
        case Direction::East:
            return (U64) Ray::East >> (int) square;
        case Direction::South:
            if ((int) square > (int) Square::A8)
                return (U64) Ray::South >> (int) square - (int) Square::A8;
            else
                return (U64) Ray::South << (int) square - (int) Square::A8;
        case Direction::West:
            if ((int) square > (int) Square::H1)
                return (U64) Ray::West >> (int) square - (int) Square::H1;
            else
                return (U64) Ray::West << (int) square - (int) Square::H1;
        }
        
        return 0;
    };
private:
};
#endif
