#include "./Headers/Zobrist.h"
#include <stdlib.h>
#include <math.h>
#include <random>
#include <limits>

Zobrist::Zobrist(Board* board) {
	this->board = board;
}

unsigned long int Zobrist::getHash() {
	return this->currentHash;
}

int Zobrist::GetCurrentInstanceCount()
{
	return evalPoints.at(currentHash).GetInstanceCount();
}

bool Zobrist::GetValue(int depth, int& value)
{
	return evalPoints.at(currentHash).GetValue(depth, value);
}

void Zobrist::flipSquare(int x, int y, int index, int color) {
	unsigned long int value = ZobristValueBoard[x][y][index][color];
	this->currentHash = this->currentHash ^ value;
}

void Zobrist::StoreEval(int depth, int value)
{
	evalPoints.at(currentHash).StoreEval(depth, value);
}

void Zobrist::initializeHash() {
	this->currentHash = 0;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 2; j++) {
					flipSquare(x, y, this->board->GetPiece(x, y)->GetIndex(), j);
				}
			}
		}
	}
}

void Zobrist::IncrementCurrentHash()
{
	if (this->evalPoints.find(this->getHash()) == this->evalPoints.end()) {
		this->evalPoints.emplace(this->getHash(), EvalPoint());
	}
	this->evalPoints.at(this->getHash()).IncrementCurrentHash();
}

void Zobrist::DecrementCurrentHash() {
	this->evalPoints.at(this->getHash()).DecrementCurrentHash();
}

void Zobrist::Clear()
{
	evalPoints.clear();
}
