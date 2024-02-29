#include "zobrist.hpp"

// Generate hashses in a pseudo-random way
// Cannot use *actual* randomness as its compile time
// This is, however, good enough
constexpr std::array<uint64_t, HASH_COUNT> HASHES = [] {
    auto tempTable = decltype(HASHES){};

    uint64_t lfsr = 0x181818ffff181818;
    uint64_t bit;

    for (int i = 0; i < HASH_COUNT; i++) {
        bit          = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1u;
        lfsr         = (lfsr >> 1) | (bit << 63);
        tempTable[i] = lfsr;
    }

    return tempTable;
}();

void FlipTurn(Hash &hash) { hash ^= 0xaa55aa55aa55aa55; }
void FlipEnpassant(Hash &hash, Square sq) { hash ^= HASHES[HASH_COUNT + 4 + sq]; }
void FlipSquare(Hash &hash, Color color, Piece piece_type, Square square) {
    hash ^= HASHES[(64 * 6) * (int)color + 64 * piece_type + square];
}
