#ifndef BOARDIMPORTER
#define BOARDIMPORTER

#include <string>

#include "constants.h"
#include "board.h"
#include "utilities.h"

class BoardImporter {
public:
    static void ImportFEN(Board* board, std::string FEN);

private:
};
#endif