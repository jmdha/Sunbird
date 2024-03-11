#pragma once

#include "move_list.hpp"
#include "position.hpp"
#include "types.hpp"

MoveList GenerateMovesAll(const Position &pos, Color color);
MoveList GenerateMovesTactical(const Position &pos, Color color);
