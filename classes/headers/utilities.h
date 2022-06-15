#ifndef UTILITIES
#define UTILITIES

#include <string>
#include <stdexcept>

#include "constants.h"

class Utilities {
public:
    static PieceChar GetPieceChar(char c);
    static PieceType GetPieceType(PieceChar pieceChar);
    static Color GetPieceColor(PieceChar pieceChar);
    static Square GetSquare(int x, int y);
private:
};

#endif