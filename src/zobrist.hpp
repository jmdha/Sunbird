#pragma once

#include "types.hpp"
#include <array>
#include <cstddef>

// Consists of turn key, castling rights, EP squares, Piece squares
constexpr int HASH_COUNT = 2 * 6 * 64;
extern const std::array<Hash, HASH_COUNT> HASHES;

void FlipTurn(Hash &hash);
void FlipCastle(Hash &hash, Color color, Castling castling);
void FlipEnpassant(Hash &hash, Square sq);
void FlipSquare(Hash &hash, Color color, Piece piece_type, Square square);
