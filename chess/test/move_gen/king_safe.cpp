#include <stdexcept>
#include <string>
#include <stdlib.h>

#include <chess/internal/bit_shifts.hpp>
#include <chess/board.hpp>
#include <chess/board_importer.hpp>

int main(int, char* argv[]) {
    BitShifts::Init();
    Board board = BoardImporter::ImportFEN((std::string) argv[1]);
    int expectedBool = std::atoi(argv[2]);

    int kingSafe = board.IsKingSafe();

    if (expectedBool == kingSafe)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect move count | Expected " + std::to_string(expectedBool) + " - Actual " + std::to_string(kingSafe));
}
