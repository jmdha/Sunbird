#pragma once

#include "board.hpp"
#include "move_gen.hpp"

// clang-format off
static const int MVVLVA[6][6] {
    {15, 14, 13, 12, 11, 10},
    {25, 24, 23, 22, 21, 20},
    {35, 34, 33, 32, 31, 30},
    {45, 44, 43, 42, 41, 40},
    {55, 54, 53, 52, 51, 50},
    { 0,  0,  0,  0,  0,  0}
};
// clang-format on

void OrderMoves(const Board &board, MoveList &moves, Move tt_move, Move pv_move) noexcept;
void OrderMoves(const Board &board, MoveList &moves) noexcept;
