#include "chess/internal/utilities.hpp"
#include <chess/internal/bitboard.hpp>

using namespace Chess;

namespace Chess {
constexpr std::array<std::array<BB, DIRECTIONCOUNT>, SQUARECOUNT> RAYS = [] {
    auto rays = decltype(RAYS){};
    for (const auto sq : SQUARES)
        for (const auto dir : DIRECTIONS) {
            BB ray = 0;

            BB dot = ToBB(sq);
            while (dot & Utilities::NotEdge(dir))
                ray |= ShiftM(dot, dir);

            rays[(int)sq][(int)dir] = ray;
        }
    return rays;
}();

constexpr std::array<std::array<BB, SQUARECOUNT>, SQUARECOUNT> SQRAYS = [] {
    auto rays = decltype(SQRAYS){};
    for (const auto from : SQUARES)
        for (const auto to : SQUARES) {
            if (from == to)
                continue;
            rays[(int)from][(int)to] = RAYS[(int)from][(int)Utilities::GetDirection(from, to)];
        }
    return rays;
}();

constexpr std::array<std::array<BB, SQUARECOUNT>, SQUARECOUNT> XRAYS = [] {
    auto rays = decltype(XRAYS){};
    for (const auto from : SQUARES)
        for (const auto to : SQUARES) {
            if (from == to)
                continue;
            rays[(int)from][(int)to] = RAYS[(int)to][(int)Utilities::GetDirection(from, to)];
        }
    return rays;
}();

namespace {
constexpr BB GenerateRing(Square sq, int offset) {
    BB ring = 0;

    const std::array<Direction, 4> directions{Direction::North, Direction::East, Direction::South,
                                              Direction::West};
    const std::array<std::array<Direction, 2>, 4> probes{
        std::array{Direction::West, Direction::East},
        std::array{Direction::North, Direction::South},
        std::array{Direction::West, Direction::East},
        std::array{Direction::North, Direction::South}};

    for (int i = 0; i < directions.size(); ++i) {
        BB dot = ToBB(sq);
        for (int o = 0; o < offset; ++o) {
            if (dot & (~(BB)notEdges[(int)directions[i]])) {
                dot = 0;
                break;
            }
            dot = Shift(dot, directions.at(i));
            for (const auto dir : probes.at(i))
                if (!(dot & (~(BB)notEdges[(int)dir])))
                    dot |= Shift(dot, dir);
        }
        ring |= dot;
    }

    return ring;
}
} // namespace
constexpr std::array<std::array<BB, RING_COUNT>, SQUARECOUNT> RINGS = [] {
    auto rings = decltype(RINGS){};

    for (const auto sq : SQUARES)
        for (int i = 1; i < RING_COUNT; i++)
            rings[(int)sq][i] = GenerateRing(sq, i);

    return rings;
}();

namespace {
constexpr BB GenerateAttacks(PieceType piece, Square sq) {
    switch (piece) {
    case PieceType::Knight:
        return RINGS[(int)sq][2] &
               ~(RAYS[(int)sq][(int)Direction::North] | RAYS[(int)sq][(int)Direction::East] |
                 RAYS[(int)sq][(int)Direction::South] | RAYS[(int)sq][(int)Direction::West] |
                 RAYS[(int)sq][(int)Direction::NorthWest] |
                 RAYS[(int)sq][(int)Direction::NorthEast] |
                 RAYS[(int)sq][(int)Direction::SouthWest] |
                 RAYS[(int)sq][(int)Direction::SouthEast]);
    case PieceType::King:
        return RINGS[(int)sq][1];
    case PieceType::Rook:
        return RAYS[(int)sq][(int)Direction::North] | RAYS[(int)sq][(int)Direction::East] |
               RAYS[(int)sq][(int)Direction::South] | RAYS[(int)sq][(int)Direction::West];
    case PieceType::Bishop:
        return RAYS[(int)sq][(int)Direction::NorthWest] | RAYS[(int)sq][(int)Direction::NorthEast] |
               RAYS[(int)sq][(int)Direction::SouthWest] | RAYS[(int)sq][(int)Direction::SouthEast];
    case PieceType::Queen:
        return RAYS[(int)sq][(int)Direction::North] | RAYS[(int)sq][(int)Direction::East] |
               RAYS[(int)sq][(int)Direction::South] | RAYS[(int)sq][(int)Direction::West] |
               RAYS[(int)sq][(int)Direction::NorthWest] | RAYS[(int)sq][(int)Direction::NorthEast] |
               RAYS[(int)sq][(int)Direction::SouthWest] | RAYS[(int)sq][(int)Direction::SouthEast];
    case PieceType::Pawn:
    case PieceType::None:
        throw std::invalid_argument("Cannot generate attacks for pawn or no piece type.");
    }
}
} // namespace

constexpr std::array<std::array<BB, SQUARECOUNT>, PIECECOUNT> ATTACKS = [] {
    auto attacks = decltype(ATTACKS){};

    for (const auto pType :
         {PieceType::Bishop, PieceType::Knight, PieceType::Rook, PieceType::Queen, PieceType::King})
        for (const auto sq : SQUARES)
            attacks[(int)pType][(int)sq] = GenerateAttacks(pType, sq);
    return attacks;
}();

constexpr std::array<std::array<BB, SQUARECOUNT>, PIECECOUNT> BABS = [] {
    auto babs = decltype(BABS){};

    for (const auto pType : PIECES) {
        if (pType == PieceType::Pawn || pType == PieceType::Knight || pType == PieceType::King)
            continue;
        for (const auto sq : SQUARES) {
            BB nonEdge;
            if ((sq & EDGE) == 0)
                nonEdge = ~EDGE;
            else if (sq & CORNER)
                nonEdge = ~CORNER;
            else if (sq & (BB)Row::Row1)
                nonEdge = ~((BB)Row::Row8 | (BB)Column::A | (BB)Column::H);
            else if (sq & (BB)Row::Row8)
                nonEdge = ~((BB)Row::Row1 | (BB)Column::A | (BB)Column::H);
            else if (sq & (BB)Column::A)
                nonEdge = ~((BB)Row::Row1 | (BB)Row::Row8 | (BB)Column::H);
            else if (sq & (BB)Column::H)
                nonEdge = ~((BB)Row::Row1 | (BB)Row::Row8 | (BB)Column::A);
            babs[(int)pType][(int)sq] = ATTACKS[(int)pType][(int)sq] & nonEdge;
        }
    }
    return babs;
}();

constexpr std::array<std::array<BB, SQUARECOUNT>, COLORCOUNT> PAWN_ATTACKS = [] {
    auto attacks = decltype(PAWN_ATTACKS){};

    for (const auto color : {Color::White, Color::Black})
        for (const auto sq : SQUARES) {
            const BB rays = (color == Color::White) ? (RAYS[(int)sq][(int)Direction::NorthEast] |
                                                       RAYS[(int)sq][(int)Direction::NorthWest])
                                                    : (RAYS[(int)sq][(int)Direction::SouthEast] |
                                                       RAYS[(int)sq][(int)Direction::SouthWest]);
            attacks[(int)color][(int)sq] = RINGS[(int)sq][1] & rays;
        }

    return attacks;
}();

constexpr std::array<std::array<BB, SQUARECOUNT>, COLORCOUNT> PAWN_PASS = [] {
    auto passes = decltype(PAWN_PASS){};

    for (const auto color : {Color::White, Color::Black})
        for (const auto square : SQUARES) {
            const Direction dir[2] = {Direction::North, Direction::South};
            BB bb = 0;

            bb |= Ray(square, dir[(int)color]);
            if (((BB)Column::A & square) == 0) {
                BB b = Shift<Direction::West>(ToBB(square));
                Square n = (Square)jank::bit::lsb(b);
                bb |= Ray(n, dir[(int)color]);
            }
            if (((BB)Column::H & square) == 0) {
                BB b = Shift<Direction::East>(ToBB(square));
                Square n = (Square)jank::bit::lsb(b);
                bb |= Ray(n, dir[(int)color]);
            }

            passes[(int)color][(int)square] = bb;
        }

    return passes;
}();

constexpr std::array<BB, SQUARECOUNT> PAWN_ISOLATION = [] {
    auto values = decltype(PAWN_ISOLATION){};

    for (const auto square : SQUARES) {
        BB bb = 0;

        const int index = Utilities::GetColumnIndex(square);
        if (index > 0)
            bb |= (BB) COLUMNS[index - 1];
        if (index < 7)
            bb |= (BB) COLUMNS[index + 1];

        values[(int)square] = bb;
    }
    return values;
}();
} // namespace Chess
