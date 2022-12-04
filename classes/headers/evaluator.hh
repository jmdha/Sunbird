#ifndef EVALUATOR
#define EVALUATOR

#include "board.hh"

class Evaluator {
public:
    struct Stats {
        U64 evalCount = 0;
    };
    // Init
    Evaluator(Color color);
    ~Evaluator();
    // Evaluation
    int Evalute(const Board board);
    int EvaluatePieceCount(const Board board);
    int EvaluatePositionValue(const Board board);
    int EvaluatePositionValue(const Board board, Color color);
    // Misc
    inline Stats GetStats();
private:
    Color color;
    Color oppColor;
    Stats stats;
};

inline Evaluator::Stats Evaluator::GetStats() {
    return stats;
}

#endif // EVALUATOR
