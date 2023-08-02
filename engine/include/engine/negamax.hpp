#ifndef CHESS_ENGINE_NEGAMAX
#define CHESS_ENGINE_NEGAMAX

#include "chess/internal/move_list.hpp"
#include "engine/evaluation.hpp"
#include <chess/board.hpp>
#include <chess/internal/constants.hpp>
#include <chess/internal/move.hpp>
#include <functional>
#include <optional>
#include <variant>

namespace Chess::Engine::Negamax {
enum AlternativeResult {
    Draw,
    Checkmate
};
void SetEvalFunc(std::function<int(const Board&)> func);
std::pair<std::optional<Move>, int> GetBestMove(Board &board, int depth);
std::variant<Move, AlternativeResult> GetBestMoveTime(Board &board, int timeLimit);
MoveList GetOrderdMoves(Board &board, int timeLimit);
} // namespace Chess::Engine::Negamax

#endif // ENGINE_NEGAMAX
