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
    explicit Evaluator(Color color) : stats(Stats()), oColor(color) {};
    ~Evaluator();
    // Evaluation
    static int EvaluatePieceCount(const Board &board);
    static int EvaluatePositionValue(const Board &board);
    static int EvaluatePositionValue(const Board &board, Color color);
    static int EvaluatePawnStructure(const Board &board);
    static int EvaluatePawnStructure(const Board &board, Color color);
    inline int Evaluate(const Board &board);
    inline int EvaluateNoMoves(const Board &board, bool isKingSafe);
    static inline int SideModifier(const Board &board, int value);
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
    ++stats.evalCount;
#endif
    int value = EvaluatePieceCount(board) + EvaluatePositionValue(board) + EvaluatePawnStructure(board);
    
    return SideModifier(board, value);
}

inline int Evaluator::EvaluateNoMoves(const Board &board, const bool isKingSafe) {
#ifdef STATS
    ++stats.noMoveEvalCount;
#endif
    // Checkmate
    if (!isKingSafe)
        return SideModifier(board, -(int) PieceValue::Inf);
    // Stalemate
    else
        return 0;
}

inline int Evaluator::SideModifier(const Board &board, const int value) {
    if (board.GetColor() == Color::Black)
        return -value;
    else
        return value;
}

#endif // EVALUATOR
