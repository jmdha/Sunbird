#ifndef CHESS_ZOBRIST
#define CHESS_ZOBRIST

#include "types.hpp"

namespace Zobrist {
uint64_t FlipSquare(uint64_t hash, Square square, PieceType type, Color color);
uint64_t FlipCastling(uint64_t hash, Color col, Castling side);
uint64_t FlipEnPassant(uint64_t hash, Column col);
uint64_t FlipColor(uint64_t hash);
} // namespace Zobrist

#endif // CHESS_ZOBRIST
