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
    
    inline const Position& Pos() const noexcept;
    bool IsThreefoldRepetition() const noexcept;

    // modifiers
    
    void MakeMove(const Move &move) noexcept;
    void UndoMove() noexcept;

private:
    jank::container::fixed_stack<Position, MAX_PLY> _positions;
};

inline const Position& Board::Pos() const noexcept {
    return _positions.top();
}

} // namespace Chess

#endif // CHESS_BOARD
