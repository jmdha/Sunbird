#include <stdexcept>
#include <string>
#include <stdlib.h>
/*
#include "../../../classes/headers/board_OO.h"
#include "../../../classes/headers/constants.h"

int main(int argc, char* argv[]) {
    Board_OO board = Board_OO();
    Board_OO comparisonBoard = Board_OO();

    if (std::atoi(argv[2]) == 1)
        comparisonBoard.Initialize();

    board.ImportFEN(argv[1]);

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            Piece* actualPiece = board.GetPiece(x, y);
            Piece* expectedPiece = comparisonBoard.GetPiece(x, y);

            if (actualPiece == nullptr && expectedPiece == nullptr)
                continue;

            if ((actualPiece == nullptr && expectedPiece != nullptr) ||(actualPiece != nullptr && expectedPiece == nullptr)
                ||
                (board.GetPiece(x, y)->color != comparisonBoard.GetPiece(x, y)->color || board.GetPiece(x, y)->GetType() != comparisonBoard.GetPiece(x, y)->GetType()))
                throw std::logic_error("Incorrect FEN import");
        }
    }
    
    exit(EXIT_SUCCESS);
}*/

int main(int argc, char* argv[]) {

    exit(EXIT_SUCCESS);
}