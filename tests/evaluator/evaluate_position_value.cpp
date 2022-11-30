#include <stdexcept>
#include <string>
#include <stdlib.h>

#include "../../classes/headers/constants.hh"
#include "../../classes/headers/board.hh"
#include "../../classes/headers/board_importer.hh"
#include "../../classes/headers/evaluator.hh"

int main(int argc, char* argv[]) {
    Board board = Board();
    BoardImporter::ImportFEN(&board, (std::string) argv[2]);
    int expectedEval = std::atoi(argv[1]);
    Evaluator evaluator = Evaluator(board.GetColor());

    int eval = evaluator.EvaluatePositionValue(board);

    if (eval == expectedEval)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect Evaluation | Expected " + (std::string)argv[1] + " - Actual " + std::to_string(eval));
}