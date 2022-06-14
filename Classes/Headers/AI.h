#ifndef Ai_H
#define Ai_H

#include <vector>
#include <chrono>
#include <math.h>

#include "Move.h"
#include "Point.h"
#include "Board.h"
#include "Constants.h"

Move GetValidMove(Board board, Point endPos, PieceIndex promotionType, PieceChar pieceChar);
Move GetValidMove(Board board, Point endPos, PieceChar pieceChar);
Move GetValidMove(Board board, Point endPos, PieceChar pieceChar, int column);
Move GetValidMove(Board board, Point endPos, PieceChar pieceChar, int column, PieceIndex promotionType);
Move GetValidMove(Board board, Point endPos, int row, PieceChar pieceChar);
Move GetValidMove(Board board, Point endPos, int row, PieceChar pieceChar, int column);
std::vector<Move> GetAllMoves(Board board, Color side);
std::vector<Move> GetAllMovesOfPieceChar(Board board, PieceChar pieceChar);
std::vector<Move> GetAllMovesOfPieceChar(Board board, PieceChar pieceChar, int startCol);
std::vector<Move> GetAllMovesOfPieceChar(Board board, int startRow, PieceChar pieceChar);
std::vector<Move> GetAllMovesOfPieceChar(Board board, PieceChar pieceChar, int startCol, int startRow);

Move GetBestMove(Board* board, int maxTime);
Move GenerateMove(Board* board, int depth, int& totalMoves);
Move Minimax(Board* board, int depth, bool isMax, Color currentTurn, int a, int b, bool doingHE, int* totalMoves, int accDepth);

#endif