#ifndef CHESS_ZOBRIST
#define CHESS_ZOBRIST

#include <unordered_map>

#include "constants.hpp"

namespace Chess {
class Zobrist {
public:
    void FlipSquare(Square square, PieceType type, Color color);
    void FlipCastling(Color col, Castling side);
    void FlipEnPassant(Column col);
    void IncHash();
    void DecHash();
    bool IsThreefoldRep() const;
    U64 GetHash() const;

private:
    U64 hash = 0;
    std::unordered_map<U64, U8> hashOccurances;
};
} // namespace Chess

#endif // CHESS_ZOBRIST
