#ifndef BOARDIMPORTER
#define BOARDIMPORTER

#include <string>

#include "board.hh"
#include "constants.hh"
#include "utilities.hh"

class BoardImporter {
public:
    static void ImportFEN(Board* board, std::string FEN);
    static void ImportMoveSequence(Board* board, std::string moves);

private:
};
#endif
