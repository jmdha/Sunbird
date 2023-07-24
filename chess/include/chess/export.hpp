#ifndef CHESS_EXPORT
#define CHESS_EXPORT

#include "board.hpp"
#include <string>

namespace Chess::Export {
std::string FEN(const Board &board);
}

#endif // CHESS_EXPORT
