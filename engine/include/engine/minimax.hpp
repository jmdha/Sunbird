#ifndef MINIMAX
#define MINIMAX

#include <random>
#include <algorithm>
#include <iterator>

#include <chess/internal/constants.hpp>
#include <chess/internal/move.hpp>
#include <chess/board.hpp>
#include <chess/move_gen.hpp>
#include "evaluator.hpp"

class MiniMax {
public:
    explicit MiniMax() : evaluator(Evaluator()) {}
    MiniMax(Board &board, const Evaluator& eval) : evaluator(eval) {}
    Move GetBestMove(Board &board, int depth = -1);

private:
    Evaluator evaluator;

    std::pair<Move, int> NegaMax(Board &board, int depth, U64 timeLimit, U64 *timeUsed);
    int NegaMax(Board &board, int depth, int alpha, int beta);
    int Quiesce(Board &board, int alpha, int beta);
};

#endif // MINIMAX
