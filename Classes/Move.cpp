#include "Headers/Move.h"

Move::Move() {
	this->startX = -1;
	this->startY = -1;
	this->endX = -1;
	this->endY = -1;
	SetTarget(NULL);
}

Move::Move(int startX, int startY, int endX, int endY) {
	this->startX = startX;
	this->startY = startY;
	this->endX = endX;
	this->endY = endY;
	SetTarget(NULL);
}

Move::Move(std::string move) {
	this->startX = (int) move[0] - 97;
	this->startY = move[1] - '0' - 1;
	if(move[2] == ' ') {
		this->endX = (int) move[3] - 97;
		this->endY = move[4] - '0' - 1;
	} else {
		this->endX = (int) move[2] - 97;
		this->endY = move[3] - '0' - 1;
	}



	SetTarget(NULL);
}

std::string Move::GetMoveAsString() {
	if(castling) {
		if(this->endX == 6)
			return "O-O";
		else
			return "O-O-O";
	}

	return (char) (this->startX + 97)
		+ std::to_string(this->startY + 1)
		+ (char) (this->endX + 97)
		+ std::to_string(this->endY + 1);
}

// Difference between this and getMoveAsString is that this prints all moves as a single piece moving from one square to another;
std::string Move::GetMoveAsPlainString() {
	return (char) (this->startX + 97)
		+ std::to_string(this->startY + 1)
		+ (char) (this->endX + 97)
		+ std::to_string(this->endY + 1);
}

void Move::SetTarget(Piece* target) {
	this->target = target;
}
