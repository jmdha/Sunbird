#ifndef CHESS_ENGINE_MOVE_ORDERING
#define CHESS_ENGINE_MOVE_ORDERING

#include <chess/internal/move_list.hpp>
#include <chess/internal/position.hpp>

namespace Chess::Engine::MoveOrdering {
void MVVLVA(const Position& pos, MoveList &moves);

void All(const Position& pos, MoveList &moves);
}

#endif
