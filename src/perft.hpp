#pragma once

#include "board.hpp"

namespace Perft {
int RunFromPosition(Board &board, int depth);
int PerftDivide(Board &board, int depth);
} // namespace Perft
