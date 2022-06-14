#include "Headers/Piece.h"
#include <iostream>

Piece::Piece(Color color) {
	this->color = color;
}

PieceChar Piece::GetPieceChar() {
	return PieceChar::Blank;
}

std::vector<Move> Piece::GetPossibleMoves(Board board) {
	return {};
}

Move Piece::GetMoveIfPossible(Board board, Point endPos) {
	return {};
}

int Piece::GetValue() {
	return 0;
}

int Piece::GetIndex() {
	return 0;
}

// This asssumes either empty square or enemy piece is a valid end square
std::vector<Move> Piece::GetPossibleMovesInDirection(Board board, Direction direction, int length) {
	// Max moves is 7 as that is board width/height minus the piece
	std::vector<Move> moves;

	for(int i = 1; i <= length; i++) {

		Point endPoint = GetNewEndPoint(Point(this->x, this->y), direction, i);

		if(endPoint.x >= WIDTH || endPoint.x < 0 || endPoint.y >= HEIGHT || endPoint.y < 0)
			break;

		if(board.IsSquareEmpty(endPoint.x, endPoint.y)) {
			moves.push_back(Move(this->x, this->y, endPoint.x, endPoint.y));
		} else if(board.IsSquareEnemy(this->color, endPoint.x, endPoint.y)) {
			moves.push_back(Move(this->x, this->y, endPoint.x, endPoint.y));
			break;
		} else
			break;
	}
	return moves;
}

// This asssumes either enemy piece is a valid end square
std::vector<Move> Piece::GetPossibleMovesInDirection(Board board, Direction direction, int length, bool onlyCapture) {
	// Max moves is 7 as that is board width/height minus the piece
	std::vector<Move> moves;

	for(int i = 1; i <= length; i++) {

		Point endPoint = GetNewEndPoint(Point(this->x, this->y), direction, i);

		if(endPoint.x >= WIDTH || endPoint.x < 0 || endPoint.y >= HEIGHT || endPoint.y < 0)
			break;

		if(!onlyCapture) {
			if(board.IsSquareEmpty(endPoint.x, endPoint.y))
				moves.push_back(Move(this->x, this->y, endPoint.x, endPoint.y));
			else
				break;
		} else if(board.IsSquareEnemy(this->color, endPoint.x, endPoint.y)) {
			moves.push_back(Move(this->x, this->y, endPoint.x, endPoint.y));
		} else
			break;
	}
	return moves;
}

Point Piece::GetNewEndPoint(Point priorPos, Direction direction, int moveLength) {
	switch(direction) {
		case Direction::North:
		return Point(priorPos.x, priorPos.y + moveLength);

		case Direction::South:
		return Point(priorPos.x, priorPos.y - moveLength);

		case Direction::East:
		return Point(priorPos.x + moveLength, priorPos.y);

		case Direction::West:
		return Point(priorPos.x - moveLength, priorPos.y);

		case Direction::NorthEast:
		return Point(priorPos.x + moveLength, priorPos.y + moveLength);

		case Direction::NorthWest:
		return Point(priorPos.x - moveLength, priorPos.y + moveLength);

		case Direction::SouthEast:
		return Point(priorPos.x + moveLength, priorPos.y - moveLength);

		case Direction::SouthWest:
		return Point(priorPos.x - moveLength, priorPos.y - moveLength);
		default:
		printf("Direction %i\n", (int) direction);
		throw std::invalid_argument("Invalid direction ");
	}
}

bool Piece::CheckIfPosPossible(Board board, Point pos) {
	return false;
}
