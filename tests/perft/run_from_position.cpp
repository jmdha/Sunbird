#include <stdexcept>
#include <string>
#include <cstdlib>

#include "board.hh"
#include "board_importer.hh"
#include "perft.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    int expectedLeafCount = std::atoi(argv[1]);

    std::string FEN = std::string(argv[2]);
    Board board = Board();
    BoardImporter::ImportFEN(&board, FEN);    

    int depth = std::atoi(argv[3]);

    Perft perft = Perft(&board);
    int actualLeafCount = perft.RunFromPosition(depth);

    if (expectedLeafCount == actualLeafCount)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect leaf count | Expected " + std::to_string(expectedLeafCount) + " - Actual " + std::to_string(actualLeafCount));
}