#ifndef Piece_H
#define Piece_H

class Board;
class Move;

#include "constants.h"
#include "move.h"
#include "board.h"
#include "point.h"
#include <vector>


class Piece {

public:
	Color color;
	int x;
	int y;
	bool hasMoved = false;

	Piece(Color color);
	virtual PieceChar GetPieceChar();
	virtual std::vector<Move> GetPossibleMoves(Board board);
	virtual Move GetMoveIfPossible(Board board, Point endPos);
	virtual int GetValue();
	virtual int GetIndex();
	std::vector<Move> GetPossibleMovesInDirection(Board board, Direction direction, int length);
	std::vector<Move> GetPossibleMovesInDirection(Board board, Direction direction, int length, bool onlyCapture);
	virtual bool CheckIfPosPossible(Board board, Point pos);
private:
	Point GetNewEndPoint(Point priorPos, Direction direction, int moveLength);


};

#endif