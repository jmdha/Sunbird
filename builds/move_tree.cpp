#include <iostream>

#include "board.hh"
#include "board_importer.hh"
#include "move_tree_gen.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    Board board = Board();
    BoardImporter::ImportFEN(&board, argv[1]);
    MoveTreeGenerator moveGen = MoveTreeGenerator(&board, argv[2], std::atoi(argv[5]));
    moveGen.GenerateTreeToFile(std::atoi(argv[3]), std::atoi(argv[4]));
}
