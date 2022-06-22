#include <iostream>

#include "classes/headers/bit_board.hh"
#include "classes/headers/evaluator.hh"
#include "classes/headers/board_importer.hh"
#include "classes/headers/move_gen.hh"

int main(int, char**) {
    BitBoard board = BitBoard();
    board.Initialize();
    MoveGen moveGen = MoveGen();
    moveGen.GetPawnMoves(Color::White, board);
}
