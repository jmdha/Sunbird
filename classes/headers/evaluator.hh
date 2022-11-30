#ifndef EVALUATOR
#define EVALUATOR

#include "board.hh"

class Evaluator {
public:
    Evaluator(Color color);
    int Evalute(const Board board);
    int EvaluatePieceCount(const Board board);
    int EvaluatePositionValue(const Board board);
    int EvaluatePositionValue(const Board board, Color color);
    int GetPiecePositionValue(PieceType type, Square position, Color color);
    
private:
    Color color;
    Color oppColor;
};
#endif
