#include "headers/evaluator.hh"

Evaluator::Evaluator(Color color) :
    color(color),
    oppColor(Utilities::GetOppositeColor(color)) {}

int Evaluator::EvaluatePieceCount(const BitBoard board) {
    int value = 0;
    for (int i = 0; i < PIECECOUNT; i++) {
        int pieceValue = Utilities::GetPieceValue((PieceType) i);
        value += board.popCount[(int) color][i] * pieceValue;
        value -= board.popCount[(int) oppColor][i] * pieceValue;
    }
    return value;
}
