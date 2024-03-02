#pragma once

#include "board.hpp"
#include "types.hpp"

namespace Evaluation {
int Eval(const Position &pos);
int EvalNoMove(const Position &pos);
} // namespace Evaluation
