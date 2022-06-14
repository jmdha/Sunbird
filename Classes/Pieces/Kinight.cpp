#include "Headers/Knight.h"

Knight::Knight(Color color) : Piece(color) {
	this->color = color;
}

PieceChar Knight::GetPieceChar() {
	if(this->color == Color::White)
		return PieceChar::KnightWhite;
	else
		return PieceChar::KnightBlack;
}

std::vector<Move> Knight::GetPossibleMoves(Board board) {
	std::vector<Move> moves;

	for(int i = 0; i < 2; i++) {
		for(int i2 = -1; i2 <= 1; i2 += 2) {
			for(int i3 = -2; i3 <= 2; i3 += 4) {
				Point endPoint(this->x, this->y);
				if(i == 0) {
					endPoint.x += i3;
					endPoint.y += i2;
				} else {
					endPoint.x += i2;
					endPoint.y += i3;
				}

				if(endPoint.x >= WIDTH || endPoint.x < 0 || endPoint.y >= HEIGHT || endPoint.y < 0)
					continue;
				if(board.IsSquareEmpty(endPoint.x, endPoint.y) || board.IsSquareEnemy(this->color, endPoint.x, endPoint.y)) {
					moves.push_back(Move(this->x, this->y, endPoint.x, endPoint.y));
				}
			}
		}
	}

	return moves;
}

Move Knight::GetMoveIfPossible(Board board, Point endPos) {
	for(int i = 0; i < 2; i++) {
		for(int i2 = -1; i2 <= 1; i2 += 2) {
			for(int i3 = -2; i3 <= 2; i3 += 4) {
				Point endPoint(this->x, this->y);
				if(i == 0) {
					endPoint.x += i3;
					endPoint.y += i2;
				} else {
					endPoint.x += i2;
					endPoint.y += i3;
				}

				if(endPoint.x >= WIDTH || endPoint.x < 0 || endPoint.y >= HEIGHT || endPoint.y < 0)
					continue;
				if(board.IsSquareEmpty(endPoint.x, endPoint.y) || board.IsSquareEnemy(this->color, endPoint.x, endPoint.y)) {
					if(endPoint.x == endPos.x && endPoint.y == endPos.y)
						return Move(this->x, this->y, endPoint.x, endPoint.y);
				}
			}
		}
	}

	return Move(-1, -1, -1, -1);
}

int Knight::GetValue() {
	return (int) Value::Knight + ValueKnightPos[x][(this->color == Color::White) ? y : HEIGHT - 1];
}

int Knight::GetIndex() {
	return (int) PieceIndex::Knight;
}

bool Knight::CheckIfPosPossible(Board board, Point pos) {
	int xDir;
	int yDir;
	if(x < pos.x)
		xDir = 1;
	else if(x > pos.x)
		xDir = -1;
	else
		return false;
	if(y < pos.y)
		yDir = 1;
	else if(y > pos.y)
		yDir = -1;
	else
		return false;

	int xOffset[2] = {1, 2};
	int yOffset[2] = {2, 1};

	for(int i = 0; i < 2; i++)
		if(x + xOffset[i] * xDir == pos.x && y + yOffset[i] * yDir == pos.y)
			return true;


	return false;
}
