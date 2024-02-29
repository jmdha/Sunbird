#include "move_ordering.hpp"

void OrderMoves(const Board &board, MoveList &moves, Move tt_move, Move pv_move) noexcept {
    std::sort(moves.begin(), moves.end(), [&](const Move &lhs, const Move &rhs) {
        if (lhs == pv_move) return true;
        if (rhs != pv_move && lhs == tt_move) return true;
        if (rhs != pv_move && rhs != tt_move) {
            return MVVLVA[board.SquarePiece(lhs.Destination())][board.SquarePiece(lhs.Origin())] >
                   MVVLVA[board.SquarePiece(rhs.Destination())][board.SquarePiece(rhs.Origin())];
        }
        return false;
    });
}
void OrderMoves(const Board &board, MoveList &moves) noexcept {
    std::sort(moves.begin(), moves.end(), [&](const Move &lhs, const Move &rhs) {
        return MVVLVA[board.SquarePiece(lhs.Destination())][board.SquarePiece(lhs.Origin())] >
               MVVLVA[board.SquarePiece(rhs.Destination())][board.SquarePiece(rhs.Origin())];
    });
}
