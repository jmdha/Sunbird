#pragma once

#include "types.hpp"

// Returns the number of trailing 0-bits in x, starting from the least
// significant bit
constexpr Square lsb(BB x) {
    assert(x != 0);
    return static_cast<Square>(__builtin_ctzll(x));
}

// Returns the number of trailing 0-bits in x, starting from the least
// significant bit, and
constexpr Square lsb_pop(BB &x) {
    assert(x != 0);
    const int t = lsb(x);
    x &= x - 1;
    return static_cast<Square>(t);
}

// Returns the number of leading 0-bits in x, starting from the most significant
// bit
constexpr Square msb(BB x) {
    assert(x != 0);
    static_assert(sizeof(BB) == 8);
    return static_cast<Square>(63 ^ __builtin_clzll(x));
}

// Returns the number of 1-bits in x
constexpr int popcount(BB x) { return __builtin_popcountll(x); }

template <Direction D>
constexpr BB shift(BB bb) {
    if constexpr (D == NORTH)
        return bb << 8;
    else if constexpr (D == EAST)
        return bb << 1;
    else if constexpr (D == SOUTH)
        return bb >> 8;
    else if constexpr (D == WEST)
        return bb >> 1;
    else if constexpr (D == NORTH_EAST)
        return bb << 9;
    else if constexpr (D == NORTH_WEST)
        return bb << 7;
    else if constexpr (D == SOUTH_EAST)
        return bb >> 7;
    else if constexpr (D == SOUTH_WEST)
        return bb >> 9;
    else
        return bb;
}

constexpr BB shift(BB x, Direction dir) {
    switch (dir) {
    case NORTH: return shift<NORTH>(x);
    case EAST: return shift<EAST>(x);
    case SOUTH: return shift<SOUTH>(x);
    case WEST: return shift<WEST>(x);
    case NORTH_EAST: return shift<NORTH_EAST>(x);
    case NORTH_WEST: return shift<NORTH_WEST>(x);
    case SOUTH_EAST: return shift<SOUTH_EAST>(x);
    case SOUTH_WEST: return shift<SOUTH_WEST>(x);
    case DIRECTION_NONE: return x;
    }
}

constexpr BB shift_up(BB x, Color color) {
    if (color == WHITE)
        return x << 8;
    else
        return x >> 8;
}
