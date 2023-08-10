#include <stdexcept>
#include <stdlib.h>
#include <string>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <engine/evaluation.hpp>

using namespace Chess;

int main(int, char *argv[]) {
    Board board = Import::FEN((std::string)argv[2]);
    int expectedEval = std::atoi(argv[1]);

    int eval = Engine::Evaluation::EvalMaterial(board.Pos());

    if (eval == expectedEval)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect Evaluation | Expected " + (std::string)argv[1] +
                               " - Actual " + std::to_string(eval));
}
