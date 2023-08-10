#ifndef CHESS_ZOBRIST
#define CHESS_ZOBRIST

#include <unordered_map>
#include <tsl/robin_map.h>

#include "constants.hpp"

namespace Chess {
class Zobrist {
public:
    void FlipSquare(Square square, PieceType type, Color color);
    void FlipCastling(Color col, Castling side);
    void FlipEnPassant(Column col);
    inline void IncHash() { hashOccurances[hash]++; }
    inline void DecHash() {
        if (hashOccurances.at(hash)-- == 1)
            hashOccurances.erase(hash);
    }
    inline bool IsThreefoldRep() const { return hashOccurances.at(hash) > 2; }
    inline uint64_t GetHash() const { return hash; }

private:
    uint64_t hash = 0;
    tsl::robin_map<uint64_t, int> hashOccurances;
};
} // namespace Chess

#endif // CHESS_ZOBRIST
