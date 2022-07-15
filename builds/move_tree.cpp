#include <iostream>

#include "../classes/headers/bit_board.hh"
#include "../classes/headers/board_importer.hh"
#include "../classes/headers/move_tree_gen.hh"

int main(int argc, char* argv[]) {
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, "rnbqkbnr/pp2pppp/8/2pp2N1/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 3");
    MoveTreeGenerator moveGen = MoveTreeGenerator(&board, "tree.JSON");
    moveGen.GenerateTreeToFile(5);
}
