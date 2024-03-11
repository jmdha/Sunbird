#pragma once

#include "move.hpp"
#include "position.hpp"
#include "types.hpp"

// Class representing the current, and historic, state of a game of chess
class Board {
public:
    // constuctor

    Board(Position position);

    // access

    inline size_t MoveCount() const noexcept;
    inline size_t Ply() const noexcept;
    inline const Position &Pos() const noexcept;
    bool IsThreefoldRepetition() const noexcept;

    // modifiers

    void MakeMove(const Move &move) noexcept;
    void UndoMove() noexcept;

private:
    std::array<Position, MAX_PLY> positions; // The current, and prior, state of the board
    size_t current_ply = 0;                  // How many moves deep the current game is
    size_t _moves; // How many moves have been applied to the board, including those that are undone
};

inline size_t Board::MoveCount() const noexcept { return _moves; }

inline size_t Board::Ply() const noexcept { return current_ply; }

inline const Position &Board::Pos() const noexcept { return positions.at(current_ply - 1); }
