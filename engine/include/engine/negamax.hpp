#ifndef CHESS_ENGINE_NEGAMAX
#define CHESS_ENGINE_NEGAMAX

#include "engine/evaluation.hpp"
#include <chess/board.hpp>
#include <chess/internal/constants.hpp>
#include <chess/internal/move.hpp>
#include <functional>
#include <optional>

namespace Chess::Engine::Negamax {
static inline std::function<int(const Board &)> EVAL_FUNCTION = Evaluation::Eval;
// TODO: Implement time limit
std::pair<std::optional<Move>, int> GetBestMove(Board &board, int depth);
} // namespace Chess::Engine::Negamax

#endif // ENGINE_NEGAMAX
