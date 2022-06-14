#include "Headers/pawn.h"

Pawn::Pawn(Color color) : Piece(color) {
	this->color = color;
}

PieceChar Pawn::GetPieceChar() {
	if(this->color == Color::White)
		return PieceChar::PawnWhite;
	else
		return PieceChar::PawnBlack;
}

std::vector<Move> Pawn::GetPossibleMoves(Board board) {
	std::vector<Move> moves;

	std::vector<Move>::iterator it;
	it = moves.begin();
	std::vector<Move> tempMoveList;

	Direction dir = (this->color == Color::White) ? Direction::North : Direction::South;

	// check move
	int moveLength = 1;
	if(this->color == Color::White && this->y == (int) PawnRow::White)
		moveLength = 2;
	else if(this->color == Color::Black && this->y == (int) PawnRow::Black)
		moveLength = 2;

	tempMoveList = this->GetPossibleMovesInDirection(board, dir, moveLength, false);
	// look if there are two moves i.e. it can both move one square forth and two
	if(tempMoveList.size() == 2) {
		tempMoveList[1].pawnDoubleMove = true;
	} else if(tempMoveList.size() == 1) {
		if(tempMoveList[0].endY == ((this->color == Color::White) ? (int) BackRow::Black : (int) BackRow::White)) {
			Move tempMove = tempMoveList[0];
			for(int i = 0; i < 3; i++) {
				Move newMove = Move(tempMove.startX, tempMove.startY, tempMove.endX, tempMove.endY);
				tempMoveList.push_back(newMove);
			}

			for(int i = 0; i < (int) tempMoveList.size(); i++)
				tempMoveList[i].promotion = true;

			tempMoveList[0].promotionType = PieceIndex::Rook;
			tempMoveList[1].promotionType = PieceIndex::Knight;
			tempMoveList[2].promotionType = PieceIndex::Bishop;
			tempMoveList[3].promotionType = PieceIndex::Queen;
		}
	}
	moves.insert(it, tempMoveList.begin(), tempMoveList.end());
	it = moves.begin();
	
	// check capture to the right
	tempMoveList = this->GetPossibleMovesInDirection(board, (dir == Direction::North) ? Direction::NorthEast : Direction::SouthEast, 1, true);
	if(tempMoveList.size() == 1) {
		if(tempMoveList[0].endY == ((this->color == Color::White) ? (int) BackRow::Black : (int) BackRow::White)) {
			Move tempMove = tempMoveList[0];
			for(int i = 0; i < 3; i++) {
				Move newMove = Move(tempMove.startX, tempMove.startY, tempMove.endX, tempMove.endY);
				tempMoveList.push_back(newMove);
			}

			for(int i = 0; i < (int) tempMoveList.size(); i++)
				tempMoveList[i].promotion = true;

			tempMoveList[0].promotionType = PieceIndex::Rook;
			tempMoveList[1].promotionType = PieceIndex::Knight;
			tempMoveList[2].promotionType = PieceIndex::Bishop;
			tempMoveList[3].promotionType = PieceIndex::Queen;
		}
	}
	moves.insert(it, tempMoveList.begin(), tempMoveList.end());
	it = moves.begin();

	// check capture to the left
	tempMoveList = this->GetPossibleMovesInDirection(board, (dir == Direction::North) ? Direction::NorthWest : Direction::SouthWest, 1, true);
	if(tempMoveList.size() == 1) {
		if(tempMoveList[0].endY == ((this->color == Color::White) ? (int) BackRow::Black : (int) BackRow::White)) {
			Move tempMove = tempMoveList[0];
			for(int i = 0; i < 3; i++) {
				Move newMove = Move(tempMove.startX, tempMove.startY, tempMove.endX, tempMove.endY);
				tempMoveList.push_back(newMove);
			}

			for(int i = 0; i < (int) tempMoveList.size(); i++)
				tempMoveList[i].promotion = true;

			tempMoveList[0].promotionType = PieceIndex::Rook;
			tempMoveList[1].promotionType = PieceIndex::Knight;
			tempMoveList[2].promotionType = PieceIndex::Bishop;
			tempMoveList[3].promotionType = PieceIndex::Queen;
		}
	}
	moves.insert(it, tempMoveList.begin(), tempMoveList.end());
	it = moves.begin();


	// get en passant move
	if(board.enPassant != -1) {
		if((color == Color::White && y == (int) EnPassantRow::White) || (color == Color::Black && y == (int) EnPassantRow::Black)) {
			if(board.enPassant == x - 1) {
				tempMoveList = this->GetPossibleMovesInDirection(board, (dir == Direction::North) ? Direction::NorthWest : Direction::SouthWest, 1, false);
				if(tempMoveList.size() == 1)
					tempMoveList[0].enPassantTake = true;
				moves.insert(it, tempMoveList.begin(), tempMoveList.end());
				it = moves.begin();
			} else if(board.enPassant == x + 1) {
				tempMoveList = this->GetPossibleMovesInDirection(board, (dir == Direction::North) ? Direction::NorthEast : Direction::SouthEast, 1, false);
				if(tempMoveList.size() == 1)
					tempMoveList[0].enPassantTake = true;
				moves.insert(it, tempMoveList.begin(), tempMoveList.end());
				it = moves.begin();
			}
		}
	}


	return moves;
}

