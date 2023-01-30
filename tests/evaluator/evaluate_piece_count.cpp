#include <stdexcept>
#include <string>
#include <stdlib.h>

#include "board.hh"
#include "board_importer.hh"
#include "evaluator.hh"

int main(int argc, char* argv[]) {
    Board board = Board();
    BoardImporter::ImportFEN(&board, (std::string) argv[2]);
    int expectedEval = std::atoi(argv[1]);
    Evaluator evaluator = Evaluator(board.GetColor());

    int eval = evaluator.EvaluatePieceCount(board);

    if (eval == expectedEval)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect Evaluation | Expected " + (std::string)argv[1] + " - Actual " + std::to_string(eval));
}