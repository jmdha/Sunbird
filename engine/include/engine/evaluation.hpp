#ifndef CHESS_ENGINE_EVALUATION
#define CHESS_ENGINE_EVALUATION

#include "internal/values.hpp"
#include <chess/board.hpp>
#include <chess/internal/constants.hpp>

namespace Chess::Engine::Evaluation {
int EvalMaterial(const Board &board);
int EvalPosition(const Board &board);
int EvalPosition(const Board &board, Color color);
int Eval(const Board &board);
int EvalNoMove(bool isKingSafe);
} // namespace Chess::Engine::Evaluation

#endif // ENGINE_EVALUATION
