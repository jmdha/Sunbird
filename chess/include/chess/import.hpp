#ifndef CHESS_IMPORT
#define CHESS_IMPORT

#include <functional>
#include <string>

#include "board.hpp"

namespace Chess::Import {
/**
 * Creates a board and sets the state to that of the given FEN string
 * https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
 */
Board FEN(std::string FEN);

/**
 * Creates a board and imports a series of moves in sequence
 * Each move consists of the origin square and end square, e.g. e2e4 - moving a piece from e2 to e4
 * The string is then of the format "e2e4 d7d5 ..."
 */
Board MoveSequence(std::string moves);
void MoveSequence(Board& board, std::string moves);

/**
 * Creates a board and sets it's state to that resulting from the algebraic sequence of moves
 * https://en.wikipedia.org/wiki/Algebraic_notation_(chess)
 */
Board Algebraic(std::string game);

/**
 * Creates a board and call the callback with boardstate resulting from each algebraic move
 */
void Algebraic(std::string game, std::function<void(const Board &)> callback);
} // namespace Chess::Import

#endif
