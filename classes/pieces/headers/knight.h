#ifndef KNIGHT
#define KNIGHT

#include "../../headers/piece.h"

class Knight : public Piece {
public:
    Knight(const Color _color, const int _xPos, const int _yPos);
    PieceType GetType() { return PieceType::Knight; };
    int GetValue() { return (int) PieceValue::Knight; };
private:

};

#endif