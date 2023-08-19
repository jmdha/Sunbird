#include <chess/internal/bitboard.hpp>
#include <engine/evaluation.hpp>
#include <engine/internal/values.hpp>
#include <jank/bit/bit.hpp>

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

int EvalPawnStructure(const Position &pos, Color color) {
    int score = 0;

    const BB pawns = pos.GetPieces(color, PieceType::Pawn);
    const Color oppColor = Utilities::GetOppositeColor(color);
    const BB oppPawns = pos.GetPieces(oppColor, PieceType::Pawn);

    // Doubled pawns
    for (auto col : COLUMNS) {
        BB pawns = pawns & (BB)col;
        int pawnCount = jank::bit::popcount(pawns);

        if (pawnCount > 1)
            score += 0.25 * (double)MaterialValue::Pawn;
    }

    // Backward pawns
    {
        BB tPawns = pawns;
        while (tPawns) {
            Square pawn = (Square)jank::bit::lsb_pop(tPawns);
            if ((PawnAttacks(pawn, oppColor) & pawns) != 0)
                continue;
            Direction moveDir = (color == Color::White) ? Direction::North : Direction::South;
            Square destSquare = (Square)jank::bit::lsb(Ray(pawn, moveDir) & Ring(pawn, 1));
            if ((PawnAttacks(destSquare, color) & oppPawns) == 0)
                continue;
            score += 0.25 * (double)MaterialValue::Pawn;
        }
    }

    return score;
}

int EvalPawnStructure(const Position &pos) {
    return EvalPawnStructure(pos, Color::White) - EvalPawnStructure(pos, Color::Black);
}

int Eval(const Position &pos) {
    const int value = EvalPosition(pos) + EvalMaterial(pos);
    return (pos.GetTurn() == Color::White) ? value : -value;
}

int EvalNoMove(const Position &pos) {
    bool isKingSafe = pos.IsKingSafe();
    // Checkmate
    if (!isKingSafe)
        return -MaterialValue::Inf;
    // Stalemate
    else
        return 0;
}
} // namespace Chess::Engine::Evaluation
