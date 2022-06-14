#ifndef Bishop_H
#define Bishop_H

#include "../../Headers/Piece.h"
#include "../../Headers/Move.h"

class Bishop : public Piece {
public:
	Bishop(Color color);
	PieceChar GetPieceChar();
	std::vector<Move> GetPossibleMoves(Board board);
	Move GetMoveIfPossible(Board board, Point endPos);
	int GetValue();
	int GetIndex();
	bool CheckIfPosPossible(Board board, Point pos);

private:

};

#endif