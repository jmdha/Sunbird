#include "Headers/queen.h"

Queen::Queen(Color color) : Piece(color) {
	this->color = color;
}

PieceChar Queen::GetPieceChar() {
	if(this->color == Color::White)
		return PieceChar::QueenWhite;
	else
		return PieceChar::QueenBlack;
}

std::vector<Move> Queen::GetPossibleMoves(Board board) {
	std::vector<Move> moves;

	std::vector<Move>::iterator it;
	it = moves.begin();
	std::vector<Move> tempMoveList;

	for(int i = 1; i <= 9; i++) {
		if(i == 5)
			continue;
		tempMoveList = this->GetPossibleMovesInDirection(board, (Direction) i, 8);
		moves.insert(it, tempMoveList.begin(), tempMoveList.end());
		it = moves.begin();
	}

	return moves;
}

// does not check for castling
Move Queen::GetMoveIfPossible(Board board, Point endPos) {
	std::vector<Move> moves;

	for(int i = 1; i <= 9; i++) {
		if(i == 5)
			continue;
		moves = this->GetPossibleMovesInDirection(board, (Direction) i, 8);
		for(int i = 0; i < (int) moves.size(); i++) {
			if(moves[i].endX == endPos.x && moves[i].endY == endPos.y)
				return moves[i];
		}
	}

	return Move(-1, -1, -1, -1);
}

int Queen::GetValue() {
	return (int) Value::Queen + ValueQueenPos[x][(this->color == Color::White) ? y : HEIGHT - 1];
}

int Queen::GetIndex() {
	return (int) PieceIndex::Queen;
}

bool Queen::CheckIfPosPossible(Board board, Point pos) {
	int xDir;
	int yDir;
	if(x > pos.x)
		xDir = -1;
	else if(x < pos.x)
		xDir = 1;
	else
		xDir = 0;
	if(y > pos.y)
		yDir = -1;
	else if(y < pos.y)
		yDir = 1;
	else
		yDir = 0;

	for(int i = 1; i < 8; i++) {
		int newX = x + i * xDir;
		int newY = y + i * yDir;
		if(pos.x == newX && pos.y == newY)
			return true;
		else if(board.GetPiece(newX, newY) != NULL)
			return false;
	}

	return false;
}
