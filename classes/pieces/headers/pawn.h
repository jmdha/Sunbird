#ifndef PAWN
#define PAWN

#include "../../headers/piece.h"

class Pawn : public Piece {
public:
    Pawn(const Color _color, const int _xPos, const int _yPos);
    PieceType GetType() { return PieceType::Pawn; };
    int GetValue() { return (int) PieceValue::Pawn; };
private:

};

#endif