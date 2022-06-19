#ifndef BITEVALUATOR
#define BITEVALUATOR

#include "board.hh"
#include "bit_board.hh"
#include "evaluator.hh"

class BitEvaluator : public Evaluator<BitBoard> {
public:
    int EvaluatePieceCount(const BitBoard board);
    
private:
};
#endif