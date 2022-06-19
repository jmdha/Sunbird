#ifndef BOARDIMPORTER
#define BOARDIMPORTER

#include <string>

#include "constants.hh"
#include "board.hh"
#include "utilities.hh"

class BoardImporter {
public:
    static void ImportFEN(Board* board, std::string FEN);

private:
};
#endif