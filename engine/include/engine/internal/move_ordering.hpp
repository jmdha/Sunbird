#ifndef CHESS_ENGINE_MOVE_ORDERING
#define CHESS_ENGINE_MOVE_ORDERING

#include <chess/internal/move_list.hpp>
#include <chess/internal/position.hpp>
#include <engine/internal/pv.hpp>

namespace Chess::Engine::MoveOrdering {
void MVVLVA(const Board& board, MoveList &moves);

void PVPrioity(const Board& board, const PV &pv, MoveList &moves);

void TTPrioity(Move move, MoveList &moves);

void All(const Board& board, Move ttMove, const PV &pv, MoveList &moves);
}

#endif
