#pragma once

#include "bit.hpp"
#include "types.hpp"
#include <stdexcept>

/**
 * Contains various functions and overloads related to bitboards
 */
#define RING_COUNT 8

extern const std::array<std::array<BB, DIRECTIONCOUNT>, SQUARE_COUNT> RAYS;
extern const std::array<std::array<BB, SQUARE_COUNT>, SQUARE_COUNT> SQRAYS;
extern const std::array<std::array<BB, SQUARE_COUNT>, SQUARE_COUNT> XRAYS;
extern const std::array<std::array<BB, SQUARE_COUNT>, PIECE_COUNT> BABS;
extern const std::array<std::array<BB, RING_COUNT>, SQUARE_COUNT> RINGS;
extern const std::array<std::array<BB, SQUARE_COUNT>, PIECE_COUNT> ATTACKS;
extern const std::array<std::array<BB, SQUARE_COUNT>, COLOR_COUNT> PAWN_ATTACKS;
extern const std::array<std::array<BB, SQUARE_COUNT>, COLOR_COUNT> PAWN_PASS;
extern const std::array<BB, SQUARE_COUNT> PAWN_ISOLATION;

constexpr inline BB ToBB(Square sq) { return (static_cast<BB>(1) << static_cast<int>(sq)); }
constexpr inline Square First(BB bb) { return static_cast<Square>(lsb(bb)); }
constexpr inline Square Last(BB bb) { return static_cast<Square>(msb(bb)); }
constexpr inline Square Next(BB &bb) { return static_cast<Square>(lsb_pop(bb)); }

// clang-format off

constexpr inline BB operator&(BB bb, Square sq) { return bb & ToBB(sq); }
constexpr inline BB operator|(BB bb, Square sq) { return bb | ToBB(sq); }
constexpr inline BB operator^(BB bb, Square sq) { return bb ^ ToBB(sq); }
constexpr inline BB &operator&=(BB &bb, Square sq) { return bb &= ToBB(sq); }
constexpr inline BB &operator|=(BB &bb, Square sq) { return bb |= ToBB(sq); }
constexpr inline BB &operator^=(BB &bb, Square sq) { return bb ^= ToBB(sq); }

constexpr inline BB operator&(Square sq, BB bb) { return bb & sq; }
constexpr inline BB operator|(Square sq, BB bb) { return bb | sq; }
constexpr inline BB operator^(Square sq, BB bb) { return bb ^ sq; }

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
    if constexpr (D == Direction::North)
        return bb << 8;
    else if constexpr (D == Direction::East)
        return bb << 1;
    else if constexpr (D == Direction::South)
        return bb >> 8;
    else if constexpr (D == Direction::West)
        return bb >> 1;
    else if constexpr (D == Direction::NorthEast)
        return bb << 9;
    else if constexpr (D == Direction::NorthWest)
        return bb << 7;
    else if constexpr (D == Direction::SouthEast)
        return bb >> 7;
    else if constexpr (D == Direction::SouthWest)
        return bb >> 9;
    else
        return bb;
}

template <Direction D>
constexpr inline BB ShiftM(BB &bb) {
    return bb = Shift<D>(bb);
}

constexpr inline BB Shift(BB bb, Direction dir) {
    // clang-format off
    switch (dir) {
        case Direction::North:     return Shift<Direction::North>(bb);
        case Direction::East:      return Shift<Direction::East>(bb);
        case Direction::South:     return Shift<Direction::South>(bb);
        case Direction::West:      return Shift<Direction::West>(bb);
        case Direction::NorthEast: return Shift<Direction::NorthEast>(bb);
        case Direction::NorthWest: return Shift<Direction::NorthWest>(bb);
        case Direction::SouthEast: return Shift<Direction::SouthEast>(bb);
        case Direction::SouthWest: return Shift<Direction::SouthWest>(bb);
        case Direction::None:      return Shift<Direction::None>(bb);
    }
    // clang-format on
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
    assert(sq != Square::None);
    assert(dir != Direction::None);
    return RAYS[static_cast<size_t>(sq)][static_cast<size_t>(dir)];
}

// Defines a ray from a square through each other square
// ......    ....X
// ...t..    ...X.
// ..f... -> .....
// ......    .....
// ......    .....
// Where "f" is from square, t is "to" square, and X are enabled bits
constexpr inline BB Ray(Square from, Square to) {
    assert(from != Square::None);
    assert(to != Square::None);
    assert(from != to);
    return SQRAYS[static_cast<size_t>(from)][static_cast<size_t>(to)];
}

// Defines a ray from behind target square
// ....X
// ...t.
// ..f..
// .....
// .....
// Where f defines "from" square t defines "to" square and X defines enabled bits
constexpr inline BB XRay(Square from, Square to) {
    assert(from != Square::None);
    assert(to != Square::None);
    return XRAYS[static_cast<size_t>(from)][static_cast<size_t>(to)];
}

constexpr inline BB BAndB(Square sq, PieceType pType) {
    assert(sq != Square::None);
    assert(pType != PieceType::None);
    assert(pType != PieceType::Pawn);
    return BABS[static_cast<size_t>(pType)][static_cast<size_t>(sq)];
}

// A ring for each square and offset
// ..... <- offset 1 XXXXX
// .XXX.             X...X
// .XpX.             X.p.X
// .XXX.             X...X
// ..... offset 2 -> XXXXX
// Where p is the square and X is the bits in the ring at a given offset
constexpr inline BB Ring(Square sq, size_t offset) {
    assert(sq != Square::None);
    assert(offset > 0 && offset < 8);
    return RINGS[static_cast<size_t>(sq)][offset];
}

constexpr inline BB KnightAttacks(Square sq) {
    return ATTACKS[static_cast<size_t>(PieceType::Knight)][static_cast<size_t>(sq)];
}

constexpr inline BB BishopAttacks(Square sq) {
    return ATTACKS[static_cast<size_t>(PieceType::Bishop)][static_cast<size_t>(sq)];
}

constexpr inline BB RookAttacks(Square sq) {
    return ATTACKS[static_cast<size_t>(PieceType::Rook)][static_cast<size_t>(sq)];
}

constexpr inline BB KingAttacks(Square sq) {
    return ATTACKS[static_cast<size_t>(PieceType::King)][static_cast<size_t>(sq)];
}

// The available moves on a clear board for pieces, except pawn
constexpr inline BB Attacks(Square sq, PieceType pType) {
    assert(pType != PieceType::Pawn);
    assert(pType != PieceType::None);
    return ATTACKS[static_cast<size_t>(pType)][static_cast<size_t>(sq)];
}

// The available moves on a clear board for pawns
constexpr inline BB PawnAttacks(Square sq, Color color) {
    assert(sq != Square::None);
    assert(color != COLOR_NONE);
    return PAWN_ATTACKS[static_cast<size_t>(color)][static_cast<size_t>(sq)];
}

constexpr inline BB PawnPassMask(Square sq, Color color) {
    assert(sq != Square::None);
    assert(color != COLOR_NONE);
    return PAWN_PASS[static_cast<size_t>(color)][static_cast<size_t>(sq)];
}
constexpr inline BB PawnIsolationMask(Square sq) {
    assert(sq != Square::None);
    return PAWN_ISOLATION[static_cast<size_t>(sq)];
}