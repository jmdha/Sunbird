#ifndef MOVE
#define MOVE

#include "constants.hh"

class Move {
public:
    Square fromSquare;
    Square toSquare;
    Color fromColor;
    Color toColor;
    PieceType fromType;
    PieceType toType;
    Move(){}
    Move(Square fromSquare, Square toSquare, Color fromColor, Color toColor, PieceType fromType, PieceType toType) {
        this->fromSquare = fromSquare;
        this->toSquare = toSquare;
        this->fromColor = fromColor;
        this->toColor = toColor;
        this->fromType = fromType;
        this->toType = toType;
    }
private:
};
#endif
