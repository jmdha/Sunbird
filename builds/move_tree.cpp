#include <iostream>

#include "../classes/headers/bit_board.hh"
#include "../classes/headers/board_importer.hh"
#include "../classes/headers/move_tree_gen.hh"

int main(int argc, char* argv[]) {
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, argv[1]);
    MoveTreeGenerator moveGen = MoveTreeGenerator(&board, argv[2]);
    moveGen.GenerateTreeToFile(std::atoi(argv[3]), std::atoi(argv[4]));
}
