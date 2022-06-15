#ifndef Zobrist_H
#define Zobrist_H

class Board;

#include <unordered_map>
#include "constants.h"
#include "board.h"
#include "EvalPoint.h"

class Zobrist {

public:
	Zobrist(Board* board);
	unsigned long int getHash();
	int GetCurrentInstanceCount();
	bool GetValue(int depth, int& value);
	void initializeHash();
	void flipSquare(int x, int y, int index, int color);
	void StoreEval(int depth, int value);
	void IncrementCurrentHash();
	void DecrementCurrentHash();
	void Clear();


private:
	unsigned long int currentHash;
	std::unordered_map<unsigned long int, EvalPoint> evalPoints;
	Board* board;
};

#endif