#include <stdexcept>
#include <string>
#include <stdlib.h>

#include "../../classes/headers/board.h"
#include "../../classes/headers/board_OO.h"
#include "../../classes/headers/constants.h"
#include "../../classes/headers/eval.h"

int main(int argc, char* argv[]) {
    Board_OO board = Board_OO();
    board.Initialize();

    int eval = Eval::EvalBoard(&board);

    if (eval == std::atoi(argv[1]))
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect evaluation | Expected " + (std::string)argv[1] + " - Actual " + std::to_string(eval));
}