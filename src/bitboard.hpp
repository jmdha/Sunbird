#pragma once

#include "types.hpp"
#include <cassert>
#include <stdexcept>

/**
 * Contains various functions and overloads related to bitboards
 */
#define RING_COUNT 8
#define EDGE 0xff818181818181ff
#define CORNER 0x8100000000000081

constexpr std::array<Square, 2> CORNER_A = {A1, A8};
constexpr std::array<Square, 2> CORNER_H = {H1, H8};

extern const std::array<std::array<BB, DIRECTION_COUNT>, SQUARE_COUNT> RAYS;
extern const std::array<std::array<BB, SQUARE_COUNT>, SQUARE_COUNT> SQRAYS;
extern const std::array<std::array<BB, SQUARE_COUNT>, SQUARE_COUNT> XRAYS;
extern const std::array<std::array<BB, SQUARE_COUNT>, PIECE_COUNT> BABS;
extern const std::array<std::array<BB, RING_COUNT>, SQUARE_COUNT> RINGS;
extern const std::array<std::array<BB, SQUARE_COUNT>, PIECE_COUNT> ATTACKS;
extern const std::array<std::array<BB, SQUARE_COUNT>, COLOR_COUNT> PAWN_ATTACKS;
extern const std::array<std::array<BB, SQUARE_COUNT>, COLOR_COUNT> PAWN_PASS;
extern const std::array<BB, SQUARE_COUNT> PAWN_ISOLATION;

// clang-format off

constexpr inline BB operator&(Column l, Column r) { return static_cast<BB>(l) & static_cast<BB>(r); }
constexpr inline BB operator|(Column l, Column r) { return static_cast<BB>(l) | static_cast<BB>(r); }
constexpr inline BB operator^(Column l, Column r) { return static_cast<BB>(l) ^ static_cast<BB>(r); }
constexpr inline BB operator&(BB bb, Column c) { return bb & static_cast<BB>(c); }
constexpr inline BB operator|(BB bb, Column c) { return bb | static_cast<BB>(c); }
constexpr inline BB operator^(BB bb, Column c) { return bb ^ static_cast<BB>(c); }
constexpr inline BB operator&=(BB bb, Column c) { return bb &= static_cast<BB>(c); }
constexpr inline BB operator|=(BB bb, Column c) { return bb |= static_cast<BB>(c); }
constexpr inline BB operator^=(BB bb, Column c) { return bb ^= static_cast<BB>(c); }
constexpr inline BB operator&(Column c, Square sq) { return static_cast<BB>(c) & sq; }
constexpr inline BB operator|(Column c, Square sq) { return static_cast<BB>(c) | sq; }
constexpr inline BB operator^(Column c, Square sq) { return static_cast<BB>(c) ^ sq; }
constexpr inline BB operator&(Square sq, Column c) { return c & sq; }
constexpr inline BB operator|(Square sq, Column c) { return c | sq; }
constexpr inline BB operator^(Square sq, Column c) { return c ^ sq; }

constexpr inline BB operator&(Row l, Row r) { return static_cast<BB>(l) & static_cast<BB>(r); }
constexpr inline BB operator|(Row l, Row r) { return static_cast<BB>(l) | static_cast<BB>(r); }
constexpr inline BB operator^(Row l, Row r) { return static_cast<BB>(l) ^ static_cast<BB>(r); }
constexpr inline BB operator&(BB bb, Row r) { return bb & static_cast<BB>(r); }
constexpr inline BB operator|(BB bb, Row r) { return bb | static_cast<BB>(r); }
constexpr inline BB operator^(BB bb, Row r) { return bb ^ static_cast<BB>(r); }
constexpr inline BB operator&=(BB bb, Row r) { return bb &= static_cast<BB>(r); }
constexpr inline BB operator|=(BB bb, Row r) { return bb |= static_cast<BB>(r); }
constexpr inline BB operator^=(BB bb, Row r) { return bb ^= static_cast<BB>(r); }
constexpr inline BB operator&(Row r, Square sq) { return static_cast<BB>(r) & sq; }
constexpr inline BB operator|(Row r, Square sq) { return static_cast<BB>(r) | sq; }
constexpr inline BB operator^(Row r, Square sq) { return static_cast<BB>(r) ^ sq; }
constexpr inline BB operator&(Square sq, Row r) { return r & sq; }
constexpr inline BB operator|(Square sq, Row r) { return r | sq; }
constexpr inline BB operator^(Square sq, Row r) { return r ^ sq; }

constexpr inline BB operator&(Column c, Row r) { return static_cast<BB>(c) & static_cast<BB>(r); }
constexpr inline BB operator|(Column c, Row r) { return static_cast<BB>(c) | static_cast<BB>(r); }
constexpr inline BB operator^(Column c, Row r) { return static_cast<BB>(c) ^ static_cast<BB>(r); }
constexpr inline BB operator&(Row r, Column c) { return c & r; }
constexpr inline BB operator|(Row r, Column c) { return c | r; }
constexpr inline BB operator^(Row r, Column c) { return c ^ r; }

