#include "bitboard.hpp"
#include "bit.hpp"
#include "types.hpp"
#include "utilities.hpp"

constexpr std::array<std::array<BB, DIRECTIONCOUNT>, SQUARE_COUNT> RAYS = [] {
    auto rays = decltype(RAYS){};
    for (const auto sq : SQUARES)
        for (const auto dir : DIRECTIONS) {
            BB ray = 0;

            BB dot = ToBB(sq);
            while (dot & (~EDGES[static_cast<size_t>(dir)]))
                ray |= ShiftM(dot, dir);

            rays[static_cast<size_t>(sq)][static_cast<size_t>(dir)] = ray;
        }
    return rays;
}();

constexpr std::array<std::array<BB, SQUARE_COUNT>, SQUARE_COUNT> SQRAYS = [] {
    auto rays = decltype(SQRAYS){};
    for (const auto from : SQUARES)
        for (const auto to : SQUARES) {
            if (from == to) continue;
            const Direction dir = Utilities::GetDirection(from, to);
            rays[static_cast<size_t>(from)][static_cast<size_t>(to)] = Ray(from, dir);
        }
    return rays;
}();

constexpr std::array<std::array<BB, SQUARE_COUNT>, SQUARE_COUNT> XRAYS = [] {
    auto rays = decltype(XRAYS){};
    for (const auto from : SQUARES)
        for (const auto to : SQUARES) {
            if (from == to) continue;
            const Direction dir = Utilities::GetDirection(from, to);
            rays[static_cast<size_t>(from)][static_cast<size_t>(to)] = Ray(to, dir);
        }
    return rays;
}();

namespace {
constexpr BB GenerateRing(Square sq, size_t offset) {
    BB ring = 0;

    const std::array<Direction, 4> directions{NORTH, EAST, SOUTH, WEST};
    const std::array<std::array<Direction, 2>, 4> probes{
        std::array{WEST, EAST}, std::array{NORTH, SOUTH}, std::array{WEST, EAST},
        std::array{NORTH, SOUTH}
    };

    for (size_t i = 0; i < directions.size(); ++i) {
        BB dot = ToBB(sq);
        for (size_t o = 0; o < offset; ++o) {
            if (dot & (EDGES[static_cast<size_t>(directions[i])])) {
                dot = 0;
                break;
            }
            dot = Shift(dot, directions.at(i));
            for (const auto dir : probes.at(i))
                if (!(dot & (EDGES[static_cast<size_t>(dir)]))) dot |= Shift(dot, dir);
        }
        ring |= dot;
    }

    return ring;
}
} // namespace
constexpr std::array<std::array<BB, RING_COUNT>, SQUARE_COUNT> RINGS = [] {
    auto rings = decltype(RINGS){};

    for (const auto sq : SQUARES)
        for (size_t i = 1; i < RING_COUNT; i++)
            rings[static_cast<size_t>(sq)][i] = GenerateRing(sq, i);

    return rings;
}();

namespace {
constexpr BB GenerateAttacks(Piece piece, Square sq) {
    switch (piece) {
    case KNIGHT:
        return Ring(sq, 2) & ~(Ray(sq, NORTH) | Ray(sq, EAST) | Ray(sq, SOUTH) | Ray(sq, WEST) |
                               Ray(sq, NORTH_WEST) | Ray(sq, NORTH_EAST) | Ray(sq, SOUTH_WEST) |
                               Ray(sq, SOUTH_EAST));
    case KING: return Ring(sq, 1);
    case ROOK: return Ray(sq, NORTH) | Ray(sq, EAST) | Ray(sq, SOUTH) | Ray(sq, WEST);
    case BISHOP:
        return Ray(sq, NORTH_WEST) | Ray(sq, NORTH_EAST) | Ray(sq, Direction::SOUTH_WEST) |
               Ray(sq, SOUTH_EAST);
    case QUEEN:
        return Ray(sq, NORTH) | Ray(sq, EAST) | Ray(sq, SOUTH) | Ray(sq, WEST) |
               Ray(sq, NORTH_WEST) | Ray(sq, NORTH_EAST) | Ray(sq, SOUTH_WEST) |
               Ray(sq, SOUTH_EAST);
    case PAWN:
    case PIECE_NONE:
        throw std::invalid_argument("Cannot generate attacks for pawn or no piece type.");
    }
}
} // namespace

