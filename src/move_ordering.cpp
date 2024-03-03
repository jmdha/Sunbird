#include "move_ordering.hpp"
#include <algorithm>
#include <cstring>

namespace MoveOrdering {
void Killer(MoveList &moves, Move killerMove) {
    for (size_t i = moves.attacks() + 1; i < moves.size(); i++) {
        if (moves[i] == killerMove) {
            std::memmove(
                &moves[moves.attacks() + 1], &moves[moves.attacks()],
                (i - moves.attacks()) * sizeof(Move)
            );
            moves[moves.attacks()] = killerMove;
            break;
        }
    }
}

void MVVLVA(const Board &board, MoveList &moves) {
    const Position &pos = board.Pos();
    std::sort(moves.begin(), &moves[moves.attacks()], [pos](Move lhs, Move rhs) {
        const int leftCapture  = static_cast<int>(pos.GetType(lhs.Destination()));
        const int rightCapture = static_cast<int>(pos.GetType(rhs.Destination()));
        if (leftCapture > rightCapture) return true;
        const int leftPiece  = static_cast<int>(pos.GetType(lhs.Origin()));
        const int rightPiece = static_cast<int>(pos.GetType(rhs.Origin()));
        if (leftCapture == rightCapture && leftPiece < rightPiece) return true;
        return false;
    });
}

void PVPrioity(const Board &board, const PV &pv, MoveList &moves) {
    size_t pvIndex = board.Ply() - pv.ply();
    if (pvIndex > pv.size() || pv.size() == 0) return;
    Move pvMove = pv[pvIndex];
    for (size_t i = 1; i < moves.size(); i++) {
        if (moves[i] == pvMove) {
            std::memmove(&moves[1], &moves[0], i * sizeof(Move));
            moves[0] = pvMove;
            break;
        }
    }
}

void TTPrioity(Move move, MoveList &moves) {
    if (!move.IsDefined()) return;
    for (size_t i = 1; i < moves.size(); i++) {
        if (moves[i] == move) {
            std::memmove(&moves[1], &moves[0], i * sizeof(Move));
            moves[0] = move;
            break;
        }
    }
}

void All(const Board &board, Move ttMove, const PV &pv, MoveList &moves) {
    MVVLVA(board, moves);
    TTPrioity(ttMove, moves);
    PVPrioity(board, pv, moves);
}
} // namespace MoveOrdering
