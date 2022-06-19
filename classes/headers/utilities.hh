#ifndef UTILITIES
#define UTILITIES

#include <string>
#include <stdexcept>

#include "constants.hh"

class Utilities {
public:
    static PieceChar GetPieceChar(char c);
    static PieceChar GetPieceChar(PieceType type, Color color);
    static PieceType GetPieceType(PieceChar pieceChar);
    static Color GetPieceColor(PieceChar pieceChar);
    static Square GetSquare(int x, int y);
    static int GetPieceValue(PieceType type);

    // Returns the amount of bits set to 1
    // e.g. 0110 => 2 and 1000 => 1
    static int PopCount(U64 x);

private:
};

#endif