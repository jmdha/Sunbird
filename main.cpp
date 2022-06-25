#include <iostream>

#include "classes/headers/bit_board.hh"
#include "classes/headers/board_importer.hh"
#include "classes/headers/minimax.hh"
#include "classes/headers/move_gen.hh"
#include "classes/headers/perft.hh"

int main(int, char**) {
    int expectedLeafCount = 48;

    std::string FEN = std::string("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ");
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, FEN);    

    int depth = 1;

    Perft perft = Perft(&board);
    int actualLeafCount = perft.RunFromPosition(depth);

    if (expectedLeafCount == actualLeafCount)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect leaf count | Expected " + std::to_string(expectedLeafCount) + " - Actual " + std::to_string(actualLeafCount));
}
