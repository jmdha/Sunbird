#include <array>
#include <chess/internal/utilities.hpp>
#include <chess/internal/zobrist.hpp>

using namespace Chess;

constexpr int SQUARE_HASH_COUNT = 2 * 6 * 64;
// Generate hashses in a pseudo-random way
// Cannot use *actual* randomness as its compile time
// This is, however, good enough
constexpr std::array<uint64_t, SQUARE_HASH_COUNT> HASHES = [] {
    auto tempTable = decltype(HASHES){};

    uint64_t lfsr = 0x181818ffff181818;
    uint64_t bit;

    for (int i = 0; i < SQUARE_HASH_COUNT; i++) {
        bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1u;
        lfsr = (lfsr >> 1) | (bit << 63);
        tempTable[i] = lfsr;
    }

    return tempTable;
}();

uint64_t Zobrist::FlipSquare(uint64_t hash, Square square, PieceType type, Color color) {
    return hash ^ HASHES[(64 * 6) * (int)color + 64 * (int)type + (int)square];
}

uint64_t Zobrist::FlipCastling(uint64_t hash, Color col, Castling side) {
    return hash ^ HASHES[SQUARE_HASH_COUNT + 2 * (int)col + (int)side];
}

uint64_t Zobrist::FlipEnPassant(uint64_t hash, Column col) {
    return hash ^ HASHES[SQUARE_HASH_COUNT + 4 + Utilities::GetColumnIndex(col)];
}
uint64_t Zobrist::FlipColor(uint64_t hash) { return hash ^ 0xaa55aa55aa55aa55; }
