#ifndef BOARD_OO
#define BOARD_OO

#include <vector>
#include <string>
#include <stdexcept>

#include "board.h"
#include "piece.h"
#include "../pieces/headers/pawn.h"
#include "../pieces/headers/knight.h"
#include "../pieces/headers/bishop.h"
#include "../pieces/headers/rook.h"
#include "../pieces/headers/queen.h"
#include "../pieces/headers/king.h"
#include "constants.h"

class Board_OO : public Board {
public:
    Board_OO();
    void Initialize();
    void ImportFEN(std::string FEN);
    Piece* GetPiece(const int x, const int y);
    
    void MovePiece(const int startX, const int startY, const int endX, const int endY);
private:
    Piece* pieces[WIDTH][HEIGHT];
    std::vector<Piece*> whitePieces = std::vector<Piece*>();

    void PlacePiece(Piece* piece, const int x, const int y);
    void PlacePiece(const PieceChar piece, const int x, const int y);
    Piece* RemovePiece(const int x, const int y);
};
#endif