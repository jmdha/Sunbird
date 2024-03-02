#ifndef CHESS_ENGINE_EVALUATION
#define CHESS_ENGINE_EVALUATION

#include "board.hpp"
#include "types.hpp"

namespace Engine::Evaluation {
int Eval(const Position &pos);
int EvalNoMove(const Position &pos);
} // namespace Engine::Evaluation

#endif // ENGINE_EVALUATION
