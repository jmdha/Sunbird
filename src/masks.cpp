#include "masks.hpp"
#include "bb.hpp"

constexpr bool Valid(Column col, Row row) {
    return col >= COL_A && col <= COL_H && row >= ROW_1 && row <= ROW_8;
}

constexpr bool TrySet(BB &bb, int col, int row) {
    if (!Valid(static_cast<Column>(col), static_cast<Row>(row))) return false;
    bb |= ToBB(ToSquare(static_cast<Column>(col), static_cast<Row>(row)));
    return true;
}

constexpr std::array<std::array<BB, 8>, SQUARE_COUNT> RINGS = [] {
    auto rings = decltype(RINGS){0};
    for (const auto sq : SQUARES)
        for (int offset = 1; offset < 8; offset++) {
            const int DIRECTIONS[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
            const int PROBES[4][2][2]  = {
                {{1, 0}, {-1, 0}},
                {{1, 0}, {-1, 0}},
                {{0, 1}, {0, -1}},
                {{0, 1}, {0, -1}},
            };

            const Column col = ToCol(sq);
            const Row row    = ToRow(sq);

            BB ring = 0;
            for (int i = 0; i < 4; i++) {
                const Column col_root = static_cast<Column>(col + offset * DIRECTIONS[i][0]);
                const Row row_root    = static_cast<Row>(row + offset * DIRECTIONS[i][1]);
                TrySet(ring, col_root, row_root);
                for (int probe = 0; probe < 2; probe++) {
                    for (int o = 1; o <= offset; o++)
                        TrySet(
                            ring, col + offset * DIRECTIONS[i][0] + o * PROBES[i][probe][0],
                            row + offset * DIRECTIONS[i][1] + o * PROBES[i][probe][1]
                        );
                }
            }
            rings[sq][offset] = ring;
        }
    return rings;
}();

constexpr std::array<std::array<BB, SQUARE_COUNT>, SQUARE_COUNT> SQ_RAYS = [] {
    auto rays = decltype(SQ_RAYS){0};

    for (const auto ori : SQUARES)
        for (const auto dst : SQUARES) {
            Column col_ori = ToCol(ori);
            Column col_dst = ToCol(dst);
            Row row_ori    = ToRow(ori);
            Row row_dst    = ToRow(dst);

            int x = (col_ori == col_dst) ? 0 : (col_ori < col_dst ? 1 : -1);
            int y = (row_ori == row_dst) ? 0 : (row_ori < row_dst ? 1 : -1);

            BB ray = 0;
            for (int i = 1; i < 8; i++)
                TrySet(ray, col_ori + i * x, row_ori + i * y);
            rays[ori][dst] = ray;
        }

    return rays;
}();

constexpr std::array<std::array<BB, DIRECTION_COUNT>, SQUARE_COUNT> DIR_RAYS = [] {
    constexpr std::array<BB, 8> EDGES = {RANK_8,          FILE_H,          RANK_1,
                                         FILE_A,          RANK_8 | FILE_H, RANK_8 | FILE_A,
                                         RANK_1 | FILE_H, RANK_1 | FILE_A};

    auto rays = decltype(DIR_RAYS){0};

    for (const auto sq : SQUARES)
        for (const auto dir : DIRECTIONS) {
            BB ray = 0;

            BB dot = ToBB(sq);
            while (dot & (~EDGES[dir])) {
                dot = shift(dot, dir);
                ray |= dot;
            }

            rays[static_cast<size_t>(sq)][static_cast<size_t>(dir)] = ray;
        }

    return rays;
}();

constexpr std::array<std::array<BB, SQUARE_COUNT>, SQUARE_COUNT> XRAYS = [] {
    auto rays = decltype(XRAYS){0};
    for (const auto ori : SQUARES)
        for (const auto dst : SQUARES)
            rays[ori][dst] = SQ_RAYS[ori][dst] & (~SQ_RAYS[dst][ori]) & (~ToBB(dst));
    return rays;
}();

constexpr std::array<std::array<BB, SQUARE_COUNT + 1>, COLOR_COUNT> PAWN_ATTACKS = [] {
    auto attacks = decltype(PAWN_ATTACKS){0};
    for (int c = 0; c < 2; c++) {
        const int offset = (c == WHITE) ? 1 : -1;
        for (const auto sq : SQUARES) {
            TrySet(attacks[c][sq], ToCol(sq) + 1, ToRow(sq) + offset);
            TrySet(attacks[c][sq], ToCol(sq) - 1, ToRow(sq) + offset);
        }
        attacks[c][SQUARE_NONE] = 0;
    }
    return attacks;
}();

constexpr std::array<std::array<BB, SQUARE_COUNT>, PIECE_COUNT> PSEUDO_ATTACKS = [] {
    auto attacks = decltype(PSEUDO_ATTACKS){0};

    const int delta_knight[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                                    {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
    for (const auto sq : SQUARES) {
        // Set knight attacks
        for (int dir = 0; dir < 8; dir++) {
            TrySet(
                attacks[KNIGHT][sq], ToCol(sq) + delta_knight[dir][0],
                ToRow(sq) + delta_knight[dir][1]
            );
        }

        // Set king attacks
        attacks[KING][sq] = RINGS[sq][1];

        // Set bishop + rook attacks
        for (int offset = 1; offset < 8; offset++) {
            TrySet(attacks[BISHOP][sq], ToCol(sq) + offset, ToRow(sq) + offset);
            TrySet(attacks[BISHOP][sq], ToCol(sq) + offset, ToRow(sq) - offset);
            TrySet(attacks[BISHOP][sq], ToCol(sq) - offset, ToRow(sq) + offset);
            TrySet(attacks[BISHOP][sq], ToCol(sq) - offset, ToRow(sq) - offset);
            TrySet(attacks[ROOK][sq], ToCol(sq) + offset, ToRow(sq));
            TrySet(attacks[ROOK][sq], ToCol(sq) - offset, ToRow(sq));
            TrySet(attacks[ROOK][sq], ToCol(sq), ToRow(sq) + offset);
            TrySet(attacks[ROOK][sq], ToCol(sq), ToRow(sq) - offset);
            attacks[QUEEN][sq] = attacks[BISHOP][sq] | attacks[ROOK][sq];
        }
    }

    return attacks;
}();

constexpr std::array<std::array<BB, SQUARE_COUNT>, COLOR_COUNT> PAWN_PASS = [] {
    auto passes = decltype(PAWN_PASS){};

    for (const auto color : {WHITE, BLACK})
        for (const auto square : SQUARES) {
            const Direction dir[2] = {NORTH, SOUTH};
            BB bb                  = 0;

            bb |= DIR_RAYS[square][dir[color]];
            if ((FILE_A & square) == 0) {
                BB b     = shift<WEST>(ToBB(square));
                Square n = lsb(b);
                bb |= DIR_RAYS[n][dir[color]];
            }
            if ((FILE_H & square) == 0) {
                BB b     = shift<EAST>(ToBB(square));
                Square n = lsb(b);
                bb |= DIR_RAYS[n][dir[color]];
            }

            passes[static_cast<size_t>(color)][static_cast<size_t>(square)] = bb;
        }

    return passes;
}();

constexpr std::array<BB, SQUARE_COUNT> PAWN_ISOLATION = [] {
    auto values = decltype(PAWN_ISOLATION){};

    for (const auto square : SQUARES) {
        BB bb = 0;

        const Column col = ToCol(square);
        if (col > 0) bb |= FILES[col - 1];
        if (col < 7) bb |= FILES[col + 1];

        values[square] = bb;
    }
    return values;
}();
