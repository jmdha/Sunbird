#ifndef BITBOARD
#define BITBOARD

#include <string>

#include "constants.hh"
#include "utilities.hh"
#include "move.hh"

class BitBoard {
public:
    Color originalColor;
    U64 totalMoves = 0;
    
    BitBoard();
    Color GetColor() {return color; };
    Color GetColor(Square sq);
    void SetColor(Color color) { this->color = color; };
    void ClearBoard();
    void Initialize();
    PieceType GetPiece(Square square);
    Color GetSquareColor(Square square);
    void PlacePiece(Square square, PieceChar pieceChar);
    void DoMove(Move move);
    void UndoMove(Move move);
    U64 GetHash() { return occupiedBB; };

    void EnableCastling(Color color, Castling side);
    void DisableCastling(Color color, Castling side);
    void IncrementCastling();
    void DecrementCastling();

private:
    
    Color color;
    U64 pieceBB[PIECECOUNT];
    U64 colorBB[COLORCOUNT];
    U64 occupiedBB;
    U64 enPassant;
    int popCount[COLORCOUNT][PIECECOUNT] = {};
    bool castlingAllowed[2][2];
    int movesSinceCastlingDisallowed[2][2];

    void PlacePiece(Square square, PieceType type, Color color);
    void RemovePiece(Square square, PieceType type, Color color);
    
    PieceType GetType(Square square, Color color);

    friend class Evaluator;
    friend class MoveGen;
};
#endif
