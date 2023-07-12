#include <stdexcept>
#include <string>
#include <stdlib.h>

#include <chess/internal/constants.hpp>
#include <chess/board.hpp>
#include <chess/import.hpp>
#include "engine/evaluator.hpp"

int main(int, char* argv[]) {
    Board board = Import::FEN((std::string) argv[2]);
    int expectedEval = std::atoi(argv[1]);

    int eval = Evaluator::EvaluatePositionValue(board);

    if (eval == expectedEval)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect Evaluation | Expected " + (std::string)argv[1] + " - Actual " + std::to_string(eval));
}

