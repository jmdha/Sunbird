#include "headers/bit_evaluator.hh"

int BitEvaluator::EvaluatePieceCount(const BitBoard board) {
    int value = 0;
    for (int i = 0; i < PIECECOUNT; i++) {
        int pieceValue = Utilities::GetPieceValue((PieceType) i);
        value += Utilities::PopCount(board.pieceBB[i] & board.colorBB[(int) Color::White]) * pieceValue;
        value -= Utilities::PopCount(board.pieceBB[i] & board.colorBB[(int) Color::Black]) * pieceValue;
    }
    return value;
}