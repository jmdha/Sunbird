#pragma once

#include "board.hpp"
#include "types.hpp"
#include <string>

namespace Import {
/**
 * Creates a board and sets the state to that of the given FEN string
 * https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
 */
Board FEN(std::string FEN = FEN_START);
} // namespace Import
