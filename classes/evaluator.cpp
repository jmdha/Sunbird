#include "headers/evaluator.hh"

Evaluator::Evaluator(Color color) {
    this->color = color;
    this->oppColor = Utilities::GetOppositeColor(color);
}

int Evaluator::EvaluatePieceCount(const BitBoard board) {
    int value = 0;
    for (int i = 0; i < PIECECOUNT; i++) {
        int pieceValue = Utilities::GetPieceValue((PieceType) i);
        value += Utilities::PopCount(board.pieceBB[i] & board.colorBB[(int) color]) * pieceValue;
        value -= Utilities::PopCount(board.pieceBB[i] & board.colorBB[(int) oppColor]) * pieceValue;
    }
    return value;
}
