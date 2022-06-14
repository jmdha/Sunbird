#include "Headers/King.h"

King::King(Color color) : Piece(color) {
	this->color = color;
}

PieceChar King::GetPieceChar() {
	if(this->color == Color::White)
		return PieceChar::KingWhite;
	else
		return PieceChar::KingBlack;
}

std::vector<Move> King::GetPossibleMoves(Board board) {
	std::vector<Move> moves;

	std::vector<Move>::iterator it;
	it = moves.begin();
	std::vector<Move> tempMoveList;

	for(int i = 1; i <= 9; i++) {
		if(i == 5)
			continue;
		tempMoveList = this->GetPossibleMovesInDirection(board, (Direction) i, 1);
		moves.insert(it, tempMoveList.begin(), tempMoveList.end());
		it = moves.begin();
	}

	if(board.castlingValid[(int) this->color][0]) {
		bool valid = true;
		for(int i = this->x - 1; i > 0; i--) {
			if(board.GetPiece(i, this->y) != NULL) {
				valid = false;
				break;
			}
		}
		if(valid) {
			if(getCastlingPossibility(board, CastlingDirection::Left)) {
				Move move = Move(this->x, this->y, 2, this->y);
				move.castling = true;
				moves.push_back(move);
			}
		}
	}
	if(board.castlingValid[(int)this->color][1]) {
		bool valid = true;
		for(int i = this->x + 1; i < 7; i++) {
			if(board.GetPiece(i, this->y) != NULL) {
				valid = false;
				break;
			}
		}
		if(valid) {
			if(getCastlingPossibility(board, CastlingDirection::Right)) {
				Move move = Move(this->x, this->y, 6, this->y);
				move.castling = true;
				moves.push_back(move);
			}
		}
	}

	return moves;
}

// does not check for castling
Move King::GetMoveIfPossible(Board board, Point endPos) {
	std::vector<Move> moves;

	for(int i = 1; i <= 9; i++) {
		if(i == 5)
			continue;
		moves = this->GetPossibleMovesInDirection(board, (Direction) i, 1);
		for(int i = 0; i < (int) moves.size(); i++) {
			if(moves[i].endX == endPos.x && moves[i].endY == endPos.y)
				return moves[i];
		}
	}

	return Move(-1, -1, -1, -1);
}

int King::GetValue() {
	return 0 + ValueKingPosMid[x][(this->color == Color::White) ? y : HEIGHT - 1];
}

int King::GetIndex() {
	return (int) PieceIndex::King;
}

bool King::getCastlingPossibility(Board board, CastlingDirection direction) {
	Piece* pieces[16] = {};
	int pieceCount = 0;
	for(int x2 = 0; x2 < 8; x2++) {
		for(int y2 = 0; y2 < 8; y2++) {
			Piece* piece = board.GetPiece(x2, y2);
			if(piece != nullptr && piece->color != color) {
				pieces[pieceCount] = piece;
				pieceCount++;
			}
		}
	}

	// checking if in check
	for(int i = 0; i < pieceCount; i++) {
		if(pieces[i]->CheckIfPosPossible(board, Point(x, y)))
			return false;
	}

	// check�ng right
	if(direction == CastlingDirection::Right) {
		for(int i = 0; i < pieceCount; i++) {
			for(int x2 = 1; x2 < 3; x2++)
				if(pieces[i]->CheckIfPosPossible(board, Point(x + x2, y)))
					return false;
		}
		return true;
	} else {
		for(int i = 0; i < pieceCount; i++) {
			for(int x2 = 1; x2 <= 3; x2++)
				if(pieces[i]->CheckIfPosPossible(board, Point(x - x2, y)))
					return false;
		}
		return true;
	}
}

bool King::CheckIfPosPossible(Board board, Point pos) {
	for(int x = -1; x < 2; x++)
		for(int y = -1; y < 2; y++)
			if(this->x + x == pos.x && this->y + y == pos.y)
				return true;
	return false;
}
