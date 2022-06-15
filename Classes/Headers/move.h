#ifndef Move_H
#define Move_H

class Piece;

#include <string>
#include "piece.h"



class Move {

public:
	int startX;
	int startY;
	int endX;
	int endY;
	int value = -1;
	// Used for debugging
	int totalMoves = 0;
	int moveSizeBatch = 0;
	// The state of enpassant before the move
	int priorEnPassant = -1;

	// depth at which was found
	int moveDepth = -1;

	Piece* target;
	bool castling = false;
	// if this move is the one to disallow castling on a side
	bool disallowedCastling[2] = {false, false};
	// first move of a piece
	bool firstMove = false;
	bool promotion = false;
	PieceIndex promotionType;
	bool pawnDoubleMove = false;
	bool enPassantTake = false;
	Move();
	Move(int startX, int startY, int endX, int endY);
	Move(std::string move);
	std::string GetMoveAsString();
	std::string GetMoveAsPlainString();
	void SetTarget(Piece* target);
private:
};

#endif