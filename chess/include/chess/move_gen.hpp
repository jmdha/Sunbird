#ifndef CHESS_MOVE_GEN
#define CHESS_MOVE_GEN

#include "internal/move_list.hpp"
#include "internal/position.hpp"
#include "internal/types.hpp"

namespace Chess::MoveGen {
MoveList GenerateAll(const Position &pos, Color color);
MoveList GenerateAttack(const Position &pos, Color color);
} // namespace Chess::MoveGen

#endif
