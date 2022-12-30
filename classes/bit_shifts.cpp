#include "headers/bit_shifts.hh"

void BitShifts::Init () {
    for (U64 square = 0; square < SQUARECOUNT; square++)
        for (int dir = 0; dir < DIRECTIONCOUNT; dir++)
            rays[square][dir] = GenerateRay(square, (DirectionIndex) dir);
}
