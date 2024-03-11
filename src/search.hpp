#pragma once

#include "board.hpp"
#include "move.hpp"

static const std::size_t WINDOW_SIZE = 50;

Move Search(const Board &c_board, std::size_t time);
