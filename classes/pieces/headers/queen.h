#ifndef QUEEN
#define QUEEN

#include "../../headers/piece.h"

class Queen : public Piece {
public:
    Queen(const Color _color, const int _xPos, const int _yPos);
    PieceType GetType() { return PieceType::Queen; };
    int GetValue() { return (int) PieceValue::Queen; };
private:

};

#endif