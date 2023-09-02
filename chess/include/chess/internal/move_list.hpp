#ifndef CHESS_MOVE_LIST
#define CHESS_MOVE_LIST

#include "../board.hpp"
#include "constants.hpp"
#include "move.hpp"
#include <algorithm>
#include <vector>

namespace Chess {
struct MoveList {
    size_t size() const { return index; }
    size_t attacks() const { return quietIndex; }
    bool empty() const { return index == 0; }
    // Selection sort
    void sort(std::array<int, MAXMOVECOUNT> &scores) {
        for (size_t i = 0; i < index; i++)
            for (size_t t = i + 1; t < index; t++)
                if (scores[i] < scores[t]) {
                    std::swap(scores[i], scores[t]);
                    std::swap(moves[i], moves[t]);
                }
    }
    Move &operator[](size_t i) { return moves[i]; }
    const Move &operator[](size_t i) const { return moves[i]; }
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
    size_t index = 0;
    // Index of first quiet move
    // Attack moves are always stored first in moves
    size_t quietIndex = 0;
};
} // namespace Chess

#endif // CHESS_MOVE_LIST
