#ifndef ZOBRIST
#define ZOBRIST

#include <array>
#include <random>
#include <climits>
#include <unordered_map>
#include <map>

#include "constants.hh"
#include "../../include/robin-map/robin_map.h"

struct Zobrist {
public:
    Zobrist() {
        InitHashTable();
    }
    inline void FlipSquare(Square square, PieceType type, Color color);
    inline void FlipCastling(Color col, Castling side);
    inline void FlipEnPassant(Column col);
    inline void IncrementHash();
    inline void DecrementHash();
    inline bool IsThreefoldRep() const;
    inline U64 GetHash() const;

private:
    // Static
    U64 hashTable[64][PIECECOUNT][COLORCOUNT]{};
    U64 castlingHashes[2][2]{};
    U64 enPassantHashes[8]{};

    U64 hash = 0;
    tsl::robin_map<U64, U8> hashOccurances;

    void InitHashTable() {
        std::mt19937 gen(0);
	    std::uniform_int_distribution<unsigned long long> distribution(0, ULLONG_MAX);

        for (int i = 0; i < WIDTH * HEIGHT; ++i)
            for (int j = 0; j < PIECECOUNT; ++j)
                for (int t = 0; t < COLORCOUNT; ++t)
                    hashTable[i][j][t] = distribution(gen);
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                castlingHashes[i][j] = distribution(gen);
        for (int i = 0; i < 8; i++)
            enPassantHashes[i] = distribution(gen);
    }
};

inline void Zobrist::FlipSquare(Square square, PieceType type, Color color) {
    hash ^= hashTable[(int) square][(int) type][(int) color];
}

inline void Zobrist::FlipCastling(Color col, Castling side) {
    hash ^= castlingHashes[(int) col][(int) side];
}

inline void Zobrist::FlipEnPassant(Column col) {
    hash ^= enPassantHashes[Utilities::GetColumnIndex(col)];
}


inline void Zobrist::IncrementHash() {
    ++hashOccurances[hash];
}

inline void Zobrist::DecrementHash() {
    if (--hashOccurances.at(hash) == 0)
        hashOccurances.erase(hash);
}

inline bool Zobrist::IsThreefoldRep() const {
    return hashOccurances.at(hash) >= 3;
}

inline U64 Zobrist::GetHash() const {
    return hash;
}

#endif // ZOBRIST
