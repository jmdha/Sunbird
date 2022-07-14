#ifndef MINIMAX
#define MINIMAX

#include <random>
#include <algorithm>
#include <iterator>

#include "constants.hh"
#include "bit_board.hh"
#include "move_gen.hh"
#include "move.hh"
#include "evaluator.hh"

class MiniMax {
public:
    MiniMax(BitBoard* board);
    Move GetBestMove(int depth);
    
private:
    BitBoard* board;
    MoveGen* moveGens[2];
    Evaluator* evaluators[2];

    int NegaMax(bool original, bool doingHE, Move* bestMove, int depth, int alpha, int beta, U64 attackedSquares);
};
#endif
