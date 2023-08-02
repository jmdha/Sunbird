#include <engine/evaluation.hpp>

namespace Chess::Engine::Evaluation {
int EvalMaterial(const Board &board) {
    int value = 0;
    for (int i = 0; i < PIECECOUNT - 1; ++i) {
        const int whiteCount = board.GetPieceCount(Color::White, (PieceType)i);
        const int blackCount = board.GetPieceCount(Color::Black, (PieceType)i);
        const int pieceValue = MaterialValue::ALL.at(i);
        value += (whiteCount - blackCount) * pieceValue;
    }
    return value;
}

int EvalPosition(const Board &board) { 
    return EvalPosition(board, Color::White) - EvalPosition(board, Color::Black); }

int EvalPosition(const Board &board, Color color) {
    int value = 0;

    for (int i = 0; i < PIECECOUNT; i++) {
        U64 pieces = board.GetPiecePos(color, (PieceType)i);
        if (color == Color::White)
            while (pieces)
                value += PositionValue::ALL_WHITE[i][Utilities::LSB_Pop(&pieces)];
        else
            while (pieces)
                value += PositionValue::ALL_BLACK[i][Utilities::LSB_Pop(&pieces)];
    }

    return value;
}

int Eval(const Board &board) {
    const int value = EvalPosition(board) + EvalMaterial(board);
    return (board.GetColor() == Color::White) ? value : -value;
}

int EvalNoMove(bool isKingSafe) {
    // Checkmate
    if (!isKingSafe)
        return -(int)MaterialValue::Inf;
    // Stalemate
    else
        return 0;
}

} // namespace Chess::Engine::Evaluation
