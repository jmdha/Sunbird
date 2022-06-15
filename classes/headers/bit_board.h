#ifndef BITBOARD
#define BITBOARD

#define U64 unsigned long long int

#include <string>

#include "board.h"
#include "constants.h"
#include "utilities.h"

class BitBoard : Board {
public:
    BitBoard();
    void ClearBoard();
    void Initialize();
    void ImportFEN(std::string FEN);
    void MovePiece(const int startX, const int startY, const int endX, const int endY);
private:
    U64 pieceBB[PIECECOUNT];
    U64 colorBB[COLORCOUNT];
    U64 occupiedBB;
    U64 emptyBB;

    void PlacePiece(Square square, PieceType type, Color color);
    void PlacePiece(Square square, PieceChar pieceChar);
};
#endif