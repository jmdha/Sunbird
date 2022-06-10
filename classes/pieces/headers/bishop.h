#ifndef BISHOP
#define BISHOP

#include "../../headers/piece.h"

class Bishop : public Piece {
public:
    Bishop(const Color _color, const int _xPos, const int _yPos);
    PieceType GetType() { return PieceType::Bishop; };
    int GetValue() { return (int) PieceValue::Bishop; };
private:

};

#endif