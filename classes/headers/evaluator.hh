#ifndef EVALUATOR
#define EVALUATOR

#include "board.hh"
#include "constants.hh"

class Evaluator {
public:
    struct Stats {
        U64 evalCount = 0;
    };
    // Init
    Evaluator(Color color) : stats(Stats()), oColor(color) {};
    ~Evaluator();
    // Evaluation
    int Evalute(const Board &board);
    int EvaluatePieceCount(const Board &board);
    int EvaluatePositionValue(const Board &board);
    int EvaluatePositionValue(const Board &board, Color color);
    // Misc
    inline Stats GetStats();
private:
    Stats stats;
    Color oColor;
};

inline Evaluator::Stats Evaluator::GetStats() {
    return stats;
}

#endif // EVALUATOR
