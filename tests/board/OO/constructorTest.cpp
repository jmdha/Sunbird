/*#include <stdexcept>
#include <string>
#include <stdlib.h>

#include "../../../classes/headers/board_OO.h"
#include "../../../classes/headers/constants.h"

int main(int argc, char* argv[]) {
    Board_OO board = Board_OO();
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            if (board.GetPiece(x, y) != nullptr)
                throw std::logic_error("Board not null");
        }
    }
    exit(EXIT_SUCCESS);
}*/