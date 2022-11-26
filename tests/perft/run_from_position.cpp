#include <stdexcept>
#include <string>
#include <stdlib.h>

#include <string>

#include "../../classes/headers/constants.hh"
#include "../../classes/headers/bit_board.hh"
#include "../../classes/headers/board_importer.hh"
#include "../../classes/headers/move.hh"
#include "../../classes/headers/perft.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    int expectedLeafCount = std::atoi(argv[1]);

    std::string FEN = std::string(argv[2]);
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, FEN);    

    int depth = std::atoi(argv[3]);

    Perft perft = Perft(&board);
    int actualLeafCount = perft.RunFromPosition(depth);

    if (expectedLeafCount == actualLeafCount)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect leaf count | Expected " + std::to_string(expectedLeafCount) + " - Actual " + std::to_string(actualLeafCount));
}