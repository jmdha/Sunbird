#pragma once

#include "board.hpp"
#include "move.hpp"
#include "types.hpp"
#include <algorithm>

class MoveList {
public:
    size_t size() const { return move_count; }
    void size(std::size_t v) { move_count = v; }
    void operator<<(Move move) { moves[move_count++] = move; }
    Move &operator[](size_t i) { return moves[i]; }
    Move operator[](size_t i) const { return moves[i]; }
    std::array<Move, MAX_MOVES>::iterator begin() { return &moves[0]; }
    std::array<Move, MAX_MOVES>::const_iterator begin() const { return &moves[0]; }
    std::array<Move, MAX_MOVES>::iterator end() { return &moves[size()]; }
    std::array<Move, MAX_MOVES>::const_iterator end() const { return &moves[size()]; }
    bool contains(const Move &move) const { return std::find(begin(), end(), move) != end(); }

private:
    std::array<Move, MAX_MOVES> moves;
    size_t move_count = 0;
};

void GenerateMovesQuiet(MoveList &moves, const Board &board, Color color) noexcept;
void GenerateMovesTactical(MoveList &moves, const Board &board, Color color) noexcept;
void GenerateMovesAll(MoveList &moves, const Board &board, Color color) noexcept;
MoveList GenerateMovesAll(const Board &board, Color color) noexcept;
MoveList GenerateLegalMoves(const Board &board, Color color) noexcept;
