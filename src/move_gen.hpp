#pragma once

#include "board.hpp"
#include "move_list.hpp"
#include "types.hpp"

MoveList GenerateMovesAll(const Board &board, Color color);
MoveList GenerateMovesTactical(const Board &board, Color color);
