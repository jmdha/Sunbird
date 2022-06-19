#include <iostream>

#include "classes/headers/bit_board.hh"
#include "classes/headers/bit_evaluator.hh"
#include "classes/headers/board_importer.hh"

int main(int, char**) {
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, "8/8/8/8/8/8/8/R7 w KQkq - 0 1");
    BitEvaluator evaluator = BitEvaluator();
    evaluator.EvaluatePieceCount(board);
}
