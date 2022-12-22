#ifndef ZOBRIST
#define ZOBRIST

#include <array>
#include <random>
#include <climits>
#include <unordered_map>

#include "constants.hh"

struct Zobrist {
public:
    Zobrist() {
        InitHashTable();
    }
    inline void FlipSqure(Square square, PieceType type, Color color);
    inline void IncrementHash();
    inline void DecrementHash();
    inline bool IsThreefoldRep() const;
    inline U64 GetHash() const;

private:
    // Static
    U64 hashTable[64][PIECECOUNT][COLORCOUNT];

    U64 hash = 0;
    std::unordered_map<U64, U8> hashOccurances;
    

    void InitHashTable() {
        std::mt19937 gen(0);
	    std::uniform_int_distribution<unsigned long long> distribution(0, ULLONG_MAX);

        for (int i = 0; i < WIDTH * HEIGHT; i++)
            for (int j = 0; j < PIECECOUNT; j++)
                for (int t = 0; t < COLORCOUNT; t++)
                    hashTable[i][j][t] = distribution(gen);
    }
};

inline void Zobrist::FlipSqure(Square square, PieceType type, Color color) {
    auto value = hashTable[(int) square][(int) type][(int) color];
    hash ^= value;
}

inline void Zobrist::IncrementHash() {
    hashOccurances[hash]++;
}

inline void Zobrist::DecrementHash() {
    hashOccurances.at(hash)--;
    if (hashOccurances.at(hash) == 0)
        hashOccurances.erase(hash);
}

inline bool Zobrist::IsThreefoldRep() const {
    return hashOccurances.at(hash) >= 3;
}

inline U64 Zobrist::GetHash() const {
    return hash;
}

#endif // ZOBRIST
