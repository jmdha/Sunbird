#pragma once

#include "types.hpp"

constexpr BB RANK_1 = 0xff;
constexpr BB RANK_2 = 0xff00;
constexpr BB RANK_3 = 0xff0000;
constexpr BB RANK_4 = 0xff000000;
constexpr BB RANK_5 = 0xff00000000;
constexpr BB RANK_6 = 0xff0000000000;
constexpr BB RANK_7 = 0xff000000000000;
constexpr BB RANK_8 = 0xff00000000000000;
constexpr std::array<BB, 8> RANKS{RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8};

constexpr BB FILE_A = 0x101010101010101;
constexpr BB FILE_B = 0x202020202020202;
constexpr BB FILE_C = 0x404040404040404;
constexpr BB FILE_D = 0x808080808080808;
constexpr BB FILE_E = 0x1010101010101010;
constexpr BB FILE_F = 0x2020202020202020;
constexpr BB FILE_G = 0x4040404040404040;
constexpr BB FILE_H = 0x8080808080808080;
constexpr std::array<BB, 8> FILES{FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H};

constexpr std::array<Square, 2> CORNER_A = {A1, A8};
constexpr std::array<Square, 2> CORNER_H = {H1, H8};
constexpr BB CORNERS                     = ToBB(A1) | ToBB(A8) | ToBB(H1) | ToBB(H8);
constexpr BB EDGE_HORIZONTAL             = RANK_1 | RANK_8;
constexpr BB EDGE_VERTICAL               = FILE_A | FILE_H;
constexpr BB EDGE                        = EDGE_HORIZONTAL | EDGE_VERTICAL;

constexpr BB CENTER_RANKS = RANK_3 | RANK_4 | RANK_5 | RANK_6;
constexpr BB CENTER_FILES = FILE_C | FILE_D | FILE_E | FILE_F;
constexpr BB CENTER       = CENTER_RANKS & CENTER_FILES;

extern const std::array<std::array<BB, 8>, SQUARE_COUNT> RINGS;
extern const std::array<std::array<BB, SQUARE_COUNT>, SQUARE_COUNT> SQ_RAYS;
extern const std::array<std::array<BB, DIRECTION_COUNT>, SQUARE_COUNT> DIR_RAYS;
extern const std::array<std::array<BB, SQUARE_COUNT>, SQUARE_COUNT> XRAYS;
extern const std::array<std::array<BB, SQUARE_COUNT + 1>, COLOR_COUNT> PAWN_ATTACKS;
extern const std::array<std::array<BB, SQUARE_COUNT>, PIECE_COUNT> PSEUDO_ATTACKS;
extern const std::array<std::array<BB, SQUARE_COUNT>, COLOR_COUNT> PAWN_PASS;
extern const std::array<BB, SQUARE_COUNT> PAWN_ISOLATION;
