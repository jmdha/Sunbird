#include <iostream>

#include "classes/headers/bit_board.hh"
#include "classes/headers/evaluator.hh"
#include "classes/headers/board_importer.hh"
#include "classes/headers/move_gen.hh"

int main(int, char**) {
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    MoveGen moveGen = MoveGen(Color::White);

    std::vector<Move> moves = moveGen.GetAllMoves(board);
}
