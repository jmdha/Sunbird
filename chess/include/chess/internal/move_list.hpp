#ifndef CHESS_MOVE_LIST
#define CHESS_MOVE_LIST

#include "chess/board.hpp"
#include "constants.hpp"
#include "move.hpp"
#include <algorithm>
#include <vector>

namespace Chess {
struct MoveList {
    size_t size() const { return index; }
    bool empty() const { return index == 0; }
    // Selection sort
    void sort(std::array<int, MAXMOVECOUNT> &scores) {
        for (int i = 0; i < index; i++)
            for (int t = i + 1; t < index; t++)
                if (scores[i] < scores[t]) {
                    std::swap(scores[i], scores[t]);
                    std::swap(moves[i], moves[t]);
                }
    }
    Move &operator[](int i) { return moves[i]; }
    const Move &operator[](int i) const { return moves[i]; }
    void operator<<(Move move) {
        moves[index++] = move;
        if (move.IsCapture())
            quietIndex++;
    }
    std::array<Move, MAXMOVECOUNT>::iterator begin() { return &moves[0]; }
    std::array<Move, MAXMOVECOUNT>::const_iterator begin() const { return &moves[0]; }
    std::array<Move, MAXMOVECOUNT>::iterator end() { return &moves[index]; }
    std::array<Move, MAXMOVECOUNT>::const_iterator end() const { return &moves[index]; }

private:
    std::array<Move, MAXMOVECOUNT> moves;
    int index = 0;
    // Index of first quiet move
    // Attack moves are always stored first in moves
    int quietIndex = 0;
};
} // namespace Chess

#endif // CHESS_MOVE_LIST
