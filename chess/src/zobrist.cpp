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
template <U64 l>
constexpr std::array<U64, l> GenerateHashes() {
    std::array<U64, l> tempTable;

    for (int i = 0; i < l; i++) {
        U64 hash = i;
        hash ^= (i ^ 2) << 17;
        tempTable.at(i) = hash;
    }

    return tempTable;
}
constexpr std::array<U64, SQUARE_HASH_COUNT> hashes = GenerateHashes<SQUARE_HASH_COUNT>();
} // namespace

void Zobrist::FlipSquare(Square square, PieceType type, Color color) {
    hash ^= hashes[(64 * 6) * (int)color + 64 * (int)type + (int)square];
}

void Zobrist::FlipCastling(Color col, Castling side) {
    hash ^= hashes[SQUARE_HASH_COUNT + 2 * (int)col + (int)side];
}

void Zobrist::FlipEnPassant(Column col) {
    hash ^= hashes[SQUARE_HASH_COUNT + 4 + Utilities::GetColumnIndex(col)];
}

void Zobrist::IncHash() { ++hashOccurances[hash]; }

void Zobrist::DecHash() {
    if (--hashOccurances.at(hash) == 0)
        hashOccurances.erase(hash);
}

bool Zobrist::IsThreefoldRep() const { return hashOccurances.at(hash) > 2; }

U64 Zobrist::GetHash() const { return hash; }
