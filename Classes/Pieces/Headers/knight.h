#ifndef Knight_H
#define Knight_H

#include "../../Headers/piece.h"
#include "../../Headers/move.h"

class Knight : public Piece {
public:
	Knight(Color color);
	PieceChar GetPieceChar();
	std::vector<Move> GetPossibleMoves(Board board);
	Move GetMoveIfPossible(Board board, Point endPos);
	int GetValue();
	int GetIndex();
	bool CheckIfPosPossible(Board board, Point pos);

private:

};

#endif