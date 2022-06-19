#ifndef EVALUATOR
#define EVALUATOR

#include "board.hh"

template <class Board>
class Evaluator {
public:
    virtual int EvaluatePieceCount(const Board board) = 0;
    
private:
};
#endif