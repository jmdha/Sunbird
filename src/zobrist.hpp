#pragma once

#include "types.hpp"

namespace Zobrist {
void FlipSquare(uint64_t &hash, Square square, Piece type, Color color);
void FlipCastling(uint64_t &hash, Color col, Castling side);
void FlipEnPassant(uint64_t &hash, Square sq);
void FlipColor(uint64_t &hash);
} // namespace Zobrist
