#ifndef UTILITIES
#define UTILITIES

#include <strings.h>
#include <stdexcept>
#include <math.h>
#include <bitset>

#include "constants.hh"

class Utilities {
public:
    static PieceChar GetPieceChar(char c);
    static PieceChar GetPieceChar(PieceType type, Color color);
    static PieceType GetPieceType(PieceChar pieceChar);
    static Color GetPieceColor(PieceChar pieceChar);
    static Square GetSquare(int x, int y);
    static Square GetSquare(char row, char column);
    static std::string GetSquareString(Square sq);
    static int GetPieceValue(PieceType type);
    static Color GetOppositeColor(Color color);

    // Returns the amount of bits set to 1
    // e.g. 0110 => 2 and 1000 => 1
    static int PopCount(U64 x);

    static int LSB(U64 x);

    // Returns the least significant set bit, and pops it
    // e.g. 0110 => 1 and 1000 => 3 and 1001 => 0
    static int LSB_Pop(U64* x);

    static int MSB(U64 x);

    static U64 NotEdge(Direction dir);

    static Column GetColumn(int columnIndex);
    static Column GetColumn(Square square);
    static int GetColumnIndex(Square square);

    static Row GetRow(int rowIndex);
    static Row GetRow(Square square);
    static int GetRowIndex(Square square);

    // Converts 1-8 to bitshift equivilant
    static Direction GetDirection(int i);

private:
};

#endif
