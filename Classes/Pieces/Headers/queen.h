#ifndef Queen_H
#define Queen_H

#include "../../Headers/piece.h"
#include "../../Headers/move.h"

class Queen : public Piece {
public:
	Queen(Color color);
	PieceChar GetPieceChar();
	std::vector<Move> GetPossibleMoves(Board board);
	Move GetMoveIfPossible(Board board, Point endPos);
	int GetValue();
	int GetIndex();
	bool CheckIfPosPossible(Board board, Point pos);

private:

};

#endif