Move Pawn::GetMoveIfPossible(Board board, Point endPos) {
	std::vector<Move> moves;
	Direction dir = (this->color == Color::White) ? Direction::North : Direction::South;

	// check move
	int moveLength = 1;
	if(this->color == Color::White && this->y == (int) PawnRow::White)
		moveLength = 2;
	else if(this->color == Color::Black && this->y == (int) PawnRow::Black)
		moveLength = 2;

	moves = this->GetPossibleMovesInDirection(board, dir, moveLength, false);
	// look if there are two moves i.e. it can both move one square forth and two
	if(moves.size() == 2) {
		moves[1].pawnDoubleMove = true;
	} else if(moves.size() == 1) {
		if(moves[0].endY == ((this->color == Color::White) ? (int) BackRow::Black : (int) BackRow::White)) {
			Move tempMove = moves[0];
			for(int i = 0; i < 3; i++) {
				Move newMove = Move(tempMove.startX, tempMove.startY, tempMove.endX, tempMove.endY);
				moves.push_back(newMove);
			}

			for(int i = 0; i < (int) moves.size(); i++)
				moves[i].promotion = true;

			moves[0].promotionType = PieceIndex::Rook;
			moves[1].promotionType = PieceIndex::Knight;
			moves[2].promotionType = PieceIndex::Bishop;
			moves[3].promotionType = PieceIndex::Queen;
		}
	}

	for(int i = 0; i < (int) moves.size(); i++)
		if(moves[i].endX == endPos.x && moves[i].endY == endPos.y)
			return moves[i];

	// check capture to the right
	moves = this->GetPossibleMovesInDirection(board, (dir == Direction::North) ? Direction::NorthEast : Direction::SouthEast, 1, true);
	if(moves.size() == 1) {
		if(moves[0].endY == ((this->color == Color::White) ? (int) BackRow::Black : (int) BackRow::White)) {
			Move tempMove = moves[0];
			for(int i = 0; i < 3; i++) {
				Move newMove = Move(tempMove.startX, tempMove.startY, tempMove.endX, tempMove.endY);
				moves.push_back(newMove);
			}

			for(int i = 0; i < (int) moves.size(); i++)
				moves[i].promotion = true;

			moves[0].promotionType = PieceIndex::Rook;
			moves[1].promotionType = PieceIndex::Knight;
			moves[2].promotionType = PieceIndex::Bishop;
			moves[3].promotionType = PieceIndex::Queen;
		}
		for(int i = 0; i < (int) moves.size(); i++)
			if(moves[i].endX == endPos.x && moves[i].endY == endPos.y)
				return moves[i];
	}


	// check capture to the left
	moves = this->GetPossibleMovesInDirection(board, (dir == Direction::North) ? Direction::NorthWest : Direction::SouthWest, 1, true);
	if(moves.size() == 1) {
		if(moves[0].endY == ((this->color == Color::White) ? (int) BackRow::Black : (int) BackRow::White)) {
			Move tempMove = moves[0];
			for(int i = 0; i < 3; i++) {
				Move newMove = Move(tempMove.startX, tempMove.startY, tempMove.endX, tempMove.endY);
				moves.push_back(newMove);
			}

			for(int i = 0; i < (int) moves.size(); i++)
				moves[i].promotion = true;

			moves[0].promotionType = PieceIndex::Rook;
			moves[1].promotionType = PieceIndex::Knight;
			moves[2].promotionType = PieceIndex::Bishop;
			moves[3].promotionType = PieceIndex::Queen;
		}
		for(int i = 0; i < (int) moves.size(); i++)
			if(moves[i].endX == endPos.x && moves[i].endY == endPos.y)
				return moves[i];
	}


	// get en passant move
	if(board.enPassant != -1) {
		if(board.enPassant == x - 1) {
			moves = this->GetPossibleMovesInDirection(board, (dir == Direction::North) ? Direction::NorthWest : Direction::SouthWest, 1, false);
			if(moves.size() == 1) {
				moves[0].enPassantTake = true;
				if(moves[0].endX == endPos.x && moves[0].endY == endPos.y)
					return moves[0];
			}
		} else if(board.enPassant == x + 1) {
			moves = this->GetPossibleMovesInDirection(board, (dir == Direction::North) ? Direction::NorthEast : Direction::SouthEast, 1, false);
			if(moves.size() == 1) {
				moves[0].enPassantTake = true;
				if(moves[0].endX == endPos.x && moves[0].endY == endPos.y)
					return moves[0];
			}
		}
	}

	return Move(-1, -1, -1, -1);
}

int Pawn::GetValue() {
	return (int) Value::Pawn + ValuePawnPos[x][(this->color == Color::White) ? y : HEIGHT - 1];
}

int Pawn::GetIndex() {
	return (int) PieceIndex::Pawn;
}

bool Pawn::CheckIfPosPossible(Board board, Point pos) {
	int newY = ((color == Color::White) ? +1 : -1);
	for(int newX = -1; newX < 2; newX += 2) {
		if(pos.x == x + newX && pos.y == y + newY)
			return true;
	}
	return false;
}
