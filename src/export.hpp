#ifndef CHESS_EXPORT
#define CHESS_EXPORT

#include "position.hpp"
#include <string>

namespace Export {
std::string FEN(const Position &board);
}

#endif // CHESS_EXPORT