constexpr std::array<std::array<BB, SQUARE_COUNT>, PIECE_COUNT> ATTACKS = [] {
    auto attacks = decltype(ATTACKS){};

    for (const auto p : PIECES)
        for (const auto sq : SQUARES)
            if (p != PAWN) attacks[p][sq] = GenerateAttacks(p, sq);

    return attacks;
}();

constexpr std::array<std::array<BB, SQUARE_COUNT>, PIECE_COUNT> BABS = [] {
    auto babs = decltype(BABS){};

    for (const auto pType : PIECES) {
        if (pType == PAWN || pType == KNIGHT || pType == KING) continue;
        for (const auto sq : SQUARES) {
            BB nonEdge;
            if ((sq & EDGE) == 0)
                nonEdge = ~EDGE;
            else if (sq & CORNER)
                nonEdge = ~CORNER;
            else if (sq & Row::Row1)
                nonEdge = ~(Row::Row8 | Column::A | Column::H);
            else if (sq & Row::Row8)
                nonEdge = ~(Row::Row1 | Column::A | Column::H);
            else if (sq & Column::A)
                nonEdge = ~(Row::Row1 | Row::Row8 | Column::H);
            else if (sq & Column::H)
                nonEdge = ~(Row::Row1 | Row::Row8 | Column::A);
            else
                throw std::logic_error("Should not happen");
            babs[static_cast<size_t>(pType)][static_cast<size_t>(sq)] =
                Attacks(sq, pType) & nonEdge;
        }
    }
    return babs;
}();

constexpr std::array<std::array<BB, SQUARE_COUNT>, COLOR_COUNT> PAWN_ATTACKS = [] {
    auto attacks = decltype(PAWN_ATTACKS){};

    for (const auto color : {WHITE, BLACK})
        for (const auto sq : SQUARES) {
            const BB rays = (color == WHITE) ? (Ray(sq, NORTH_EAST) | Ray(sq, NORTH_WEST))
                                             : (Ray(sq, SOUTH_EAST) | Ray(sq, SOUTH_WEST));

            attacks[static_cast<size_t>(color)][static_cast<size_t>(sq)] = Ring(sq, 1) & rays;
        }

    return attacks;
}();

constexpr std::array<std::array<BB, SQUARE_COUNT>, COLOR_COUNT> PAWN_PASS = [] {
    auto passes = decltype(PAWN_PASS){};

    for (const auto color : {WHITE, BLACK})
        for (const auto square : SQUARES) {
            const Direction dir[2] = {NORTH, SOUTH};
            BB bb                  = 0;

            bb |= Ray(square, dir[static_cast<size_t>(color)]);
            if ((Column::A & square) == 0) {
                BB b     = Shift<WEST>(ToBB(square));
                Square n = lsb(b);
                bb |= Ray(n, dir[static_cast<size_t>(color)]);
            }
            if ((Column::H & square) == 0) {
                BB b     = Shift<EAST>(ToBB(square));
                Square n = lsb(b);
                bb |= Ray(n, dir[static_cast<size_t>(color)]);
            }

            passes[static_cast<size_t>(color)][static_cast<size_t>(square)] = bb;
        }

    return passes;
}();

constexpr std::array<BB, SQUARE_COUNT> PAWN_ISOLATION = [] {
    auto values = decltype(PAWN_ISOLATION){};

    for (const auto square : SQUARES) {
        BB bb = 0;

        const size_t index = Utilities::GetColumnIndex(square);
        if (index > 0) bb |= COLUMNS[index - 1];
        if (index < 7) bb |= COLUMNS[index + 1];

        values[static_cast<size_t>(square)] = bb;
    }
    return values;
}();
