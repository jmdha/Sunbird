#ifndef BITBOARD
#define BITBOARD

#include <string>

#include "constants.hh"
#include "utilities.hh"

class BitBoard {
public:
    BitBoard();
    Color GetColor() {return color; };
    void SetColor(Color color) { this->color = color; };
    void ClearBoard();
    void Initialize();
    PieceChar GetPiece(Square square);
    void PlacePiece(Square square, PieceChar pieceChar);

private:
    Color color;
    U64 pieceBB[PIECECOUNT];
    U64 colorBB[COLORCOUNT];
    U64 occupiedBB;

    void PlacePiece(Square square, PieceType type, Color color);
    
    friend class Evaluator;
    friend class MoveGen;
};
#endif