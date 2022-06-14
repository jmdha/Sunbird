#ifndef Generic_Helper_Functions_H
#define Generic_Helper_Functions_H

#include <string>
#include "Move.h"

bool IsNumber(char charToCheck);

int GetColumnAsNumber(char column);

int GetRowAsNumber(char row);

bool IsLowercase(char charToCheck);

std::string GetStatString(Move move, int64_t duration, int pieceCount);

#endif