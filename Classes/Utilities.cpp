#include "Headers/Utilities.h"

bool IsNumber(char charToCheck) {
	if(int(charToCheck) > 47 && int(charToCheck) < 58)
		return true;
	else
		return false;
}

int GetColumnAsNumber(char column) {
	if(IsLowercase(column))
		return int(column) - 97;
	else
		return int(column) - 65;
}

int GetRowAsNumber(char row) {
	return int(row) - 49;
}

bool IsLowercase(char charToCheck) {
	if(int(charToCheck) > 64 && int(charToCheck) < 91)
		return false;
	else
		return true;
}

std::string GetStatString(Move move, int64_t duration, int pieceCount) {
	std::string statString = "";
	statString.append("Piece count: " + std::to_string(pieceCount) + "\n");
	statString.append("Time (ms): " + std::to_string(duration) + "\n");
	statString.append("Total moves: " + std::to_string(move.totalMoves));

	return statString;
}