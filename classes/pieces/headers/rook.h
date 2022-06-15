#ifndef ROOK
#define ROOK

#include "../../headers/piece.h"

class Rook : public Piece {
public:
    Rook(const Color _color, const int _xPos, const int _yPos);
    PieceType GetType() { return PieceType::Rook; };
    int GetValue() { return (int) PieceValue::Rook; };
private:

};

#endif