// clang-format on

template <Direction D>
constexpr inline BB Shift(BB bb) {
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

template <Direction D>
constexpr inline BB ShiftM(BB &bb) {
    return bb = Shift<D>(bb);
}

constexpr inline BB Shift(BB bb, Direction dir) {
    switch (dir) {
    case NORTH: return Shift<NORTH>(bb);
    case EAST: return Shift<EAST>(bb);
    case SOUTH: return Shift<SOUTH>(bb);
    case WEST: return Shift<WEST>(bb);
    case NORTH_EAST: return Shift<NORTH_EAST>(bb);
    case NORTH_WEST: return Shift<NORTH_WEST>(bb);
    case SOUTH_EAST: return Shift<SOUTH_EAST>(bb);
    case SOUTH_WEST: return Shift<SOUTH_WEST>(bb);
    case DIRECTION_NONE: return Shift<DIRECTION_NONE>(bb);
    }
    throw std::invalid_argument("Invalid control flow.");
}

constexpr inline BB Shift(BB bb, Direction dir, size_t count) {
    for (size_t i = 0; i < count; i++)
        bb = Shift(bb, dir);
    return bb;
}

constexpr inline BB ShiftM(BB &bb, Direction dir) { return bb = Shift(bb, dir); }

constexpr inline BB ShiftM(BB &bb, Direction dir, size_t count) {
    for (size_t i = 0; i < count; i++)
        bb = Shift(bb, dir);
    return bb;
}

constexpr inline bool Multiple(BB bb) { return bb & (bb - 1); }

// Defines a ray from each square in each direction
// A ray in the northeast direction:
// ....X
// ...X.
// ..o..
// .....
// .....
// Where o defines square and X the bits in the array
constexpr inline BB Ray(Square sq, Direction dir) {
    assert(sq != SQUARE_NONE);
    assert(dir != DIRECTION_NONE);
    return RAYS[sq][static_cast<size_t>(dir)];
}

// Defines a ray from a square through each other square
// ......    ....X
// ...t..    ...X.
// ..f... -> .....
// ......    .....
// ......    .....
// Where "f" is from square, t is "to" square, and X are enabled bits
constexpr inline BB Ray(Square from, Square to) {
    assert(from != SQUARE_NONE);
    assert(to != SQUARE_NONE);
    assert(from != to);
    return SQRAYS[from][to];
}

// Defines a ray from behind target square
// ....X
// ...t.
// ..f..
// .....
// .....
// Where f defines "from" square t defines "to" square and X defines enabled bits
constexpr inline BB XRay(Square from, Square to) {
    assert(from != SQUARE_NONE);
    assert(to != SQUARE_NONE);
    return XRAYS[from][to];
}

constexpr inline BB BAndB(Square sq, Piece pType) {
    assert(sq != SQUARE_NONE);
    assert(pType != PIECE_NONE);
    assert(pType != PAWN);
    return BABS[pType][sq];
}

// A ring for each square and offset
// ..... <- offset 1 XXXXX
// .XXX.             X...X
// .XpX.             X.p.X
// .XXX.             X...X
// ..... offset 2 -> XXXXX
// Where p is the square and X is the bits in the ring at a given offset
constexpr inline BB Ring(Square sq, size_t offset) {
    assert(sq != SQUARE_NONE);
    assert(offset > 0 && offset < 8);
    return RINGS[sq][offset];
}

constexpr inline BB KnightAttacks(Square sq) { return ATTACKS[KNIGHT][sq]; }

constexpr inline BB BishopAttacks(Square sq) { return ATTACKS[BISHOP][sq]; }

constexpr inline BB RookAttacks(Square sq) { return ATTACKS[ROOK][sq]; }

constexpr inline BB KingAttacks(Square sq) { return ATTACKS[KING][sq]; }

// The available moves on a clear board for pieces, except pawn
constexpr inline BB Attacks(Square sq, Piece pType) {
    assert(pType != PAWN);
    assert(pType != PIECE_NONE);
    return ATTACKS[pType][sq];
}

// The available moves on a clear board for pawns
constexpr inline BB PawnAttacks(Square sq, Color color) {
    assert(sq != SQUARE_NONE);
    assert(color != COLOR_NONE);
    return PAWN_ATTACKS[color][sq];
}

constexpr inline BB PawnPassMask(Square sq, Color color) {
    assert(sq != SQUARE_NONE);
    assert(color != COLOR_NONE);
    return PAWN_PASS[color][sq];
}
constexpr inline BB PawnIsolationMask(Square sq) {
    assert(sq != SQUARE_NONE);
    return PAWN_ISOLATION[sq];
}
