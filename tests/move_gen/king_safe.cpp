#include <stdexcept>
#include <string>
#include <stdlib.h>

#include "bit_shifts.hh"
#include "board.hh"
#include "board_importer.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    Board board = Board();
    BoardImporter::ImportFEN(&board, (std::string) argv[1]);
    int expectedBool = std::atoi(argv[2]);

    int kingSafe = board.IsKingSafe();

    if (expectedBool == kingSafe)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect move count | Expected " + std::to_string(expectedBool) + " - Actual " + std::to_string(kingSafe));
}