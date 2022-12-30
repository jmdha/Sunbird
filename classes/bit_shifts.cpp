#include "headers/bit_shifts.hh"

void BitShifts::Init () {
    for (U64 square = 0; square < SQUARECOUNT; square++)
        for (int dir = 0; dir < DIRECTIONCOUNT; dir++)
            rays[square][dir] = GenerateRay(square, (DirectionIndex) dir);
    for (U8 from = 0; from < SQUARECOUNT; from++)
        for (U8 to = 0; to < SQUARECOUNT; to++)
            if (from != to)
                behind[from][to] = GenerateBehind(from, to);
}
