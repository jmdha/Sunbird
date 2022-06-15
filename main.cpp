#include <iostream>

#include "classes/headers/bit_board.h"
#include "classes/headers/bit_evaluator.h"
#include "classes/headers/board_importer.h"

int main(int, char**) {
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, "8/8/8/8/8/8/8/R7 w KQkq - 0 1");
    BitEvaluator evaluator = BitEvaluator();
    evaluator.EvaluatePieceCount(board);
}
