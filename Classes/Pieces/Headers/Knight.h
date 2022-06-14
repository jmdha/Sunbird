#ifndef Knight_H
#define Knight_H

#include "../../Headers/Piece.h"
#include "../../Headers/Move.h"

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