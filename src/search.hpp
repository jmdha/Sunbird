#pragma once

#include "board.hpp"
#include "move.hpp"
#include "pv.hpp"
#include "search_limit.hpp"

namespace Search {
namespace Internal {
/*
 * From a given position, searches all non-quiet moves
 */
int Quiesce(Board &board, int alpha, int beta, const PV &pv);
/*
 * Finds optimal move for a given position, or until the limit is reached
 */
int Negamax(
    Board &board, int alpha, int beta, int depth, int searchDepth, const PV &pv,
    SearchLimit *limit = nullptr
);
}; // namespace Internal
Move GetBestMoveDepth(Board &board, int depth);
Move GetBestMoveTime(Board &board, int timeLimit);
} // namespace Search
