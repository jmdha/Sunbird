#ifndef ENGINE_NEGAMAX
#define ENGINE_NEGAMAX

#include <chess/board.hpp>
#include <chess/internal/constants.hpp>
#include <chess/internal/move.hpp>

namespace Engine::Negamax {
// TODO: Implement time limit
Move GetBestMove(Board &board, int depth = -1);
} // namespace Engine::Negamax

#endif // ENGINE_NEGAMAX
