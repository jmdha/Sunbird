#ifndef CHESS_ENGINE_EVALUATION
#define CHESS_ENGINE_EVALUATION

#include "internal/values.hpp"
#include <chess/board.hpp>
#include <chess/internal/constants.hpp>

namespace Chess::Engine::Evaluation {
int EvalMaterial(const Position &pos);
int EvalPosition(const Position &pos);
int EvalPosition(const Position &pos, Color color);
int Eval(const Position &pos);
int EvalNoMove(const Position &pos);
} // namespace Chess::Engine::Evaluation

#endif // ENGINE_EVALUATION
