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
std::pair<std::optional<Move>, int> GetBestMove(Board &board, int depth);
std::pair<std::optional<Move>, int> GetBestMoveTime(Board &board, int timeLimit);
int EvaluateMove(Board &board, Move move, int timeLimit);
} // namespace Chess::Engine::Negamax

#endif // ENGINE_NEGAMAX
