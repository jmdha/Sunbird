#ifndef BOARDIMPORTER
#define BOARDIMPORTER

#include <string>

#include "bit_board.hh"
#include "constants.hh"
#include "utilities.hh"

class BoardImporter {
public:
    static void ImportFEN(BitBoard* board, std::string FEN);
    static void ImportMoveSequence(BitBoard* board, std::string moves);

private:
};
#endif
