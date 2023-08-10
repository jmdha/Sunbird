#include <chess/internal/utilities.hpp>
#include <chess/internal/zobrist.hpp>

#include <array>
#include <climits>
#include <cassert>
#include <random>

using namespace Chess;

namespace {
constexpr int SQUARE_HASH_COUNT = 2 * 6 * 64;
constexpr int HASH_COUNT = SQUARE_HASH_COUNT + 4 * 8;
template <uint64_t l>
constexpr std::array<uint64_t, l> GenerateHashes() {
    std::array<uint64_t, l> tempTable;

    uint64_t startState = 0x181818ffff181818;
    uint64_t lfsr = startState;
    uint64_t bit;
    uint64_t period = 0;

    for (int i = 0; i < l; i++) {
        bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1u;
        lfsr = (lfsr >> 1) | (bit << 63);
        tempTable[i] = lfsr;
    }

    return tempTable;
}
constexpr std::array<uint64_t, SQUARE_HASH_COUNT> hashes = GenerateHashes<SQUARE_HASH_COUNT>();
} // namespace

uint64_t Zobrist::FlipSquare(uint64_t hash, Square square, PieceType type, Color color) {
    return hash ^ hashes[(64 * 6) * (int)color + 64 * (int)type + (int)square];
}

uint64_t Zobrist::FlipCastling(uint64_t hash, Color col, Castling side) {
    return hash ^ hashes[SQUARE_HASH_COUNT + 2 * (int)col + (int)side];
}

uint64_t Zobrist::FlipEnPassant(uint64_t hash, Column col) {
    return hash ^ hashes[SQUARE_HASH_COUNT + 4 + Utilities::GetColumnIndex(col)];
}
