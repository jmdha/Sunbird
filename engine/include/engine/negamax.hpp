#ifndef CHESS_ENGINE_NEGAMAX
#define CHESS_ENGINE_NEGAMAX

#include <chess/board.hpp>
#include <chess/internal/constants.hpp>
#include <chess/internal/move.hpp>

namespace Chess::Engine::Negamax {
// TODO: Implement time limit
Move GetBestMove(Board &board, int depth = -1);
} // namespace Chess::Engine::Negamax

#endif // ENGINE_NEGAMAX
