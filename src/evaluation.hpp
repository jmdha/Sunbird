#ifndef CHESS_ENGINE_EVALUATION
#define CHESS_ENGINE_EVALUATION

#include "board.hpp"
#include "types.hpp"

namespace Evaluation {
int Eval(const Position &pos);
int EvalNoMove(const Position &pos);
} // namespace Evaluation

#endif // ENGINE_EVALUATION
