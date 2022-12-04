#include "headers/bit_shifts.hh"

void BitShifts::Init () {
    for (U8 sq = 0; sq < 64; sq++ ){
        rays[sq][0] = 0;

        U64 bit = C64(sq);
        while (bit & (U64) NotEdge::North) {
            bit = Shift(bit, Direction::North, 1);
            rays[sq][(U8) DirectionIndex::North] |= bit;
        }

        bit = C64(sq);
        while (bit & (U64) NotEdge::East) {
            bit = Shift(bit, Direction::East, 1);
            rays[sq][(U8) DirectionIndex::East] |= bit;
        }

        bit = C64(sq);
        while (bit & (U64) NotEdge::South) {
            bit = Shift(bit, Direction::South, 1);
            rays[sq][(U8) DirectionIndex::South] |= bit;
        }

        bit = C64(sq);
        while (bit & (U64) NotEdge::West) {
            bit = Shift(bit, Direction::West, 1);
            rays[sq][(U8) DirectionIndex::West] |= bit;
        }

        bit = C64(sq);
        while (bit & (U64) NotEdge::North & (U64) NotEdge::West) {
            bit = Shift(bit, Direction::NorthWest, 1);
            rays[sq][(U8) DirectionIndex::NorthWest] |= bit;
        }

        bit = C64(sq);
        while (bit & (U64) NotEdge::North & (U64) NotEdge::East) {
            bit = Shift(bit, Direction::NorthEast, 1);
            rays[sq][(U8) DirectionIndex::NorthEast] |= bit;
        }

        bit = C64(sq);
        while (bit & (U64) NotEdge::South & (U64) NotEdge::West) {
            bit = Shift(bit, Direction::SouthWest, 1);
            rays[sq][(U8) DirectionIndex::SouthWest] |= bit;
        }

        bit = C64(sq);
        while (bit & (U64) NotEdge::South & (U64) NotEdge::East) {
            bit = Shift(bit, Direction::SouthEast, 1);
            rays[sq][(U8) DirectionIndex::SouthEast] |= bit;
        }
    }
}
