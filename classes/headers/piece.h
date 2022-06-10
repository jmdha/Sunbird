#ifndef PIECE
#define PIECE

#include "../headers/constants.h"

class Piece {
public:
    Color color;
    Piece(const Color _color, const int xPos, const int yPos);
    virtual PieceType GetType() = 0; 
    virtual int GetValue() = 0;    
private:
    int xPos;
    int yPos;

};

#endif