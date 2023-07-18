#ifndef MOVE_LIST
#define MOVE_LIST

#include "constants.hpp"
#include "move.hpp"

struct MoveList {
    size_t size() const { return index; }
    bool empty() const { return index == 0; }
    Move &operator[](U8 i) { return moves[i]; }
    const Move &operator[](U8 i) const { return moves[i]; }
    void operator<<(Move move) { moves[index++] = std::move(move); }
    void operator<<(MoveList moves) {
        for (auto &move : moves)
            *this<<move;
    }
    std::array<Move, MAXMOVECOUNT>::iterator begin() { return &moves[0]; }
    std::array<Move, MAXMOVECOUNT>::const_iterator begin() const {
        return &moves[0];
    }
    std::array<Move, MAXMOVECOUNT>::iterator end() { return &moves[index]; }
    std::array<Move, MAXMOVECOUNT>::const_iterator end() const {
        return &moves[index];
    }

  private:
    std::array<Move, MAXMOVECOUNT> moves;
    U8 index = 0;
};

#endif

