#ifndef BITBOARD
#define BITBOARD

#include <string>

#include "constants.hh"
#include "utilities.hh"
#include "move.hh"

class BitBoard {
public:
    int totalMoves = 0;

    BitBoard();
    Color GetColor() {return color; };
    void SetColor(Color color) { this->color = color; };
    void ClearBoard();
    void Initialize();
    PieceType GetPiece(Square square);
    Color GetSquareColor(Square square);
    void PlacePiece(Square square, PieceChar pieceChar);
    void DoMove(Move move);
    void UndoMove(Move move);
    U64 GetHash() { return occupiedBB; };

private:
    Color color;
    U64 pieceBB[PIECECOUNT];
    U64 colorBB[COLORCOUNT];
    U64 occupiedBB;
    int popCount[COLORCOUNT][PIECECOUNT] = {};

    void PlacePiece(Square square, PieceType type, Color color);
    void RemovePiece(Square square, PieceType type, Color color);
    
    PieceType GetType(Square square, Color color);

    friend class Evaluator;
    friend class MoveGen;
};
#endif
