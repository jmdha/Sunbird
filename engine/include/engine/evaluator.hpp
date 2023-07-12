#ifndef EVALUATOR
#define EVALUATOR

#include <chess/internal/constants.hpp>
#include <chess/board.hpp>

class Evaluator {
public:
    // Init
    explicit Evaluator() : sPieceValues(pieceValues) {};
    explicit Evaluator(std::array<int, PIECECOUNT> pValues) : sPieceValues(pValues) {};
    ~Evaluator() = default;
    // Evaluation
    int EvaluatePieceCount(const Board &board) const;
    static inline int EvaluatePositionValue(const Board &board);
    static int EvaluatePositionValue(const Board &board, Color color);
    inline int Evaluate(const Board &board) const;
    static inline int EvaluateNoMoves(bool isKingSafe) ;
    static inline int SideModifier(const Board &board, int value);
private:
    const std::array<int, PIECECOUNT> sPieceValues;
};

inline int Evaluator::Evaluate(const Board &board) const {
    const int value = EvaluatePieceCount(board) + EvaluatePositionValue(board);
    return SideModifier(board, value);
}

inline int Evaluator::EvaluateNoMoves(const bool isKingSafe) {
    // Checkmate
    if (!isKingSafe)
        return -(int) PieceValue::Inf;
    // Stalemate
    else
        return 0;
}

inline int Evaluator::EvaluatePositionValue(const Board &board) {
    return EvaluatePositionValue(board, Color::White) - EvaluatePositionValue(board, Color::Black);
}

inline int Evaluator::SideModifier(const Board &board, const int value) {
    if (board.GetColor() == Color::Black)
        return -value;
    else
        return value;
}

#endif // EVALUATOR
