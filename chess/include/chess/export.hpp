#ifndef CHESS_EXPORT
#define CHESS_EXPORT

#include "internal/position.hpp"
#include <string>

namespace Chess::Export {
std::string FEN(const Position &board);
}

#endif // CHESS_EXPORT
