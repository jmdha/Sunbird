#ifndef CHESS_ENGINE_NEGAMAX
#define CHESS_ENGINE_NEGAMAX

#include <chess/board.hpp>
#include <chess/internal/move.hpp>
#include <engine/internal/pv.hpp>
#include <engine/internal/search_limit.hpp>
#include <optional>
#include <variant>

namespace Chess::Engine::Search {
enum class AlternativeResult { Draw, Checkmate };
namespace Internal {
/*
 * From a given position, searches all non-quiet moves
 */
int Quiesce(Board &board, int alpha, int beta, const PV &pv);
/*
 * Finds optimal move for a given position, or until the limit is reached
 */
int Negamax(Board &board, int alpha, int beta, int depth, int searchDepth, const PV &pv,
            SearchLimit *limit = nullptr);
}; // namespace Internal
std::variant<Move, AlternativeResult> GetBestMoveDepth(Board &board, int depth);
std::variant<Move, AlternativeResult> GetBestMoveTime(Board &board, int timeLimit);
} // namespace Chess::Engine::Search

#endif // ENGINE_NEGAMAX
