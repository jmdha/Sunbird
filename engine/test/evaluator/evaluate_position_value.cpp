#include <stdexcept>
#include <stdlib.h>
#include <string>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <chess/internal/constants.hpp>
#include <engine/evaluation.hpp>

using namespace Chess;

int main(int, char *argv[]) {
    Board board = Import::FEN(std::string(argv[2]));
    int expectedEval = std::atoi(argv[1]);

    int eval = Engine::Evaluation::EvalPosition(board);

    if (eval == expectedEval)
        exit(EXIT_SUCCESS);
    else {
        printf("Incorrect Evaluation. Expected %s found %d.", argv[1], eval);
        exit(EXIT_FAILURE);
    }
}
