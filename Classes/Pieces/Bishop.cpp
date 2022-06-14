#include "Headers/Bishop.h"

Bishop::Bishop(Color color) : Piece(color) {
	this->color = color;
}

PieceChar Bishop::GetPieceChar() {
	if(this->color == Color::White)
		return PieceChar::BishopWhite;
	else
		return PieceChar::BishopBlack;
}

std::vector<Move> Bishop::GetPossibleMoves(Board board) {
	std::vector<Move> moves;

	std::vector<Move>::iterator it;
	it = moves.begin();
	std::vector<Move> tempMoveList;

	for(int i = 0; i < 4; i++) {

		Direction dir = Direction::NorthWest;
		switch(i) {
			case 0:
			dir = Direction::NorthEast;
			break;

			case 1:
			dir = Direction::SouthWest;
			break;

			case 2:
			dir = Direction::SouthEast;
			break;
		}
		tempMoveList = this->GetPossibleMovesInDirection(board, dir, 8);
		moves.insert(it, tempMoveList.begin(), tempMoveList.end());
		it = moves.begin();
	}


	return moves;
}

Move Bishop::GetMoveIfPossible(Board board, Point endPos) {
	std::vector<Move> moves;

	for(int i = 0; i < 4; i++) {

		Direction dir = Direction::NorthWest;
		switch(i) {
			case 0:
			dir = Direction::NorthEast;
			break;

			case 1:
			dir = Direction::SouthWest;
			break;

			case 2:
			dir = Direction::SouthEast;
			break;
		}
		moves = this->GetPossibleMovesInDirection(board, dir, 8);
		for(int i = 0; i < static_cast<int>(moves.size()); i++) {
			if(moves[i].endX == endPos.x && moves[i].endY == endPos.y)
				return moves[i];
		}
	}
	return Move(-1, -1, -1, -1);
}

int Bishop::GetValue() {
	return static_cast<int>(Value::Bishop) + ValueBishopPos[x][(this->color == Color::White) ? y : HEIGHT - 1];
}

int Bishop::GetIndex() {
	return static_cast<int>(PieceIndex::Bishop);
}

bool Bishop::CheckIfPosPossible(Board board, Point pos) {
	int xDir;
	int yDir;
	if(x > pos.x)
		xDir = -1;
	else if(x < pos.x)
		xDir = 1;
	else
		return false;
	if(y > pos.y)
		yDir = -1;
	else if(y < pos.y)
		yDir = 1;
	else
		return false;

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
