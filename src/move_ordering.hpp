#ifndef CHESS_ENGINE_MOVE_ORDERING
#define CHESS_ENGINE_MOVE_ORDERING

#include "board.hpp"
#include "move_list.hpp"
#include "position.hpp"
#include "pv.hpp"

namespace MoveOrdering {
void Killer(MoveList &moves, Move killerMove);
void MVVLVA(const Board &board, MoveList &moves);
void PVPrioity(const Board &board, const PV &pv, MoveList &moves);
void TTPrioity(Move move, MoveList &moves);
void All(const Board &board, Move ttMove, const PV &pv, MoveList &moves);
} // namespace MoveOrdering

#endif
