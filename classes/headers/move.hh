#ifndef MOVE
#define MOVE

#include "constants.hh"

class Move {
public:
    Square fromSquare;
    Square toSquare;
    Color fromColor;
    Color toColor;

    Move(Square fromSquare, Square toSquare, Color fromColor, Color toColor) {
        this->fromSquare = fromSquare;
        this->toSquare = toSquare;
        this->fromColor = fromColor;
        this->toColor = toColor;
    }
private:
};
#endif
