#ifndef BOARD_IMPORTER
#define BOARD_IMPORTER

#include <string>

#include "internal/constants.hpp"
#include "internal/utilities.hpp"
#include "board.hpp"

class BoardImporter {
public:
    static Board ImportFEN(std::string FEN);
    static Board ImportMoveSequence(std::string moves);

private:
};
#endif // BOARD_IMPORTER
