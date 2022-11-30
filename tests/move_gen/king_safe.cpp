#include <stdexcept>
#include <string>
#include <stdlib.h>

#include "../../classes/headers/constants.hh"
#include "../../classes/headers/board.hh"
#include "../../classes/headers/board_importer.hh"
#include "../../classes/headers/move_gen.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    Board board = Board();
    BoardImporter::ImportFEN(&board, (std::string) argv[1]);
    int expectedBool = std::atoi(argv[2]);

    MoveGen gen = MoveGen(board.GetColor());
    int kingSafe = (int) gen.IsKingSafe(board);


    if (expectedBool == kingSafe)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect move count | Expected " + std::to_string(expectedBool) + " - Actual " + std::to_string(kingSafe));
}