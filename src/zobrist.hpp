#pragma once

#include "types.hpp"

namespace Zobrist {
uint64_t FlipSquare(uint64_t hash, Square square, Piece type, Color color);
uint64_t FlipCastling(uint64_t hash, Color col, Castling side);
uint64_t FlipEnPassant(uint64_t hash, Square sq);
uint64_t FlipColor(uint64_t hash);
} // namespace Zobrist
