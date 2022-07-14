#ifndef EVALUATOR
#define EVALUATOR

#include "bit_board.hh"

class Evaluator {
public:
    Evaluator(Color color);
    int Evalute(const BitBoard board);
    int EvaluatePieceCount(const BitBoard board);
    int EvaluatePositionValue(const BitBoard board);
    int EvaluatePositionValue(const BitBoard board, Color color);
    int GetPiecePositionValue(PieceType type, Square position, Color color);
    
private:
    Color color;
    Color oppColor;
};
#endif
