#include <engine/internal/move_ordering.hpp>

namespace Chess::Engine::MoveOrdering {
void MVVLVA(const Position &pos, MoveList &moves) {
    std::sort(moves.begin(), &moves[moves.attacks()], [pos](Move lhs, Move rhs) {
        const int leftCapture = (int)pos.GetType(lhs.GetTo());
        const int rightCapture = (int)pos.GetType(rhs.GetTo());
        if (leftCapture > rightCapture)
            return true;
        const int leftPiece = (int)pos.GetType(lhs.GetFrom());
        const int rightPiece = (int)pos.GetType(rhs.GetFrom());
        if (leftCapture == rightCapture && leftPiece < rightPiece)
            return true;
        return false;
    });
}

void All(const Position &pos, MoveList &moves) { MVVLVA(pos, moves); }
} // namespace Chess::Engine::MoveOrdering
