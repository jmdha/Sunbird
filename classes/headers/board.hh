#ifndef BOARD
#define BOARD

#include <string>

#include "constants.hh"

class Board {
public:
    Board();
    void SetColor(Color color) { this->color = color; };

    virtual void ClearBoard() = 0;
    virtual void Initialize() = 0;
    virtual PieceChar GetPiece(Square square) = 0;
    virtual void PlacePiece(Square square, PieceChar pieceChar) = 0;
    virtual void MovePiece(const int startX, const int startY, const int endX, const int endY) = 0;

private:
    Color color;

};
#endif
