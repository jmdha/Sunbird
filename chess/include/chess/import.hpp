#ifndef IMPORT
#define IMPORT

#include <optional>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "board.hpp"

namespace Import {
// Format: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
Board FEN(std::string FEN);

// Format: d2d4 e7e5 etc
Board MoveSequence(std::string moves);

// https://en.wikipedia.org/wiki/Algebraic_notation_(chess)
Board Algebraic(std::string game);
}; // namespace Import

#endif
