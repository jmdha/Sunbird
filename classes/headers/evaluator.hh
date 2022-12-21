#ifndef EVALUATOR
#define EVALUATOR

#include "board.hh"
#include "constants.hh"

class Evaluator {
public:
    struct Stats {
        U64 evalCount = 0;
        U64 noMoveEvalCount = 0;
    };
    // Init
    Evaluator(Color color) : stats(Stats()), oColor(color) {};
    ~Evaluator();
    // Evaluation
    int EvaluatePieceCount(const Board &board);
    int EvaluatePositionValue(const Board &board);
    int EvaluatePositionValue(const Board &board, Color color);
    inline int Evaluate(const Board &board);
    inline int EvaluateNoMoves(const Board &board, const bool isKingSafe);
    inline int SideModifier(const Board &board, const int value);
    // Misc
    inline Stats GetStats();
private:
    Stats stats;
    Color oColor;
};

inline Evaluator::Stats Evaluator::GetStats() {
    return stats;
}

inline int Evaluator::Evaluate(const Board &board) {
#ifdef STATS
    stats.evalCount++;
#endif
    int value = EvaluatePieceCount(board) + EvaluatePositionValue(board);
    
    return SideModifier(board, value);
}

inline int Evaluator::EvaluateNoMoves(const Board &board, const bool isKingSafe) {
#ifdef STATS
    stats.noMoveEvalCount++;
#endif
    // Checkmate
    if (!isKingSafe)
        return (int) PieceValue::Inf;
    // Stalemate
    else
        return 0;
}

inline int Evaluator::SideModifier(const Board &board, const int value) {
    if (board.color == Color::Black)
        return -value;
    else
        return value;
}

#endif // EVALUATOR
