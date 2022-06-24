#ifndef EVALUATOR
#define EVALUATOR

#include "bit_board.hh"

class Evaluator {
public:
    Evaluator(Color color);
    int EvaluatePieceCount(const BitBoard board);
    
private:
    Color color;
    Color oppColor;
};
#endif
