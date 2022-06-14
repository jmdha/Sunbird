#ifndef King_H
#define King_H

#include "../../Headers/Piece.h"
#include "../../Headers/Move.h"

class King : public Piece {
public:
	King(Color color);
	PieceChar GetPieceChar();
	std::vector<Move> GetPossibleMoves(Board board);
	Move GetMoveIfPossible(Board board, Point endPos);
	int GetValue();
	int GetIndex();
	bool CheckIfPosPossible(Board board, Point pos);


private:
	bool getCastlingPossibility(Board board, CastlingDirection direction);

};

#endif