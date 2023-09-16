#ifndef CHESS_BOARD
#define CHESS_BOARD

#include <cassert>
#include <string>

#include "internal/bitboard.hpp"
#include "internal/constants.hpp"
#include "internal/move.hpp"
#include "internal/position.hpp"

namespace Chess {
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
    std::array<Position, MAX_PLY> positions;
    size_t positionCount = 0;
    size_t _moves;
};

inline size_t Board::MoveCount() const noexcept { return _moves; }

inline size_t Board::Ply() const noexcept { return positionCount; }

inline const Position &Board::Pos() const noexcept { return positions.at(positionCount - 1); }

} // namespace Chess

#endif // CHESS_BOARD
