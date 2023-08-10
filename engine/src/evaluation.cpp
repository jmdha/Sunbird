#include <jank/bit/bit.hpp>
#include <engine/evaluation.hpp>

namespace Chess::Engine::Evaluation {
int EvalMaterial(const Position &pos) {
    int value = 0;
    for (int i = 0; i < PIECECOUNT - 1; ++i) {
        const int whiteCount = pos.GetPieceCount(Color::White, (PieceType)i);
        const int blackCount = pos.GetPieceCount(Color::Black, (PieceType)i);
        const int pieceValue = MaterialValue::ALL.at(i);
        value += (whiteCount - blackCount) * pieceValue;
    }
    return value;
}

int EvalPosition(const Position &pos) {
    return EvalPosition(pos, Color::White) - EvalPosition(pos, Color::Black);
}

int EvalPosition(const Position &pos, Color color) {
    int value = 0;

    for (int i = 0; i < PIECECOUNT; i++) {
        BB pieces = pos.GetPieces(color, (PieceType)i);
        if (color == Color::White)
            while (pieces)
                value += PositionValue::ALL_WHITE[i][jank::bit::lsb_pop(pieces)];
        else
            while (pieces)
                value += PositionValue::ALL_BLACK[i][jank::bit::lsb_pop(pieces)];
    }

    return value;
}

int Eval(const Position &pos) {
    const int value = EvalPosition(pos) + EvalMaterial(pos);
    return (pos.GetTurn() == Color::White) ? value : -value;
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
