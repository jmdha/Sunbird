#ifndef CHESS_ENGINE_NEGAMAX
#define CHESS_ENGINE_NEGAMAX

#include "chess/internal/move_list.hpp"
#include "engine/evaluation.hpp"
#include <chess/board.hpp>
#include <chess/internal/constants.hpp>
#include <chess/internal/move.hpp>
#include <functional>
#include <optional>

namespace Chess::Engine::Negamax {
static inline std::function<int(const Board &)> EVAL_FUNCTION = Evaluation::Eval;
std::pair<std::optional<Move>, int> GetBestMove(Board &board, int depth);
Move GetBestMoveTime(Board &board, int timeLimit);
MoveList GetOrderdMoves(Board &board, int timeLimit);
} // namespace Chess::Engine::Negamax

#endif // ENGINE_NEGAMAX
