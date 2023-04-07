#ifndef BOARD_IMPORTER
#define BOARD_IMPORTER

#include <string>

#include "board.hh"
#include "constants.hh"
#include "utilities.hh"

class BoardImporter {
public:
    static Board ImportFEN(std::string FEN);
    static Board ImportMoveSequence(std::string moves);

private:
};
#endif // BOARD_IMPORTER
