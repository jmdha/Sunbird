#ifndef CHESS_BITBOARD
#define CHESS_BITBOARD

#include "chess/internal/utilities.hpp"
#include "constants.hpp"
#include <cstdint>

/**
 * Contains various functions and overloads related to bitboards
 */
namespace Chess {
#define RING_COUNT 8

extern const std::array<std::array<BB, DIRECTIONCOUNT>, SQUARECOUNT> RAYS;
extern const std::array<std::array<BB, SQUARECOUNT>, SQUARECOUNT> SQRAYS;
extern const std::array<std::array<BB, SQUARECOUNT>, SQUARECOUNT> XRAYS;
extern const std::array<std::array<BB, SQUARECOUNT>, PIECECOUNT> BABS;
extern const std::array<std::array<BB, RING_COUNT>, SQUARECOUNT> RINGS;
extern const std::array<std::array<BB, SQUARECOUNT>, PIECECOUNT> ATTACKS;
extern const std::array<std::array<BB, SQUARECOUNT>, COLORCOUNT> PAWN_ATTACKS;

constexpr inline BB ToBB(Square sq) { return ((BB)1 << (int)sq); }

constexpr inline BB operator&(BB bb, Square sq) { return bb & ToBB(sq); }
constexpr inline BB operator|(BB bb, Square sq) { return bb | ToBB(sq); }
constexpr inline BB operator^(BB bb, Square sq) { return bb ^ ToBB(sq); }
constexpr inline BB &operator&=(BB &bb, Square sq) { return bb &= ToBB(sq); }
constexpr inline BB &operator|=(BB &bb, Square sq) { return bb |= ToBB(sq); }
constexpr inline BB &operator^=(BB &bb, Square sq) { return bb ^= ToBB(sq); }

constexpr inline BB operator&(Square sq, BB bb) { return bb & sq; }
constexpr inline BB operator|(Square sq, BB bb) { return bb | sq; }
constexpr inline BB operator^(Square sq, BB bb) { return bb ^ sq; }

template <Direction D> constexpr inline BB Shift(BB bb) {
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

template <Direction D> constexpr inline BB ShiftM(BB &bb) { return bb = Shift<D>(bb); }

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

constexpr inline BB ShiftM(BB &bb, Direction dir) { return bb = Shift(bb, dir); }

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
    return RAYS[(int)sq][(int)dir];
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
    return SQRAYS[(int)from][(int)to];
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
    return XRAYS[(int)from][(int)to];
}

constexpr inline BB BAndB(Square sq, PieceType pType) {
    assert(sq != Square::None);
    assert(pType != PieceType::None);
    assert(pType != PieceType::Pawn);
    return BABS[(int)pType][(int)sq];
}

// A ring for each square and offset
// ..... <- offset 1 XXXXX
// .XXX.             X...X
// .XpX.             X.p.X
// .XXX.             X...X
// ..... offset 2 -> XXXXX
// Where p is the square and X is the bits in the ring at a given offset
constexpr inline BB Ring(Square sq, int offset) {
    assert(sq != Square::None);
    assert(offset > 0 && offset < 8);
    return RINGS[(int)sq][offset];
}

// The available moves on a clear board for pieces, except pawn
constexpr inline BB Attacks(Square sq, PieceType pType) {
    assert(pType != PieceType::Pawn);
    assert(pType != PieceType::None);
    return ATTACKS[(int)pType][(int)sq];
}

// The available moves on a clear board for pawns
constexpr inline BB PawnAttacks(Square sq, Color color) {
    assert(sq != Square::None);
    assert(color != Color::None);
    return PAWN_ATTACKS[(int)color][(int)sq];
}
} // namespace Chess
#endif
