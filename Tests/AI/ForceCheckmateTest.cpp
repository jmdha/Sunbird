#include <stdexcept>
#include <string>
#include <stdlib.h>

#include "../../Classes/Headers/ai.h"
#include "../../Classes/Headers/board.h"

int main(int argc, char* argv[]) {
    Board* board = new Board();
    board->ImportFEN(argv[1]);

    int moveCount = 0;

    Move move = GenerateMove(board, 1, moveCount);

    if (move.GetMoveAsPlainString() == (std::string)argv[2])
        exit(EXIT_SUCCESS);

    throw std::logic_error("Non equal strings " + (std::string)move.GetMoveAsPlainString() + " " + (std::string)argv[2]);
    
}