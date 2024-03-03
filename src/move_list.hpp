#pragma once

#include "move.hpp"
#include "types.hpp"
#include <cstring>

struct MoveList {
    enum Type { Quiet, Attack };
    size_t size() const { return attack_count + quiet_count; }
    size_t attacks() const { return attack_count; }
    bool empty() const { return size() == 0; }
    Move &operator[](size_t i) { return moves[i]; }
    const Move &operator[](size_t i) const { return moves[i]; }
    template <Type t>
    void push(Move move) {
        if constexpr (t == Attack) {
            moves[attack_count++] = move;
        } else {
            moves[MAXMOVECOUNT - (++quiet_count)] = move;
        }
    }
    void finish() {
        std::memmove(
            &moves[attack_count], &moves[MAXMOVECOUNT - quiet_count], quiet_count * sizeof(Move)
        );
    }
    std::array<Move, MAXMOVECOUNT>::iterator begin() { return &moves[0]; }
    std::array<Move, MAXMOVECOUNT>::const_iterator begin() const { return &moves[0]; }
    std::array<Move, MAXMOVECOUNT>::iterator end() { return &moves[size()]; }
    std::array<Move, MAXMOVECOUNT>::const_iterator end() const { return &moves[size()]; }

private:
    std::array<Move, MAXMOVECOUNT> moves;
    size_t attack_count = 0;
    size_t quiet_count  = 0;
};
