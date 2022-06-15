#ifndef BITEVALUATOR
#define BITEVALUATOR

#include "board.h"
#include "bit_board.h"
#include "evaluator.h"

class BitEvaluator : public Evaluator<BitBoard> {
public:
    int EvaluatePieceCount(const BitBoard board);
    
private:
};
#endif