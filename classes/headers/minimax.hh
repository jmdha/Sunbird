#ifndef MINIMAX
#define MINIMAX

#include "constants.hh"
#include "bit_board.hh"
#include "move_gen.hh"
#include "evaluator.hh"

class MiniMax {
public:
    MiniMax(BitBoard* board);
    
private:
    BitBoard* board;
    MoveGen* moveGens[2];
    Evaluator* evaluators[2];

};
#endif
