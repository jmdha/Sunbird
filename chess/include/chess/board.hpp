#ifndef CHESS_BOARD
#define CHESS_BOARD

#include <cassert>
#include <string>

#include "chess/internal/move.hpp"
#include "internal/bitboard.hpp"
#include "internal/constants.hpp"
#include "internal/position.hpp"
#include "jank/container/fixed_stack.hpp"

namespace Chess {
// Class representing the current, and historic, state of a game of chess
class Board {
public:

    // constuctor

    Board(Position position);

    // access
    
    const Position& Pos() const noexcept;

    // modifiers
    
    void MakeMove(const Move &move) noexcept;
    void UndoMove() noexcept;

private:
    jank::container::fixed_stack<Position, MAX_PLY> _positions;
};
} // namespace Chess

#endif // CHESS_BOARD
