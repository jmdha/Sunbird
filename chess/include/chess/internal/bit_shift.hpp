#ifndef BIT_SHIFT
#define BIT_SHIFT

#include <algorithm>
#include <array>
#include <cassert>
#include <stdexcept>
#include <vector>

#include "constants.hpp"
#include "utilities.hpp"

namespace BitShift {
// Shifts given bitboard b once in direction dir
constexpr U64 Shift(U64 b, Direction dir) {
    if ((int)dir > 0)
        return b << (int)dir;
    else
        return b >> (((int)dir > 0) ? (int)dir : -(int)dir);
}

// Shifts and modifies given bitbord b once in direction dir
constexpr U64 ShiftM(U64 &b, Direction dir) { return b = Shift(b, dir); }

// Shifts given bitboard b n times in direction dir
constexpr U64 Shift(U64 b, Direction dir, int n) {
    if ((int)dir > 0)
        return b << (int)dir * n;
    else
        return b >> (((int)dir > 0) ? (int)dir : -(int)dir) * n;
}

namespace {
constexpr U64 GenerateRay(Square sq, DirectionIndex dir) {
    U64 ray = 0;
    U64 b = C64((U64)sq);
    while (b & (U64)notEdges[(U8)dir])
        ray |= ShiftM(b, DIRECTIONS[(U8)dir]);
    return ray;
}
} // namespace

// Defines a ray from each square in each direction
// A ray in the northeast direction:
// ....X
// ...X.
// ..o..
// .....
// .....
// Where o defines square and X the bits in the array
constexpr std::array<std::array<U64, DIRECTIONCOUNT>, SQUARECOUNT> RAYS = [] {
    auto s = decltype(RAYS){};
    for (const auto square : SQUARES)
        for (const auto index : DIRECTION_INDEXES) {
            s[(U8)square][(U8)index] = GenerateRay(square, index);
        }
    return s;
}();

namespace {
constexpr U64 GenerateSQRay(Square from, Square to) {
    DirectionIndex dir = Utilities::GetDirectionIndex(from, to);
    return RAYS[(int)from][(int)dir];
}
} // namespace

// Defines a ray from a square through each other square
// ......    ....X
// ...t..    ...X.
// ..f... -> .....
// ......    .....
// ......    .....
// Where "f" is from square, t is "to" square, and X are enabled bits
constexpr std::array<std::array<U64, SQUARECOUNT>, SQUARECOUNT> SQ_RAYS = [] {
    auto s = decltype(SQ_RAYS){};
    for (const auto from : SQUARES)
        for (const auto to : SQUARES) {
            if (from == to)
                continue;
            s[(int)from][(int)to] = GenerateSQRay(from, to);
        }
    return s;
}();

namespace {
constexpr U64 GenerateBehind(Square from, Square to) {
    return RAYS[(int)to][(int)Utilities::GetDirectionIndex(from, to)];
}
} // namespace

// Defines a ray from behind target square
// ....X
// ...t.
// ..f..
// .....
// .....
// Where f defines "from" square t defines "to" square and X defines enabled bits
constexpr std::array<std::array<U64, SQUARECOUNT>, SQUARECOUNT> XRAYS = [] {
    auto s = decltype(XRAYS){};
    for (const auto from : SQUARES)
        for (const auto to : SQUARES)
            if (from != to)
                s[(int)from][(int)to] = GenerateBehind(from, to);
    return s;
}();

namespace {
constexpr U64 GenerateRing(Square sq, U8 offset) {
    U64 ring = 0;

    const std::array<Direction, 4> directions{Direction::North, Direction::East, Direction::South,
                                              Direction::West};
    const std::array<std::array<Direction, 2>, 4> probes{
        std::array{Direction::West, Direction::East},
        std::array{Direction::North, Direction::South},
        std::array{Direction::West, Direction::East},
        std::array{Direction::North, Direction::South}};

    for (U8 i = 0; i < directions.size(); ++i) {
        U64 dot = C64((U64)sq);
        for (U8 o = 0; o < offset; ++o) {
            if (dot & (~(U64)notEdges[(U8)Utilities::GetDirectionIndex(directions.at(i))])) {
                dot = 0;
                break;
            }
            dot = Shift(dot, directions.at(i));
            for (const auto dir : probes.at(i))
                if (!(dot & (~(U64)notEdges[(U8)Utilities::GetDirectionIndex(dir)])))
                    dot |= Shift(dot, dir);
        }
        ring |= dot;
    }

    return ring;
}
} // namespace

// A ring for each square and offset
// ..... <- offset 1 XXXXX
// .XXX.             X...X
// .XpX.             X.p.X
// .XXX.             X...X
// ..... offset 2 -> XXXXX
// Where p is the square and X is the bits in the ring at a given offset
constexpr std::array<std::array<U64, 8>, SQUARECOUNT> RINGS = [] {
    auto s = decltype(RINGS){};
    for (const auto sq : SQUARES)
        for (U8 i = 0; i < 8; ++i)
            s[(int)sq][i] = GenerateRing(sq, i);
    return s;
}();

namespace {
constexpr U64 GenerateAttacks(PieceType piece, Square sq) {
    switch (piece) {
    case PieceType::Knight:
        return KnightMoves[(int)sq];
    case PieceType::King:
        return KingMoves[(int)sq];
    case PieceType::Rook:
        return RAYS[(int)sq][(int)DirectionIndex::North] |
               RAYS[(int)sq][(int)DirectionIndex::East] |
               RAYS[(int)sq][(int)DirectionIndex::South] | RAYS[(int)sq][(int)DirectionIndex::West];
    case PieceType::Bishop:
        return RAYS[(int)sq][(int)DirectionIndex::NorthWest] |
               RAYS[(int)sq][(int)DirectionIndex::NorthEast] |
               RAYS[(int)sq][(int)DirectionIndex::SouthWest] |
               RAYS[(int)sq][(int)DirectionIndex::SouthEast];
    case PieceType::Queen:
        return RAYS[(int)sq][(int)DirectionIndex::North] |
               RAYS[(int)sq][(int)DirectionIndex::East] |
               RAYS[(int)sq][(int)DirectionIndex::South] |
               RAYS[(int)sq][(int)DirectionIndex::West] |
               RAYS[(int)sq][(int)DirectionIndex::NorthWest] |
               RAYS[(int)sq][(int)DirectionIndex::NorthEast] |
               RAYS[(int)sq][(int)DirectionIndex::SouthWest] |
               RAYS[(int)sq][(int)DirectionIndex::SouthEast];
    case PieceType::Pawn:
    case PieceType::None:
        throw std::invalid_argument("Cannot generate attacks for pawn or no piece type.");
    }
}
} // namespace

// The available moves on a clear board for pieces, except pawn
constexpr std::array<std::array<U64, SQUARECOUNT>, PIECECOUNT> MOVES = [] {
    auto s = decltype(MOVES){};
    for (const auto piece : PIECES) {
        if (piece == PieceType::Pawn)
            continue;
        for (const auto sq : SQUARES)
            s[(int)piece][(int)sq] = GenerateAttacks(piece, sq);
    }
    return s;
}();

namespace {
constexpr U64 GenerateBB(PieceType piece, Square sq) {
    if ((C64(sq) & EDGE) == 0)
        return MOVES[(U8)piece][(U8)sq] & (~EDGE);

    if (C64(sq) & CORNER)
        return MOVES[(U8)piece][(U8)sq] & (~CORNER);

    if (C64(sq) & (U64)Row::Row1)
        return MOVES[(U8)piece][(U8)sq] & (~((U64)Row::Row8 | (U64)Column::A | (U64)Column::H));
    if (C64(sq) & (U64)Row::Row8)
        return MOVES[(U8)piece][(U8)sq] & (~((U64)Row::Row1 | (U64)Column::A | (U64)Column::H));
    if (C64(sq) & (U64)Column::A)
        return MOVES[(U8)piece][(U8)sq] & (~((U64)Row::Row1 | (U64)Row::Row8 | (U64)Column::H));
    if (C64(sq) & (U64)Column::H)
        return MOVES[(U8)piece][(U8)sq] & (~((U64)Row::Row1 | (U64)Row::Row8 | (U64)Column::A));
    throw std::logic_error("Unexpected square");
}
} // namespace
constexpr std::array<std::array<U64, SQUARECOUNT>, PIECECOUNT> BB = [] {
    auto s = decltype(MOVES){};
    for (const auto piece : PIECES) {
        if (piece == PieceType::Pawn || piece == PieceType::Knight || piece == PieceType::King)
            continue;
        for (const auto sq : SQUARES)
            s[(int)piece][(int)sq] = GenerateBB(piece, sq);
    }
    return s;
}();

}; // namespace BitShift

#endif // BIT_SHIFTS
