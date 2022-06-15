#ifndef KING
#define KING

#include "../../headers/piece.h"

class King : public Piece {
public:
    King(const Color _color, const int _xPos, const int _yPos);
    PieceType GetType() { return PieceType::King; };
    int GetValue() { return (int) PieceValue::King; };
private:

};

#endif