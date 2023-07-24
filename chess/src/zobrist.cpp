#include <chess/internal/zobrist.hpp>
#include <chess/internal/utilities.hpp>

#include <array>
#include <climits>
#include <random>

using namespace Chess;

namespace {
constexpr int HASH_COUNT = 2 * 6 *64 + 4 + 8;
constexpr std::array<U64, HASH_COUNT> GenerateHashes() {
    std::array<U64, HASH_COUNT> tempTable;

    return tempTable;
}
constexpr std::array<U64, HASH_COUNT> hashTable{GenerateHashes()};
}

void Zobrist::FlipSquare(Square square, PieceType type, Color color) {
    //hash ^= hashTable[(int) square][(int) type][(int) color];
}

void Zobrist::FlipCastling(Color col, Castling side) {
    //hash ^= castlingHashes[(int) col][(int) side];
}

void Zobrist::FlipEnPassant(Column col) {
    //hash ^= enPassantHashes[Utilities::GetColumnIndex(col)];
}

void Zobrist::IncHash() {
    ++hashOccurances[hash];
}

void Zobrist::DecHash() {
    if (--hashOccurances.at(hash) == 0)
        hashOccurances.erase(hash);
}

bool Zobrist::IsThreefoldRep() const {
    return hashOccurances.at(hash) > 2;
}

U64 Zobrist::GetHash() const {
    return hash;
}
