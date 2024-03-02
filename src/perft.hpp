#ifndef CHESS_PERFT
#define CHESS_PERFT

#include "board.hpp"

namespace Chess::Perft {
int RunFromPosition(Board &board, int depth);
int PerftDivide(Board &board, int depth);
} // namespace Chess::Perft
#endif // PERFT
