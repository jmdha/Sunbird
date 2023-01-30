#ifndef BOARD_IMPORTER
#define BOARD_IMPORTER

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
#endif // BOARD_IMPORTER
