#ifndef MINIMAX
#define MINIMAX

#include <random>
#include <algorithm>
#include <iterator>

#include "constants.hh"
#include "board.hh"
#include "move_gen.hh"
#include "move.hh"
#include "evaluator.hh"

class MiniMax {
public:
    MiniMax(Board* board);
    ~MiniMax() {
        delete(moveGens[0]);
        delete(moveGens[1]);
    }
    Move GetBestMove(int depth);
    
private:
    Board *board;
    MoveGen *moveGens[2];
    Evaluator evaluator;

    Move NegaMax(int depth);
    int NegaMax(int depth, int alpha, int beta, U64 attackedSquares[2]);
};
#endif // MINIMAX
