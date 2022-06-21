#ifndef MOVE
#define MOVE

#include "constants.hh"

class Move {
public:
    Square fromSquare;
    Square toSquare;
    PieceType fromType;
    PieceType toType;
    Color fromColor;
    Color toColor;

    Move(Square fromSquare, Square toSquare, PieceType fromType, PieceType toType, Color fromColor, Color toColor) {
        this->fromSquare = fromSquare;
        this->toSquare = toSquare;
        this->fromType = fromType;
        this->toType = toType;
        this->fromColor = fromColor;
        this->toColor = toColor;
    }
private:
};
#endif